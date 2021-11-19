#include <Hook.hpp>
#include <Mod.hpp>
#include <Log.hpp>
#include <Loader.hpp>

using namespace lilac;

Loader* Loader::get() {
    static auto g_loader = new Loader;
    return g_loader;
}

void Loader::createDirectories() {
    directory_create(const_join_path_c_str<lilac_directory>);
    directory_create(const_join_path_c_str<lilac_directory, lilac_resource_directory>);
    directory_create(const_join_path_c_str<lilac_directory, lilac_plugin_directory>);
}

size_t Loader::updatePlugins() {
    size_t loaded = 0;
    this->createDirectories();
    for (auto const& entry : std::filesystem::directory_iterator(
        std::filesystem::absolute(lilac_directory) / lilac_plugin_directory
    )) {
        if (
            std::filesystem::is_regular_file(entry) &&
            entry.path().extension() == lilac_plugin_extension
        ) {
            if (!vector_contains<BGDPlugin*>(
                this->m_vLoadedPlugins,
                [entry](BGDPlugin* p) -> bool {
                    return p->m_sPath == entry.path().string();
                }
            )) {
                if (this->loadPluginFromFile(entry.path().string())) {
                    loaded++;
                }
            }
        }
    }
    return loaded;
}

bool Loader::isPluginLoaded(std::string const& id) {
    return vector_contains<BGDPlugin*>(
        this->m_vLoadedPlugins,
        [id](BGDPlugin* p) -> bool {
            return p->m_sID == id;
        }
    );
}

BGDPlugin* Loader::getLoadedPlugin(std::string const& id) {
    return vector_select<BGDPlugin*>(
        this->m_vLoadedPlugins,
        [id](BGDPlugin* p) -> bool {
            return p->m_sID == id;
        }
    );
}

std::vector<BGDPlugin*> Loader::getAllPlugins() {
    return this->m_vLoadedPlugins;
}

bool Loader::setup() {
    if (this->m_bIsSetup)
        return true;

    lilac_PLATFORM_CONSOLE(
        loadConsole();
    )
    this->createDirectories();
    BGDInternal::get()->setup();
    this->updatePlugins();
    BGDInternal::get()->loadHooks();
    this->loadData();

    this->m_bIsSetup = true;

    return true;
}

void Loader::loadData() {
    for (auto const& plugin : this->m_vLoadedPlugins) {
        plugin->loadData();
    }
}

void Loader::saveData() {
    for (auto const& plugin : this->m_vLoadedPlugins) {
        plugin->saveData();
    }
}

Loader::Loader() {
    this->m_pLogStream = new BGDLogStream;
}

Loader::~Loader() {
    for (auto const& plugin : this->m_vLoadedPlugins) {
        delete plugin;
    }
    for (auto const& log : this->m_vLogs) {
        delete log;
    }
    delete this->m_pLogStream;
}

void Loader::log(BGDLogMessage* log) {
    this->m_vLogs.push_back(log);
}

void Loader::deleteLog(BGDLogMessage* log) {
    vector_erase(this->m_vLogs, log);
    delete log;
}

std::vector<BGDLogMessage*> const& Loader::getLogs() const {
    return this->m_vLogs;
}

std::vector<BGDLogMessage*> Loader::getLogs(
    std::initializer_list<BGDLogType>  typeFilter,
    std::initializer_list<BGDSeverity> severityFilter
) {
    if (
        !typeFilter.size() && !severityFilter.size()
    ) {
        return this->m_vLogs;
    }

    std::vector<BGDLogMessage*> logs;

    for (auto const& log : this->m_vLogs) {
        for (auto const& type : typeFilter) {
            if (log->getType() == type) {
                logs.push_back(log);
                goto found_this;
            }
        }
        for (auto const& severity : severityFilter) {
            if (log->getSeverity() == severity) {
                logs.push_back(log);
                goto found_this;
            }
        }
        found_this:;
    }

    return logs;
}


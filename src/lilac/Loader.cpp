#include <Hook.hpp>
#include <Mod.hpp>
#include <Log.hpp>
#include <Loader.hpp>
#include <utils.hpp>
#include <Internal.hpp>

USE_LILAC_NAMESPACE();

Loader* Loader::get() {
    static auto g_loader = new Loader;
    return g_loader;
}

void Loader::createDirectories() {
    directory_create(const_join_path_c_str<lilac_directory>);
    directory_create(const_join_path_c_str<lilac_directory, lilac_resource_directory>);
    directory_create(const_join_path_c_str<lilac_directory, lilac_mod_directory>);
}

bool Loader::checkDependencies(Mod* mod) {
    if (!mod->m_dependencies.size()) {
        return true;
    }

    size_t unresolved_count = 0;
    for (auto & [dep_id, value] : mod->m_dependencies) {
        auto& [type, resolved] = value;
        resolved = this->isModLoaded(dep_id.c_str());
        if (!resolved) {
            if (type != DependencyType::Optional) {
                unresolved_count++;
            }
        }
    }

    return unresolved_count;
}

size_t Loader::updateMods() {
    size_t loaded = 0;
    this->createDirectories();
    for (auto const& entry : std::filesystem::directory_iterator(
        std::filesystem::absolute(lilac_directory) / lilac_mod_directory
    )) {
        if (
            std::filesystem::is_regular_file(entry) &&
            entry.path().extension() == lilac_mod_extension
        ) {
            if (!vector_contains<Mod*>(
                this->m_mods,
                [entry](Mod* p) -> bool {
                    return p->m_path == entry.path().string();
                }
            )) {
                if (this->loadModFromFile(entry.path().string())) {
                    loaded++;
                }
            }
        }
    }
    return loaded;
}

bool Loader::isModLoaded(std::string_view const& id) {
    return vector_contains<Mod*>(
        this->m_mods,
        [id](Mod* p) -> bool {
            return p->m_id == id;
        }
    );
}

Mod* Loader::getLoadedMod(std::string_view const& id) {
    return vector_select<Mod*>(
        this->m_mods,
        [id](Mod* p) -> bool {
            return p->m_id == id;
        }
    );
}

std::vector<Mod*> Loader::getLoadedMods() {
    return this->m_mods;
}

void Loader::unloadMod(Mod* mod) {
    vector_erase(this->m_mods, mod);
    // ~Mod will call FreeLibrary 
    // automatically
    delete mod;
}

bool Loader::setup() {
    if (this->m_isSetup)
        return true;
    
    if (!Lilac::get()->setup())
        return false;

    this->createDirectories();
    this->updateMods();

    this->m_isSetup = true;

    return true;
}

Loader::Loader() {
    this->m_logStream = new LogStream;
}

Loader::~Loader() {
    for (auto const& Mod : this->m_mods) {
        delete Mod;
    }
    for (auto const& log : this->m_logs) {
        delete log;
    }
    delete this->m_logStream;
}

LogStream& Loader::logStream() {
    return *this->m_logStream;
}

void Loader::log(LogMessage* log) {
    this->m_logs.push_back(log);
}

void Loader::deleteLog(LogMessage* log) {
    vector_erase(this->m_logs, log);
    delete log;
}

std::vector<LogMessage*> const& Loader::getLogs() const {
    return this->m_logs;
}

std::vector<LogMessage*> Loader::getLogs(
    std::initializer_list<Severity> severityFilter
) {
    if (!severityFilter.size()) {
        return this->m_logs;
    }

    std::vector<LogMessage*> logs;

    for (auto const& log : this->m_logs) {
        if (vector_contains<Severity>(severityFilter, log->getSeverity())) {
            logs.push_back(log);
        }
    }

    return logs;
}


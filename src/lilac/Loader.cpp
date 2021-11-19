#include <Hook.hpp>
#include <Mod.hpp>
#include <Log.hpp>
#include <Loader.hpp>
#include <utils.hpp>

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
                this->m_loadedMods,
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
        this->m_loadedMods,
        [id](Mod* p) -> bool {
            return p->m_id == id;
        }
    );
}

Mod* Loader::getLoadedMod(std::string_view const& id) {
    return vector_select<Mod*>(
        this->m_loadedMods,
        [id](Mod* p) -> bool {
            return p->m_id == id;
        }
    );
}

std::vector<Mod*> Loader::getLoadedMods() {
    return this->m_loadedMods;
}

bool Loader::setup() {
    if (this->m_isSetup)
        return true;

    this->createDirectories();
    this->updateMods();

    this->m_isSetup = true;

    return true;
}

void Loader::loadData() {
    for (auto const& Mod : this->m_loadedMods) {
        Mod->loadData();
    }
}

void Loader::saveData() {
    for (auto const& Mod : this->m_loadedMods) {
        Mod->saveData();
    }
}

Loader::Loader() {
    // this->m_pLogStream = new BGDLogStream;
}

Loader::~Loader() {
    for (auto const& Mod : this->m_loadedMods) {
        delete Mod;
    }
    // for (auto const& log : this->m_vLogs) {
    //     delete log;
    // }
    // delete this->m_pLogStream;
}

// void Loader::log(BGDLogMessage* log) {
//     this->m_vLogs.push_back(log);
// }

// void Loader::deleteLog(BGDLogMessage* log) {
//     vector_erase(this->m_vLogs, log);
//     delete log;
// }

// std::vector<BGDLogMessage*> const& Loader::getLogs() const {
//     return this->m_vLogs;
// }

// std::vector<BGDLogMessage*> Loader::getLogs(
//     std::initializer_list<BGDLogType>  typeFilter,
//     std::initializer_list<BGDSeverity> severityFilter
// ) {
//     if (
//         !typeFilter.size() && !severityFilter.size()
//     ) {
//         return this->m_vLogs;
//     }

//     std::vector<BGDLogMessage*> logs;

//     for (auto const& log : this->m_vLogs) {
//         for (auto const& type : typeFilter) {
//             if (log->getType() == type) {
//                 logs.push_back(log);
//                 goto found_this;
//             }
//         }
//         for (auto const& severity : severityFilter) {
//             if (log->getSeverity() == severity) {
//                 logs.push_back(log);
//                 goto found_this;
//             }
//         }
//         found_this:;
//     }

//     return logs;
// }


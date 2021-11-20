#include <Hook.hpp>
#include <Mod.hpp>
#include <Log.hpp>
#include <Loader.hpp>
#include <utils/utils.hpp>

USE_LILAC_NAMESPACE();

Mod::Mod() {}

Mod::~Mod() {
    this->platformCleanup();
    for (auto const& hook : this->m_hooks) {
        this->removeHook(hook);
    }
}

// BGDLogStream& Mod::log() {
//     return BGDLoader::get()->logStream() << this;
// }

void Mod::setup() {}
void Mod::enable() {}
void Mod::disable() {}
void Mod::saveData() {
    auto path = "BetterGD/plugins/"_s;
    path += this->m_name;
}
void Mod::loadData() {
    auto path = "BetterGD/plugins/"_s;
    path += this->m_name;
}

decltype(Mod::m_id) Mod::getID() const {
    return this->m_id;
}

decltype(Mod::m_name) Mod::getName() const {
    return this->m_name;
}

decltype(Mod::m_developer) Mod::getDeveloper() const {
    return this->m_developer;
}

decltype(Mod::m_credits) Mod::getCredits() const {
    return this->m_credits;
}

decltype(Mod::m_description) Mod::getDescription() const {
    return this->m_description;
}

decltype(Mod::m_details) Mod::getDetails() const {
    return this->m_details;
}

decltype(Mod::m_path) Mod::getPath() const {
    return this->m_path;
}

bool Mod::isEnabled() const {
    return this->m_enabled;
}

std::vector<Hook*> Mod::getHooks() const {
    return this->m_hooks;
}

void Mod::setDependencies(std::unordered_map<std::string, bool> const& dependencies) {
    this->m_dependencies = dependencies;
}

// void Mod::throwError(
//     std::string_view const& info,
//     BGDSeverity severity
// ) {
//     BGDLoader::get()->log(new BGDLogMessage(
//         std::string(info),
//         severity,
//         kBGDLogTypeError,
//         this
//     ));
// }

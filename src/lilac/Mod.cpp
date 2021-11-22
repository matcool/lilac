#include <Hook.hpp>
#include <Mod.hpp>
#include <Log.hpp>
#include <Loader.hpp>
#include <SharedMod.hpp>
#include <utils/utils.hpp>

USE_LILAC_NAMESPACE();

Mod::Mod() : m_version(1, 0, 0) {}

Mod::~Mod() {
    this->platformCleanup();
    for (auto const& hook : this->m_hooks) {
        this->removeHook(hook);
    }
}

void Mod::setup() {}
void Mod::enable() {}
void Mod::disable() {}

void Mod::disableBase() {
    this->m_enabled = false;
    Loader::get()->handleSharedModDependencies(this, &SharedMod::disableMod);
    this->disable();
}

void Mod::enableBase() {
    this->m_enabled = true;
    Loader::get()->handleSharedModDependencies(this, &SharedMod::enableMod);
    this->enable();
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

VersionInfo Mod::getVersion() const {
    return this->m_version;
}

bool Mod::isEnabled() const {
    return this->m_enabled;
}

std::vector<Hook*> Mod::getHooks() const {
    return this->m_hooks;
}

void Mod::setDependencies(std::unordered_map<std::string, DependencyType> const& dependencies) {
    this->m_dependencies = map_remap<
        std::string, DependencyType,
        std::string, Dependency
    >(dependencies, [](std::tuple<std::string, DependencyType> val) -> std::tuple<std::string, Dependency> {
        return { std::get<0>(val), { std::get<1>(val), false } };
    });
}

LogStream& Mod::log() {
    return Loader::get()->logStream() << this;
}

void Mod::throwError(
    std::string_view const& info,
    Severity severity
) {
    Loader::get()->log(new LogMessage(
        std::string(info),
        severity,
        this
    ));
}

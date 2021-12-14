#include <KeybindManager.hpp>
#include <Hook.hpp>
#include <Mod.hpp>
#include <Log.hpp>
#include <Loader.hpp>
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
    for (auto const& hook : this->m_hooks) {
        this->disableHook(hook);
    }
    this->disable();
}

void Mod::enableBase() {
    this->m_enabled = true;
    // note: this will enable hooks that were 
    // disabled prior to disabling the mod. 
    // not good!!!
    for (auto const& hook : this->m_hooks) {
        this->enableHook(hook);
    }
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

bool Mod::isLoaded() const {
    return this->m_loaded;
}

std::vector<Hook*> Mod::getHooks() const {
    return this->m_hooks;
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

bool Mod::addKeybindAction(
    KeybindAction     const& action,
    KeybindList       const& defaults,
    keybind_action_id const& insertAfter
) {
    return KeybindManager::get()->addKeybindAction(
        this, action, defaults, insertAfter
    );
}

bool Mod::removeKeybindAction(keybind_action_id const& id) {
    return KeybindManager::get()->removeKeybindAction(this, id);
}

bool Mod::depends(std::string_view const& id) const {
    return this->ModBase::depends(id);
}

bool ModBase::depends(std::string_view const& id) const {
    return vector_utils::contains<Dependency>(
        this->m_dependencies,
        [id](Dependency t) -> bool { return t.m_name == id; }
    );
}

bool ModBase::hasUnresolvedDependencies() const {
    return vector_utils::contains<Dependency>(
        this->m_dependencies,
        [](Dependency t) -> bool { return t.m_required && !t.m_loaded; }
    );
}

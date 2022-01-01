#include <Loader.hpp>
#include <Mod.hpp>
#include <CApiMod.hpp>
#include <InternalMod.hpp>
#include <Log.hpp>
#include <unzipper.h>
#undef snprintf
#include <json.hpp>

USE_LILAC_NAMESPACE();

using namespace zipper;

Result<Mod*> Loader::checkMetaInformation(std::string const& path) {
    auto unzip = Unzipper(path);
    lilac::byte_array bytes {};
    if (!unzip.extractEntryToMemory("mod.json", bytes)) {
        return Err<>(
            "\"" + path + "\" is missing mod.json "
            "(or is not a zip file at all)"
        );
    }
    auto json = nlohmann::json::parse(bytes.begin(), bytes.end());
    if (!json.is_object()) {
        return Err<>(
            "\"" + path + "/mod.json\" does not have an "
            "object at root despite expected"
        );
    }
    auto schema = 1;
    if (json.contains("lilac") && json["lilac"].is_number_integer()) {
        schema = json["lilac"];
    }
    if (schema < Loader::s_supportedSchemaMin) {
        return Err<>(
            "\"" + path + "\" has a lower target version (" + 
            std::to_string(schema) + ") than this version of "
            "lilac supports (" + std::to_string(Loader::s_supportedSchemaMin) +
            "). You may need to downdate lilac in order to use "
            "this mod."
        );
    }
    if (schema > Loader::s_supportedSchemaMax) {
        return Err<>(
            "\"" + path + "\" has a higher target version (" + 
            std::to_string(schema) + ") than this version of "
            "lilac supports (" + std::to_string(Loader::s_supportedSchemaMax) +
            "). You may need to update lilac in order to use "
            "this mod."
        );
    }
    switch (schema) {
        case 1: {
            if (!json.contains("id")) {
                return Err<>(
                    "\"" + path + "\" lacks a Mod ID"
                );
            }
            auto mod = new UnresolvedMod;
            mod->m_id = json["id"];
            this->m_unresolvedMods.push_back(mod);
            if (json.contains("dependencies")) {

            }
        } break;
    }
    return Err<>(
        "\"" + path + "\" has a version schema (" +
        std::to_string(schema) + ") that isn't "
        "supported by this version of lilac. "
        "This may be a bug, or the given version "
        "schema is invalid."
    );
}

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

Result<bool> Loader::checkMetaInformation(std::string const& path) {
    auto unzip = Unzipper(path);
    lilac::byte_array bytes {};
    if (!unzip.extractEntryToMemory("mod.json", bytes)) {
        return Err<>(
            "\"" + path + "\" is missing mod.json "
            "(or is not a zip file at all)"
        );
    }
    auto rawJson = nlohmann::json(bytes);
    if (!rawJson.is_object()) {
        return Err<>(
            "\"" + path + "/mod.json\" does not have an "
            "object at root despite expected"
        );
    }
    auto json = nlohmann::json::object(rawJson);
    auto schema = 1;
    if (json.contains("schema") && json["schema"].is_number_integer()) {
        schema = json["schema"];
    }
    if (schema > Loader::s_supportedSchemaMax) {
        return Err<>(
            "\"" + path + "\" has a higher version schema (" + 
            std::to_string(schema) + ") than this version of "
            "lilac supports (" + std::to_string(Loader::s_supportedSchemaMax) +
            "). You may need to update lilac in order to use "
            "this mod."
        );
    }
    switch (schema) {
        case 1: {


            return Ok<>(true);
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

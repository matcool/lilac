#pragma once

#include "macros.hpp"
#include "types.hpp"
#include <string_view>
#include <vector>
#include <string>

class Lilac;

namespace lilac {
    #pragma warning(disable: 4251)

    static constexpr const std::string_view lilac_directory          = "lilac";
    static constexpr const std::string_view lilac_mod_directory      = "mods";
    static constexpr const std::string_view lilac_resource_directory = "resources";
    static constexpr const std::string_view lilac_mod_extension      = LILAC_WINDOWS(".dll")
                                                                       LILAC_MACOS(".dylib")
                                                                       LILAC_ANDROID(".so");

    class Mod;
    class Hook;
    class LogStream;
    class LogMessage;

    class LILAC_DLL Loader {
        protected:
            std::vector<Mod*> m_mods;
            std::vector<LogMessage*> m_logs;
            LogStream* m_logStream;
            bool m_isSetup = false;

            Loader();
            virtual ~Loader();
            
            Result<> loadModFromFile(std::string const& file);
            Result<bool> checkDependencies(std::string const& file);
            void createDirectories();

            friend class Mod;
            friend class CustomLoader;
            friend class Lilac;
            
        public:
            static Loader* get();
            bool setup();
            size_t updateMods();

            LogStream& logStream();
            void log(LogMessage* log);
            void deleteLog(LogMessage* log);
            std::vector<LogMessage*> const& getLogs() const;
            std::vector<LogMessage*> getLogs(
                std::initializer_list<Severity> severityFilter
            );
    
            bool isModLoaded(std::string_view const& id);
            Mod* getLoadedMod(std::string_view const& id);
            std::vector<Mod*> getLoadedMods();
            void unloadMod(Mod* mod);
            bool isCustomLoaderLoaded(std::string_view const& id);
    };

}

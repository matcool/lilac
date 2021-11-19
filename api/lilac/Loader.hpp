#pragma once

#include "Macros.hpp"


namespace lilac {
    static constexpr const std::string_view lilac_directory          = "lilac";
    static constexpr const std::string_view lilac_mod_directory      = "mods";
    static constexpr const std::string_view lilac_resource_directory = "resources";
    static constexpr const std::string_view lilac_mod_extension      = LILAC_WIN32(".dll")
                                                                       LILAC_MACOS(".dylib")
                                                                       LILAC_ANDROID(".so");

    class Mod;
    class Hook;

    class LILAC_DLL Loader {
        protected:
            std::vector<Mod*> m_loadedMods;
            // std::vector<LogMessage*> m_vLogs;
            // BGDLogStream* m_pLogStream;
            bool m_isSetup = false;

            Loader();
            virtual ~Loader();

            void createDirectories();
            bool loadModFromFile(std::string const& file);
            
        public:
            static Loader* get();
            bool setup();
            size_t updateMods();

            void saveData();
            void loadData();

            // inline BGDLogStream& logStream() {
            //     return *this->m_pLogStream;
            // }

            // void log(BGDLogMessage* log);
            // void deleteLog(BGDLogMessage* log);
            // std::vector<BGDLogMessage*> const& getLogs() const;
            // std::vector<BGDLogMessage*> getLogs(
                // std::initializer_list<BGDLogType>  typeFilter,
                // std::initializer_list<BGDSeverity> severityFilter = {}
            // );
    
            bool isModLoaded(std::string_view const& id);
            Mod* getLoadedMod(std::string_view const& id);
            std::vector<Mod*> getLoadedMods();
    };
}

#pragma once

#include "Hook.hpp"
#include <string_view>
#include <vector>

namespace lilac {
    #pragma warning(disable: 4251) // i will use unordered_map and
                                   // no amount of compiler warnings
                                   // can stop me >:)
                                   
    struct PlatformInfo;

    class LILAC_DLL ModBase {
        protected:
            std::string_view m_sPath;
            PlatformInfo* m_pInfo;
            std::vector<Hook*> m_vHooks;

            Result<Hook*> addHookBase(
                void* addr,
                void* detour,
                Hook* hook = nullptr
            );
            Result<Hook*> addHookBase(Hook* hook);
    };

    class LILAC_DLL Mod : ModBase {
        protected:
            std::string_view m_sID;
            std::string_view m_sName;
            std::string_view m_sDeveloper;
            std::string_view m_sDescription;
            std::string_view m_sCredits;

            void platformCleanup();

            virtual void setup() = 0;
            virtual void enable();
            virtual void disable();
            virtual void saveData();
            virtual void loadData();

            Result<Hook*> addHookInternal(
                void* addr,
                void* detour,
                void** trampoline
            );

        public:
            std::string_view getID()         const;
            std::string_view getName()       const;
            std::string_view getDeveloper()  const;
            std::string_view getCredits()    const;
            std::string_view getPath()       const;
            std::string_view getDescription()const;

            Mod();
            virtual ~Mod();

            std::vector<Hook*> getHooks() const;

            Result<> enableHook(Hook* hook);
            Result<> disableHook(Hook* hook);
            Result<> removeHook(Hook* hook);
    };
}

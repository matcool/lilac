#pragma once

#include <Log.hpp>
#include <vector>

USE_LILAC_NAMESPACE();

/**
 * For internal state management.
 * @class Lilac
 */
class Lilac {
    protected:
        std::vector<LogMessage*> m_logQueue;
        bool m_platformConsoleReady = false;

        Lilac();

    public:
        static Lilac* get();
        ~Lilac();

        bool setup();

        bool loadHooks();

        bool platformConsoleReady() const;
        void queueConsoleMessage(LogMessage*);
        void setupPlatformConsole();
        void awaitPlatformConsole();
        void closePlatformConsole();
        void platformMessageBox(const char* title, const char* info);
};

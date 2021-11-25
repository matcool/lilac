#pragma once

/**
 * For internal state management.
 * @class Lilac
 */
class Lilac {
    protected:
        Lilac();

    public:
        static Lilac* get();

        bool setup();

        bool loadHooks();

        void setupPlatformConsole();
        void awaitPlatformConsole();
        void closePlatformConsole();
};

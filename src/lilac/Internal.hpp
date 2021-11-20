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

        void setupPlatformConsole();
        void awaitPlatformConsole();
        void closePlatformConsole();
};

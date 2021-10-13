#include <lilac.hpp>

void lilac_main(lilac::Mod* mod) {
    lilac::log("hi");
    lilac::self()->on(lilac::CallbackEvent::Load, [&]() {
        lilac::log("hello");
    });
}
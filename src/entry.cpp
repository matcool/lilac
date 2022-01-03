#include "entry.hpp"

int lilacEntry() {
	// setup internals
	if (!Lilac::get()->setup()) {
		// if we've made it here, Lilac will
		// be gettable (otherwise the call to
		// setup would've immediately crashed)
		Lilac::get()->platformMessageBox(
			"Unable to Load Lilac!",
			"There was an unknown fatal error setting up "
			"internal tools and Lilac could not be loaded."
		);
		return 1;
	}

	InternalMod::get()->log()
		<< Severity::Debug
		<< "Loaded internal Lilac class"
		<< lilac::endl;

	// set up loader, load mods, etc.
	if (!Loader::get()->setup()) {
		Lilac::get()->platformMessageBox(
			"Unable to Load Lilac!",
			"There was an unknown fatal error setting up "
			"the loader and Lilac could not be loaded."
		);
		delete Lilac::get();
		return 1;
	}

	InternalMod::get()->log()
		<< Severity::Debug
		<< "Set up loader"
		<< lilac::endl;

	// debugging console
#ifdef LILAC_PLATFORM_CONSOLE
	InternalMod::get()->log()
		<< Severity::Debug
		<< "Loading Console..."
		<< lilac::endl;

	Lilac::get()->setupPlatformConsole();
	Lilac::get()->awaitPlatformConsole();
	Lilac::get()->closePlatformConsole();

	InternalMod::get()->log()
		<< Severity::Debug
		<< "Cleaning up..."
		<< lilac::endl;

	delete Lilac::get();
#endif

	return 0;
}

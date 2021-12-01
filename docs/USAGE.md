# Usage

Lilac provides a comprehensive modding API that eases mod development by utilizing concepts such as hook ownership. What that means is that each mod "owns" their hooks, keybinds, etc., so if the mod is disabled by the end user at runtime, lilac can automatically disable all keybinds and hooks owned by that mod. This means that for most mods, you will not need to write code for disabling, enabling or unloading the mod; lilac will take care of that for you. All you have to clean up is, for example, any custom manager classes you've made, but doing so should be very simple with a well-designed codebase.

In order to succesfully do hook ownership, all lilac mods start in the `lilac_main` function, which returns a pointer to a `Mod` interface. The base `Mod` class is an abstract class; meaning that in order to use it, you need to inherit from it with your own class. This may seem like a burden for some, but the benefit of a system like this is that sharing utilities in your mod with other mods is as easy as putting your Mod interface definition in a header and marking it as `dllexport`. Each Mod should also come with its own `get` method that just returns a pointer to the shared instance, so other mods and lilac macros can easily get the shared instance in their code.

## Your Mod Interface

Creating your own Mod interface is as simple as inheriting from `Mod` and defining 2 member functions: an override of the base `setup` and a static `get` method.

```cpp
class MyMod : public Mod {
protected:
    void setup() override {
        this->m_name       = "My Mod";                 // Human-readable name
        this->m_id         = "com.my_name.my_mod";     // Unique ID
        this->m_developer  = "Foobar";                 // Developer name
        this->m_description= "My Awesome Mod";         // Short description
        this->m_details    = "This is my awesome mod!";// Long description
        this->m_credits    = "Mom & dog";              // Credits
        this->m_version    = { 1, 0, 0 };              // Version
    }

public:
    static MyMod* get() {
        // Static initialization is really useful 
        // for writing getters for shared instances; 
        // this first line of code will only be 
        // run once when the function is first 
        // called.

        // Do note though that this does introduce 
        // runtime overhead; if you are extremely 
        // serious about performance, you may 
        // replace this with a global variable 
        // and initialize the instance before 
        // returning from lilac::main.
        static auto g_mod = new MyMod;
        return g_mod;
    }
};
```

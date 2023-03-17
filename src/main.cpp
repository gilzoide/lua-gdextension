#include <godot_cpp/godot.hpp>

using namespace godot;

extern "C" GDExtensionBool luagdextension_entrypoint(
    const GDExtensionInterface *p_interface,
    GDExtensionClassLibraryPtr p_library,
    GDExtensionInitialization *r_initialization
) {
    GDExtensionBinding::InitObject init_obj(p_interface, p_library, r_initialization);

	return init_obj.init();
}

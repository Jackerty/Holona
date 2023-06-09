#include<core/defs.hpp>
#include<core/class_db.hpp>
#include<gdextension_interface.h>
#include<variant/utility_functions.hpp>
#include"RootNode.hpp"
#include"MovementController.hpp"

#ifdef GD_EXTENSION
	#include<godot.hpp>
	using namespace godot;
#endif

/*******************************************
* Initialize per level.                    *
*******************************************/
void initialize_Holona_module(ModuleInitializationLevel p_level){
	// Guard against everything else than scene level initialization.
	if(p_level!=godot::MODULE_INITIALIZATION_LEVEL_SCENE) return;
	ClassDB::register_class<RootNode>();
	ClassDB::register_class<MovementController>();
}
/*******************************************
* Uninitialized per level.                 *
*******************************************/
void uninitialize_Holona_module(godot::ModuleInitializationLevel p_level){
	;
}

extern "C" {

/*******************************************
* Godot's entry to the GDExtension.        *
* Registers initialization functions and   *
* initialization level.                    *
*******************************************/
#ifdef GD_EXTENSION
GDE_EXPORT GDExtensionBool godot_init(const GDExtensionInterfaceGetProcAddress p_interface, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization){
	godot::GDExtensionBinding::InitObject init_obj(p_interface,p_library,r_initialization);
	init_obj.register_initializer(initialize_Holona_module);
	init_obj.register_terminator(uninitialize_Holona_module);
	// Set the lowest level initialization needed for the extension.
	// This means the
	init_obj.set_minimum_library_initialization_level(godot::MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
#endif

}

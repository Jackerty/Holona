/************************************************
* Additional functions for interfacing with     *
* Godot common for multiple modules.            *
************************************************/
#ifndef _ETC_GODOT_INTERFACE_HPP_
#define _ETC_GODOT_INTERFACE_HPP_

#include<classes/engine.hpp>

/***************************************************
* Macro for checking is for editor vs engine call. *
***************************************************/
#ifdef TOOLS_ENABLED
	#define IS_EDITOR_MODE Engine::get_singleton()->is_editor_hint()
#else
	#define IS_EDITOR_MODE false
#endif

#endif /* _ETC_GODOT_INTERFACE_HPP_ */

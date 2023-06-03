/*******************
* See RootNode.hpp *
*******************/
#include<classes/node.hpp>
#include<classes/input.hpp>
#include<classes/input_event.hpp>
#include<classes/input_event_mouse_button.hpp>
#include<classes/scene_tree.hpp>
#include<variant/utility_functions.hpp>
#include"EtcGodotInterface.hpp"
#include"RootNode.hpp"

#ifdef GD_EXTENSION
	using namespace godot;
#endif

/*******************
* See RootNode.hpp *
*******************/
void RootNode::_bind_methods(){
	ClassDB::bind_method(D_METHOD("get_fast_close"),&RootNode::getFastClose);
	ClassDB::bind_method(D_METHOD("set_fast_close","fastclose"),&RootNode::setFastClose);
	ClassDB::add_property("RootNode",PropertyInfo(Variant::BOOL,"fast_close"),"set_fast_close","get_fast_close");
}
/*******************
* See RootNode.hpp *
*******************/
RootNode::RootNode(){
	;
}
/*******************
* See RootNode.hpp *
*******************/
void RootNode::_ready(){
	if(!IS_EDITOR_MODE){
		// Capture the mouse.
		Input::get_singleton()->set_mouse_mode(Input::MOUSE_MODE_CAPTURED);
		// Have to set to get _input and _unhandled_input be called.
		// Processing input from this class depends upon if quick exiting
		// is to be allowed.
		this->set_process_input(this->fast_close);
		this->set_process_unhandled_input(true);
	}
}
/*******************
* See RootNode.hpp *
*******************/
void RootNode::_input(const Ref<InputEvent> &event){
	if(event->is_action_pressed("ui_cancel")) this->get_tree()->quit();
	if(event->is_action_pressed("change_mouse_input")){
		if(Input::get_singleton()->get_mouse_mode()==Input::MOUSE_MODE_CAPTURED){
			Input::get_singleton()->set_mouse_mode(Input::MOUSE_MODE_VISIBLE);
		}
		else{
			Input::get_singleton()->set_mouse_mode(Input::MOUSE_MODE_CAPTURED);
		}
	}
}
/*******************
* See RootNode.hpp *
*******************/
void RootNode::_unhandled_input(const Ref<InputEvent> &event){
	Ref<InputEventMouseButton> eventmouse = event;
	// If left click is unhandled capture the mouse.
	// There was some note about HTML5 needing this.
	if(eventmouse.is_valid() && eventmouse->get_button_index() == MOUSE_BUTTON_LEFT && eventmouse->is_pressed()){
		Input::get_singleton()->set_mouse_mode(Input::MOUSE_MODE_CAPTURED);
	}
}
/*******************
* See RootNode.hpp *
*******************/
void RootNode::setFastClose(const bool fastclose){
	this->fast_close=fastclose;
}
/*******************
* See RootNode.hpp *
*******************/
bool RootNode::getFastClose() const{
	return this->fast_close;
}

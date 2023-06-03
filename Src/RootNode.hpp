/********************************************************
* Root node for the project. Handles scene transitions. *
********************************************************/
#ifndef ROOTNODE_H
#define ROOTNODE_H
#include<classes/object.hpp>
#include<classes/input_event.hpp>
#include<classes/node.hpp>

#ifdef GD_EXTENSION
	using namespace godot;
#endif

class RootNode : public Node{
	// Macro adds some Godot requirements.
	// Must be first thing defined in the
	// class and private.
	GDCLASS(RootNode, Node);
	public:
	/*****************************************
	* Boolean to indicate if fast closing is *
	* enabled.                               *
	*****************************************/
	bool fast_close = true;
	/*****************************************
	* Godot calls this to bind methods and   *
	* properties of this object to Godot     *
	* scripting engine.                      *
	*****************************************/
	static void _bind_methods();
	/*****************************************
	* Constructor initializes the objects    *
	* members.                               *
	*****************************************/
	RootNode();
	/*****************************************
	* General notification handler for the   *
	* event pump.                            *
	* NOTE: override input function family   *
	*       for input related things.        *
	*****************************************/
	//void _notification(int p_notification);
	/*****************************************
	* Second initialization wave when node   *
	* is loaded to tree.                     *
	*****************************************/
	void _ready() override;
	/*****************************************
	* Input callback handling.               *
	*****************************************/
	void _input(const Ref<InputEvent> & event) override;
	/*****************************************
	* If Input does not handle event it is   *
	* given to unhandled. Godot manual       *
	* recommends game-play is done in it.    *
	*****************************************/
	void _unhandled_input(const Ref<InputEvent> & event) override;
	/*****************************************
	* Set and get for fast close property.   *
	*****************************************/
	void setFastClose(const bool fastclose);
	bool getFastClose() const;
};
#endif // ROOTNODE_H

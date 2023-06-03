/*****************************
* See MovementController.hpp *
*****************************/
#include<core/object.hpp>
#include<classes/character_body3d.hpp>
#include<classes/project_settings.hpp>
#include<classes/input.hpp>
#include<variant/utility_functions.hpp>
#include<classes/input_event.hpp>
#include<classes/input_event_mouse_motion.hpp>
#include<classes/camera3d.hpp>
#include<core/math.hpp>
#include"EtcGodotInterface.hpp"
#include"MovementController.hpp"

#ifdef GD_EXTENSION
	using namespace godot;
#endif

/*****************************
* See MovementController.hpp *
*****************************/
void MovementController::_bind_methods(){
	// Add properties. Properties need methods to set and get in the Godot class database.
	// Please read MovementController.hpp for the descriptions.

	// Property: gravity_multiplier
	ClassDB::bind_method(D_METHOD("setGravityMultiplier","fvalue"),&MovementController::setGravityMultiplier);
	ClassDB::bind_method(D_METHOD("getGravityMultiplier"),&MovementController::getGravityMultiplier);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT,"gravity_multiplier"),"setGravityMultiplier","getGravityMultiplier");

	// Property: normal_speed
	ClassDB::bind_method(D_METHOD("setNormalSpeed","fvalue"),&MovementController::setNormalSpeed);
	ClassDB::bind_method(D_METHOD("getNormalSpeed"),&MovementController::getNormalSpeed);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT,"normal_speed"),"setNormalSpeed","getNormalSpeed");

	// Property: sprint_speed
	ClassDB::bind_method(D_METHOD("setSprintSpeed","fvalue"),&MovementController::setSprintSpeed);
	ClassDB::bind_method(D_METHOD("getSprintSpeed"),&MovementController::getSprintSpeed);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT,"sprint_speed"),"setSprintSpeed","getSprintSpeed");

	// Property: sprint_fov_multiplier
	ClassDB::bind_method(D_METHOD("setSprintFovMultiplier","fvalue"),&MovementController::setSprintFovMultiplier);
	ClassDB::bind_method(D_METHOD("getSprintFovMiltiplier"),&MovementController::getSprintFovMultiplier);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT,"sprint_fov_multiplier"),"setSprintFovMultiplier","getSprintFovMiltiplier");

	// Property: acceleration
	ClassDB::bind_method(D_METHOD("setUpceleration","fvalue"),&MovementController::setUpceleration);
	ClassDB::bind_method(D_METHOD("getUpceleration"),&MovementController::getUpceleration);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT,"upceleration"),"setUpceleration","getUpceleration");

	// Property: deceleration
	ClassDB::bind_method(D_METHOD("setDeceleration","fvalue"),&MovementController::setDeceleration);
	ClassDB::bind_method(D_METHOD("getDeceleration"),&MovementController::getDeceleration);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT,"deceleration"),"setDeceleration","getDeceleration");

	// Property: air_control
	ClassDB::bind_method(D_METHOD("setAirControl","fvalue"),&MovementController::setAirControl);
	ClassDB::bind_method(D_METHOD("getAirControl"),&MovementController::getAirControl);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT,"air_control",PROPERTY_HINT_RANGE,"0.0,1.0,0.05"),"setAirControl","getAirControl");

	// Property: jump_height
	ClassDB::bind_method(D_METHOD("setJumpHeight","fvalue"),&MovementController::setJumpHeight);
	ClassDB::bind_method(D_METHOD("getJumpHeight"),&MovementController::getJumpHeight);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT,"jump_height"),"setJumpHeight","getJumpHeight");

	// Property: mouse_sensitivity
	ClassDB::bind_method(D_METHOD("setMouseSensitivity","fvalue"),&MovementController::setMouseSensitivity);
	ClassDB::bind_method(D_METHOD("getMouseSensitivity"),&MovementController::getMouseSensitivity);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT,"mouse_sensitivity"),"setMouseSensitivity","getMouseSensitivity");

	// Property:
	ClassDB::bind_method(D_METHOD("setYLimit","fvalue"),&MovementController::setYLimit);
	ClassDB::bind_method(D_METHOD("getYLimit"),&MovementController::getYLimit);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT,"y_limit"),"setYLimit","getYLimit");

}
/*****************************
* See MovementController.hpp *
*****************************/
MovementController::MovementController(){
	this->gravity=ProjectSettings::get_singleton()->get_setting("physics/3d/default_gravity",nullptr);
	this->gravity*=this->gravity_multiplier;
}
/*****************************
* See MovementController.hpp *
*****************************/
void MovementController::_input(const Ref<godot::InputEvent>& event){
	// Handle mouse motion if capture mode is true.
	// Mouse is captures here the rotateFaceCamera
	// function handles actual rotation.
	const Ref<InputEventMouseMotion> mousemotion=event;
	if(mousemotion.is_valid() && Input::get_singleton()->get_mouse_mode()==Input::MOUSE_MODE_CAPTURED){
		mouse_axis=mousemotion.ptr()->get_relative();
		this->rotateFaceCamera();
	}
}
/*****************************
* See MovementController.hpp *
*****************************/
void MovementController::_physics_process(double delta){

	// Handle camera rotation on joystick.
	// Sub function handles actual rotating.
	Vector2 joystickaxis=Input::get_singleton()->get_vector("look_left","look_right","look_down","look_up");
	if(!joystickaxis.is_zero_approx()){
		this->mouse_axis=joystickaxis*1000.0*delta;
		this->rotateFaceCamera();
	}

	// Get direction controller or keyboard is pressing.
	// Returned vector magnitude is at max 1 if pressing
	// is strongest. Zero vector if no pressing.
	this->input_axis=Input::get_singleton()->get_vector("move_forward","move_back","move_left","move_right");

	// Calculate direction of the movement.
	//
	// Basis aim is basis of the global transform (0.0 - 1.0 matrix of local coordinate a.k.a rotation):
	//  ┌ ┌     ┐ ┌     ┐ ┌     ┐ ┐   ┌        ┐
	//  │ │ x_0 │ │ y_0 │ │ z_0 │ │   │ aim[0] |   ┌                                                       ┐
	//  │ │ x_1 │ │ y_1 │ │ z_1 │ │ = │ aim[1] │ = │ aim.get_column(0) aim.get_column(1) aim.get_column(2) │
	//  │ │ x_2 │ │ y_2 │ │ z_2 │ │   │ aim[2] │   └                                                       ┘
	//  └ └     ┘ └     ┘ └     ┘ ┘   └        ┘
	//
	// Hence, to calculate movement direction query current local rotation and based upon that
	// calculate vector along xz-plane. So (if input_axis vector members are i_x, i_y, and i_z):
	//
	//                                               ┌                     ┐
	//                                               │  z_0·i_x + x_0·i_y  │
	// aim.get_column(2)·i_x+aim.get_column(0)·i_y = │  z_1·i_x + x_1·i_y  │
	//                                               │  z_2·i_x + x_3·i_y  │
	//                                               └                     ┘
	//
	// NOTE: Would better movement system calculate tangent plane at the floor to add y direction.
	Basis aim=this->get_global_transform().basis;
	this->direction=aim.get_column(2)*((this->input_axis.x))+aim.get_column(0)*(this->input_axis.y);

	// Get current velocity. By default assume player is on the floor
	// and gravity later.
	Vector3 newvelocity=this->get_velocity();
	float oldvelocityy=newvelocity.y;
	newvelocity.y=0;

	// Since there is velocity setup along xz-plane use that fact conjunction with direction
	// to create move delta amount of speed to that direction.
	//
	// Amount of acceleration applied. This depends upon current velocity in sense that
	// if new direction more opposite then decelerate.
	// NOTE: Should dot product be calculate faster via y is zero?
	float tempaccel=0;
	if(this->direction.dot(newvelocity)>0) tempaccel=this->upceleration;
	else tempaccel=this->deceleration;

	// Where player would be after full second in current
	// full speed.
	Vector3 target=this->direction*this->speed;

	if(!this->is_on_floor()) tempaccel*=this->air_control;
	newvelocity=newvelocity.lerp(target,tempaccel*delta);

	// Handling jumping movement. If player is on the floor then check
	// jumping button has not been pressed.
	//
	// Other movement feature handled is sprinting button check.
	if(this->is_on_floor()){
		if(Input::get_singleton()->is_action_just_pressed("jump")){
			// Set velocity takes in a vector input. So use get velocity to
			// get the pointer (as a reference) to the same vector.
			//
			// Dot y does not allow setting but array index version does.
			newvelocity.y=this->jump_acceleration;
		}

		if(Input::get_singleton()->is_action_pressed("sprint") && this->input_axis.x <= -0.5){
			// Player is sprinting so set speed to sprinting and change FOV for effect.
			this->speed=sprint_speed;
			// Lerp is used to smooth over the change.
			this->face_camera->set_fov(Math::lerp(this->face_camera->get_fov(),this->normal_fov*this->sprint_fov_multiplier,delta*8));
		}
		else{
			this->speed=normal_speed;
			// Lerp is used to smooth over the change.
			this->face_camera->set_fov(Math::lerp(this->face_camera->get_fov(),this->normal_fov,delta*8));
		}
	}
	else{
		// Apply gravity since we are not on the floor.
		// Use old velocity in y direction since otherwise
		// gravity applies in constant velocity not in
		// constant acceleration.
		newvelocity.y=oldvelocityy-this->gravity*delta;
	}

	// Set new velocity vector and update.
	this->set_velocity(newvelocity);
	this->move_and_slide();
	this->update_gizmos();

}
/*****************************
* See MovementController.hpp *
*****************************/
void MovementController::_ready(){

	// Set input and physics processing when not in the editor to avoid errors.
	this->set_process_input(!IS_EDITOR_MODE);
	this->set_physics_process(!IS_EDITOR_MODE);

	// Face camera is the Camera3D child node.
	// For speed up in the release get first child.
	// Have warning about this in editor more.
	#ifdef RELEASE
		this->face_camera=(Camera3D*)(this->get_child(0,false));
	#else
		this->face_camera=this->get_node<Camera3D>(NodePath("Camera"));

		// Warn that if release is defined face camera has to be child member.
		if(IS_EDITOR_MODE){
			if(!this->face_camera){
				UtilityFunctions::push_warning("MovementController needs a \"Camera\"!");
				return;
			}
			else if(this->face_camera!=this->get_child(0,false)){
				UtilityFunctions::push_warning("In release compilation \"Camera\" has to be zero index (most top) child.\n"
				                               "\"Camera\" is at index ",this->face_camera->get_index(false),".");
			}
		}
	#endif

	// Get the normal camera FOV to reset to when sprinting ends.
	this->normal_fov=this->face_camera->get_fov();
}
/*****************************
* See MovementController.hpp *
*****************************/
void MovementController::rotateFaceCamera(){

	// Vertical look.
	this->roty-=this->mouse_axis.x*this->mouse_sensitivity;
	// Horizontal look. Apply y_limit here.
	this->rotx=Math::clamp(this->rotx-this->mouse_axis.y*this->mouse_sensitivity,-this->y_limit,this->y_limit);

	// Rotate whole character horizontally.
	Vector3 protation=this->get_rotation();
	protation.y=this->roty;
	this->set_rotation(protation);

	// Rotate only head and camera vertically to avoid moving collision boxes and the model unnecessarily.
	Vector3 crotation=this->face_camera->get_rotation();
	crotation.x=this->rotx;
	this->face_camera->set_rotation(crotation);
}
/*****************************
* See MovementController.hpp *
*****************************/
void MovementController::setGravityMultiplier(float value){
	this->gravity_multiplier=value;
}
/*****************************
* See MovementController.hpp *
*****************************/
float MovementController::getGravityMultiplier(){
	return this->gravity_multiplier;
}
/*****************************
* See MovementController.hpp *
*****************************/
void MovementController::setNormalSpeed(float value){
	this->normal_speed=value;
	this->speed=value;
}
/*****************************
* See MovementController.hpp *
*****************************/
float MovementController::getNormalSpeed(){
	return this->normal_speed;
}
/*****************************
* See MovementController.hpp *
*****************************/
void MovementController::setSprintSpeed(float value){
	this->sprint_speed=value;
	this->speed=value;
}
/*****************************
* See MovementController.hpp *
*****************************/
float MovementController::getSprintSpeed(){
	return this->sprint_speed;
}
/*****************************
* See MovementController.hpp *
*****************************/
void MovementController::setSprintFovMultiplier(float value){
	this->sprint_fov_multiplier=value;
}
/*****************************
* See MovementController.hpp *
*****************************/
float MovementController::getSprintFovMultiplier(){
	return this->sprint_fov_multiplier;
}
/*****************************
* See MovementController.hpp *
*****************************/
void MovementController::setUpceleration(float value){
	this->upceleration=value;
}
/*****************************
* See MovementController.hpp *
*****************************/
float MovementController::getUpceleration(){
	return this->upceleration;
}
/*****************************
* See MovementController.hpp *
*****************************/
void MovementController::setDeceleration(float value){
	this->deceleration=value;
}
/*****************************
* See MovementController.hpp *
*****************************/
float MovementController::getDeceleration(){
	return this->deceleration;
}
/*****************************
* See MovementController.hpp *
*****************************/
void MovementController::setAirControl(float value){
	this->air_control=value;
}
/*****************************
* See MovementController.hpp *
*****************************/
float MovementController::getAirControl(){
	return this->air_control;
}
/*****************************
* See MovementController.hpp *
*****************************/
void MovementController::setJumpHeight(float value){
	this->jump_acceleration=value;
}
/*****************************
* See MovementController.hpp *
*****************************/
float MovementController::getJumpHeight(){
	return this->jump_acceleration;
}
/*****************************
* See MovementController.hpp *
*****************************/
void MovementController::setMouseSensitivity(float value){
	this->mouse_sensitivity=value;
}
/*****************************
* See MovementController.hpp *
*****************************/
float MovementController::getMouseSensitivity(){
	return this->mouse_sensitivity;
}
/*****************************
* See MovementController.hpp *
*****************************/
void MovementController::setYLimit(float value){
	this->y_limit=value;
}
/*****************************
* See MovementController.hpp *
*****************************/
float MovementController::getYLimit(){
	return this->y_limit;
}

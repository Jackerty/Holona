/*******************************************
 * Node to create mesh and movement of the player.
*******************************************/
#ifndef MOVEMENTCONTROLLER_H
#define MOVEMENTCONTROLLER_H

#include<classes/character_body3d.hpp>
#include<variant/vector3.hpp>
#include<classes/camera3d.hpp>
#include<math.h>

#ifdef GD_EXTENSION
	using namespace godot;
#endif

class MovementController : public CharacterBody3D{
	// Macro adds some Godot requirements.
	// Must be first thing defined in the
	// class and private.
	GDCLASS(MovementController, CharacterBody3D);
	public:
	/*****************************************
	* Node specific effect coefficient for   *
	* gravity. (In some sense this is the    *
	* effect of the air resistance.)         *
	*****************************************/
	float gravity_multiplier=3.0f;
	float gravity;
	/*****************************************
	* Speed is the maximum amount player     *
	* moves. Sprint speed is the value used  *
	* when sprinting. Normal speed is used   *
	* otherwise. Speed variable is the one   *
	* used in the calculations.              *
	*****************************************/
	float normal_speed=10.0f;
	float sprint_speed=16.0f;
	float speed;
	/*****************************************
	* Amount of positive acceleration.       *
	*****************************************/
	float upceleration=8.0f;
	/*****************************************
	* Amount of negative acceleration.       *
	*****************************************/
	float deceleration=10.0f;
	/*****************************************
	* Amount of movement speed is reduced    *
	* while in air. Ranges between 0.0 to    *
	* 1.0 and is allowed to be incremented   *
	* by 0.05 in the editor increments.      *
	*****************************************/
	float air_control=0.3f;
	/*****************************************
	* Acceleration applied when jumping.     *
	*****************************************/
	float jump_acceleration=10.0f;
	/*****************************************
	* When sprinting change FOV by this      *
	* coefficient.                           *
	*****************************************/
	double sprint_fov_multiplier=1.05;
	/*****************************************
	* Normal FOV value.                      *
	*                                        *
	* TODO: Camera should have feature to    *
	*       toggle FOV multiplier. Hence     *
	*       this variable is removed here by *
	*       that. Now changing FOV in        *
	*       settings has to update this      *
	*       value as well.                   *
	*****************************************/
	double normal_fov;
	/*****************************************
	* Sensitivity for mouse control.         *
	*****************************************/
	double mouse_sensitivity=2.0/1000;
	/*****************************************
	* Limit on vertical aiming movement.     *
	* Prevents upside down rotation.         *
	*****************************************/
	double y_limit=M_PI/2;
	/*****************************************
	* Current movement direction.            *
	*****************************************/
	Vector3 direction;
	/*****************************************
	* Memory to storage controller/keyboard  *
	* movement every frame.                  *
	*****************************************/
	Vector2 input_axis;
	/*****************************************
	* Memory to storage mouse/camera         *
	* movement for every frame.              *
	*****************************************/
	Vector2 mouse_axis;
	/*****************************************
	* Pointer to player face camera.         *
	*****************************************/
	Camera3D *face_camera;
	/*****************************************
	* Store vertical and horizontal face     *
	* rotation.                              *
	*****************************************/
	double rotx=0.0,roty=0.0;

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
	MovementController();
	/*****************************************
	* Callback for NOTIFICATION_READY.       *
	* Second wave of the initialization.     *
	*****************************************/
	virtual void _ready() override;
	/*****************************************
	* Regular input processing.              *
	*                                        *
	*****************************************/
	virtual void _input(const Ref<godot::InputEvent> &event) override;
	/*****************************************
	* This method rotates the face camera.   *
	*****************************************/
	void rotateFaceCamera();
	/*****************************************
	* Physics processing callback for        *
	* NOTIFICATION_PHYSICS_PROCESS.          *
	*                                        *
	* Handles player input.                  *
	*****************************************/
	virtual void _physics_process(double delta) override;
	/*****************************************
	* Set/get gravity_multiplier.            *
	*****************************************/
	void setGravityMultiplier(float);
	float getGravityMultiplier();
	/*****************************************
	* Set/get normal speed.                  *
	*****************************************/
	void setNormalSpeed(float);
	float getNormalSpeed();
	/*****************************************
	* Set/get sprinting speed.               *
	*****************************************/
	void setSprintSpeed(float);
	float getSprintSpeed();
	/*****************************************
	* Set/get FOV while sprinting.           *
	*****************************************/
	void setSprintFovMultiplier(float);
	float getSprintFovMultiplier();
	/*****************************************
	* Set/get acceleration.                  *
	*****************************************/
	void setUpceleration(float);
	float getUpceleration();
	/*****************************************
	* Set/get deceleration.                  *
	*****************************************/
	void setDeceleration(float);
	float getDeceleration();
	/*****************************************
	* Set/get air_control.                   *
	*****************************************/
	void setAirControl(float);
	float getAirControl();
	/*****************************************
	* Set/get jump_height.                   *
	*****************************************/
	void setJumpHeight(float);
	float getJumpHeight();
	/*****************************************
	* Set/get air_control.                   *
	*****************************************/
	void setMouseSensitivity(float);
	float getMouseSensitivity();
	/*****************************************
	* Set/get jump_height.                   *
	*****************************************/
	void setYLimit(float);
	float getYLimit();
};
#endif // MOVEMENTCONTROLLER_H

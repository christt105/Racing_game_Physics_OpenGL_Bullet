#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;

#define MAX_ACCELERATION 3000.0f
#define MAX_DECCELERATION -5000.0f
#define TURN_DEGREES 15.0f * DEGTORAD
#define BRAKE_POWER 1000.0f
#define CAMERA_OFFSET_X 10
#define CAMERA_OFFSET_Y 5
#define CAMERA_OFFSET_Z 10

struct Attitude {
	vec3 pos;
	vec3 rot;
};

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();
	void NitroSpeed();

public:

	PhysVehicle3D* vehicle;
	PhysVehicle3D* ghost;
	float turn;
	float acceleration;
	float brake;
	bool nitro = true;
	int checkpoint_value = 0;
private:
	vec3 direction;
	vec3 axis;
	bool accelerating = false;
	bool decelerating = false;
	p2DynArray<Attitude> ghost_pos;
	bool save_ghost_data = false;
	bool path_ghost = false;
	int iterator_ghost = 0;
	Timer timer;

	int current_time = 0;
	int start_time = 0;

};
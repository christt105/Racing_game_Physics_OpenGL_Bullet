#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"
#include "Timer.h"

#define MAX_SNAKE 2

struct PhysBody3D;
struct PhysMotor3D;

enum class ORIENTATION {
	NORTH,		//positive Z axis
	SOUTH,		//negative Z axis
	EAST,		//negative X azis
	WEST,		//positive X axis

	NORTHEAST,
	NORTHWEST,
	SOUTHEAST,
	SOUTHWEST
};

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	//void CreateRect(int x, int y, int z, int x_width, int high, int y_width);
	void CreateRect(const float &x, const float &y, const float &z, const float &width, const float &length, const Cube & cube, ORIENTATION orientation = ORIENTATION::NORTH);
	void CreateCurve(const float &x, const float &y, const float &z, const float & width, const float & angle_i, const float & angle_f, const Cube & cube, const float & radius);
	//void CreateRamp(const float & x, const float & y, const float & z, const float & inclination = 10.0f, const float& rotation = 0.0f);

	void NitroObject(vec3 pos, int size, int distance_to);
	void PickUpNitroObject(PhysBody3D* nitro_body);

	void CreateCheckpoint(vec3 pos);
	void Checkpoint(PhysBody3D* checkpoint_body);

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

	//void Angles2AxisAngle(const float& alpha_x, const float& beta_y, vec3& u, float& angle);

public:
	bool camera_free = false;
	p2DynArray <Cube*> map;
	p2DynArray <Sphere*> nitro_objects;
	p2DynArray <PhysBody3D*> nitro_objects_body;
	p2DynArray <Cube*> checkpoint_objects;
	p2DynArray <PhysBody3D*> checkpoint_objects_body;

private:
	Timer timer;
};

#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"

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
	void CreateCurve();
	void CreateRamp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

public:
	bool camera_free = false;
	p2DynArray <Cube*> map;
};

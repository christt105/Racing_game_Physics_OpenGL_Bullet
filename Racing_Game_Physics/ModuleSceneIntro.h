#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"

#define MAX_SNAKE 2

struct PhysBody3D;
struct PhysMotor3D;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	//void CreateRect(int x, int y, int z, int x_width, int high, int y_width);
	void CreateRect(const float &x, const float &y, const float &z, const float &width, const float &length, Cube &cube);
	void CreateCurve();
	void CreateRamp();

	void NitroObject(vec3 pos, int size, int distance_to);
	void PickUpNitroObject(PhysBody3D* nitro_obj);

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

public:

	p2DynArray <Cube*> map;
	p2DynArray <Sphere> nitro_objects;
	p2DynArray <PhysBody3D*> nitro_objects_body;

};

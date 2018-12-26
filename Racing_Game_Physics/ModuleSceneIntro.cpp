#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	CreateRect(15, 0, 300);
	CreateRect(55, 0, 300);
	return ret;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	//Render Map ==============================
	for (int i = 0; i < map.Count(); i++)
	{
		map[i].axis = true;
		map[i].Render();
	}

	
	return UPDATE_CONTINUE;
}


// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

void ModuleSceneIntro::CreateRect(float x, float y, float h)
{
	map_floor = { x, y, h };
	map_floor.color.Set(125, 125, 125, 0.25F);
	map.PushBack(map_floor);
}

void ModuleSceneIntro::CreateCurve()
{
}

void ModuleSceneIntro::CreateRamp()
{
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}


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
	
	// Global Floor
	map_floor = { 300, -1, 300 };
	map_floor.color.Set(125, 125, 125, 0.25F);
	map.PushBack(map_floor);

	// Rects (x, y, z, x_width, high, y_width)
	CreateRect(5, 0, 10, 10, 1, 30);
	CreateRect(15, 0, 40, 30, 1, 10);
	CreateRect(30, 0, -20, 30, 1, 10);
	CreateRect(15, 0, 0, 30, 1, 10);
	CreateRect(50, 0, 40, 10, 1, 30);
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

void ModuleSceneIntro::CreateRect(int x, int y, int z, int x_width, int high, int y_width)
{
	vec3 position = { (float)x, (float)y, (float)z };
	p2DynArray <Cube> rect = App->physics->AddRect(position, x_width, high, y_width);

	for (int i = 0; i < rect.Count(); i++)
	{
		map.PushBack(rect[i]);
	}
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


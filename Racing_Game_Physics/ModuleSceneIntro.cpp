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
	Cube* c = new Cube(300, -1, 300);
	c->color.Set(125, 125, 125, 0.25F);
	//c->SetPos(App->physics->AddBody(*c)->GetPosition());
	map.PushBack(c);

	// Rects (x, y, z, x_width, high, y_width)
	Cube road(1, 1, 1);
	road.color.Set(0.0f, 0.0f, 1.0f);
	CreateRect(0.0f, 0.0f, 0.0f, 15, 100, road);
	/*CreateRect(5, 0, 10, 10, 1, 30);
	CreateRect(15, 0, 40, 30, 1, 10);
	CreateRect(30, 0, -20, 30, 1, 10);
	CreateRect(15, 0, 0, 30, 1, 10);
	CreateRect(50, 0, 40, 10, 1, 30);*/
	
	return ret;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	//Render Map ==============================
	for (int i = 0; i < map.Count(); i++)
	{
		map[i]->Render();
	}

	return UPDATE_CONTINUE;
}


// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	for (int i = 0; i < map.Count(); ++i)
		delete map[i];

	return true;
}

void ModuleSceneIntro::CreateRect(const float & x, const float & y, const float & z, const float & width, const float &length, Cube &cube)
{
	PhysBody3D* phys = nullptr;
	Cube* c = nullptr;
	
	for (int i = 0; i < length; ++i) {
		phys = App->physics->AddBody(cube,0.0F);
		phys->SetPos(x, y, z + cube.size.x*i);
		c = new Cube(cube);
		c->SetPos(phys->GetPosition());
		map.PushBack(c);
	}

	for (int i = 0; i < length; ++i) {
		phys = App->physics->AddBody(cube, 0.0F);
		phys->SetPos(x + width, y, z + cube.size.x*i);
		c = new Cube(cube);
		c->SetPos(phys->GetPosition());
		map.PushBack(c);
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


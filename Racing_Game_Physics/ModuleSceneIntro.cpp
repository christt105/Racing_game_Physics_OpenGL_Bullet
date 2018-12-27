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
	map.PushBack(c);

	Cube road(1, 3, 1);
	road.color.Set(0.0f, 0.0f, 1.0f);

	CreateRect(-7.0f, 0.0f, 0.0f, 15, 100, road, ORIENTATION::NORTH);
	CreateRect(-7.0f, 0, 100, 15, 100, road, ORIENTATION::EAST);
	CreateRect(-7.0f, 0.0f, 0.0f, 15, 100, road, ORIENTATION::SOUTH);
	CreateRect(-7.0f, 0, -100, 15, 100, road, ORIENTATION::WEST);
	CreateRect(-7.0f, 0.0f, 0.0f, 15, 100, road, ORIENTATION::EAST);
	CreateRect(100-7.0f, 0.0f, 0.0f, 15, 100, road, ORIENTATION::NORTH);
	CreateRect(-7.0f, 0.0f, 0.0f, 15, 100, road, ORIENTATION::WEST);
	CreateRect(-100-7.0f, 0.0f, 0.0f, 15, 100, road, ORIENTATION::SOUTH);
	
	return ret;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		camera_free = !camera_free;

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

void ModuleSceneIntro::CreateRect(const float & x, const float & y, const float & z, const float & width, const float &length, const Cube & cube, ORIENTATION orientation)
{
	PhysBody3D* phys1 = nullptr;
	PhysBody3D* phys2 = nullptr;
	Cube* c1 = nullptr;
	Cube* c2 = nullptr;
	
	for (int i = 0; i < length; ++i) {
		phys1 = App->physics->AddBody(cube, 0.0F);
		phys2 = App->physics->AddBody(cube, 0.0F);
		switch (orientation)
		{
		case ORIENTATION::NORTH:
			phys1->SetPos(x, y, z + cube.size.x*i);
			phys2->SetPos(x + width, y, z + cube.size.x*i);
			break;
		case ORIENTATION::SOUTH:
			phys1->SetPos(x, y, z - cube.size.x*i);
			phys2->SetPos(x + width, y, z - cube.size.x*i);
			break;
		case ORIENTATION::EAST:
			phys1->SetPos(x - cube.size.x*i, y, z);
			phys2->SetPos(x - cube.size.x*i, y, z + width);
			break;
		case ORIENTATION::WEST:
			phys1->SetPos(x + cube.size.x*i, y, z);
			phys2->SetPos(x + cube.size.x*i, y, z + width);
			break;
		case ORIENTATION::NORTHEAST:
			phys1->SetPos(x - cube.size.x*i, y, z + cube.size.x*i);
			phys2->SetPos(x + width - cube.size.x*i, y, z + cube.size.x*i);
			break;
		case ORIENTATION::NORTHWEST:
			phys1->SetPos(x + cube.size.x*i, y, z + cube.size.x*i);
			phys2->SetPos(x + width + cube.size.x*i, y, z + cube.size.x*i);
			break;
		case ORIENTATION::SOUTHEAST:
			phys1->SetPos(x - cube.size.x*i, y, z - cube.size.x*i);
			phys2->SetPos(x - cube.size.x*i + width, y, z - cube.size.x*i);
			break;
		case ORIENTATION::SOUTHWEST:
			phys1->SetPos(x + cube.size.x*i, y, z - cube.size.x*i);
			phys2->SetPos(x + cube.size.x*i + width, y, z - cube.size.x*i);
			break;
		default:
			break;
		}
		c1 = new Cube(cube);
		c2 = new Cube(cube);
		c1->SetPos(phys1->GetPosition());
		c2->SetPos(phys2->GetPosition());
		map.PushBack(c1);
		map.PushBack(c2);
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


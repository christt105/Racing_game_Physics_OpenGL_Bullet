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
	
	// Floor
	Cube* c = new Cube(300, -1, 300);
	c->color.Set(125, 125, 125, 0.25F);
	map.PushBack(c);

	//Map creation
	Cube road(1, 3, 1);
	road.color.Set(0.0f, 0.0f, 0.0f);

	CreateRect(-7.0f, 0.0f, 0.0f, 15, 100, road, ORIENTATION::NORTH);
	CreateRect(-7.0f, 0, 100, 15, 100, road, ORIENTATION::EAST);
	CreateRect(-7.0f, 0.0f, 0.0f, 15, 100, road, ORIENTATION::SOUTH);
	CreateRect(-7.0f, 0, -100, 15, 100, road, ORIENTATION::WEST);
	CreateRect(-7.0f, 0.0f, 0.0f, 15, 100, road, ORIENTATION::EAST);
	CreateRect(100-7.0f, 0.0f, 0.0f, 15, 100, road, ORIENTATION::NORTH);
	CreateRect(7.0f, 0.0f, 0.0f, 15, 100, road, ORIENTATION::WEST);
	CreateRect(-100-7.0f, 0.0f, 0.0f, 15, 100, road, ORIENTATION::SOUTH);
	
	// Nitro Objects
	NitroObject({ -23, 1, 30 }, 3, 20);
	
	//Checkpoint Objects
	CreateCheckpoint({0, 1, 50});
	return ret;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		camera_free = !camera_free;

	//Render Map 
	for (int i = 0; i < map.Count(); i++)
	{
		map[i]->Render();
	}

	// Nitro Scene Objects
	for (uint i = 0; i < nitro_objects.Count(); i++)
	{
		nitro_objects[i].Render();
	}

	// Checkpoints Scene Objects
	for (uint i = 0; i < checkpoint_objects.Count(); i++)
	{
		checkpoint_objects[i].Render();
	}
	return UPDATE_CONTINUE;
}


// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	for (uint i = 0; i < map.Count(); ++i)
		delete map[i];

	/*for (int i = 0; i < nitro_objects.Count(); i++)
		delete nitro_objects[i];*/

	/*for (int i = 0; i < checkpoint_objects.Count(); i++)
		delete checkpoint_objects[i];*/

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

void ModuleSceneIntro::NitroObject(vec3 pos, int size, int distance_to)
{
	for (int i = 0; i < size; i++)
	{
		Sphere nitro_obj(1);
		nitro_obj.color.Set(255, 0, 128, 1.F);
		nitro_obj.SetPos(pos.x + distance_to, pos.y, pos.z);
		nitro_objects.PushBack(nitro_obj);
		distance_to += 4;
		PhysBody3D* sensor = App->physics->AddBody(nitro_obj, 0);
		sensor->SetListener(true);
		sensor->SetState(PhysBody3D::state::NITRO);
		nitro_objects_body.PushBack(sensor);
	} 
}

void ModuleSceneIntro::PickUpNitroObject(PhysBody3D * nitro_body)
{
	for (int i = 0; i < nitro_objects_body.Count(); i++)
	{
		if (nitro_objects_body[i] == nitro_body)
		{
			nitro_objects.Pop(nitro_objects[i]);
			nitro_objects_body.Pop(nitro_objects_body[i]);
		}
	}
	App->player->nitro = true;
}

void ModuleSceneIntro::CreateCheckpoint(vec3 pos)
{
	Cube checkpoint_obj(15, 2, 2);
	checkpoint_obj.color.Set(0, 255, 0, 1.F);
	checkpoint_obj.SetPos(pos.x, pos.y, pos.z);
	checkpoint_objects.PushBack(checkpoint_obj);
	PhysBody3D* sensor2 = App->physics->AddBody(checkpoint_obj, 0);
	sensor2->SetListener(true);
	sensor2->SetState(PhysBody3D::state::CHECKPOINT);
	checkpoint_objects_body.PushBack(sensor2);
}

void ModuleSceneIntro::Checkpoint(PhysBody3D* checkpoint_body)
{
	for (uint i = 0; i < checkpoint_objects_body.Count(); i++)
	{
		if (checkpoint_objects_body[i] == checkpoint_body)
		{
			checkpoint_objects.Pop(checkpoint_objects[i]);
			checkpoint_objects_body.Pop(nitro_objects_body[i]);
		}
	}

}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	if (body1->on_collision && body2->on_collision)
	{
		if (body2->GetState() == PhysBody3D::state::NITRO)
		{
			PickUpNitroObject(body2);
			App->physics->DestroyBody(body2);
		}
		if (body2->GetState() == PhysBody3D::state::CHECKPOINT)
		{
			Checkpoint(body2);
			App->physics->DestroyBody(body2);
		}

	}
}


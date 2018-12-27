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

	App->camera->Move(vec3(1.0F, 1.0F, 0.0F));
	App->camera->LookAt(vec3(0, 0, 0));
	
	// Floor
	Cube* c = new Cube(300, -1, 300);
	c->color.Set(125, 125, 125, 0.25F);
	/*c->SetPos(App->physics->AddBody(*c)->GetPosition());*/
	map.PushBack(c);

	// Rects
	Cube road(1, 1, 1);
	road.color.Set(0.0F, 0.0F, 1.0F);
	CreateRect(0.0F, 0.0F, 0.0F, 15, 100, road);
	
	// Nitro Objects
	NitroObject({ -17, 1, 30 }, 3, 20);
	
	//Checkpoint Objects
	//CreateCheckpoint({7, 1, 50});
	return ret;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	//Render Map 
	for (uint i = 0; i < map.Count(); i++)
	{
		map[i]->Render();
	}

	// Nitro Scene Objects
	for (uint i = 0; i < nitro_objects.Count(); i++)
	{
		nitro_objects[i].Render();
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
	Cube checkpoint_obj(25, 2, 2);
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


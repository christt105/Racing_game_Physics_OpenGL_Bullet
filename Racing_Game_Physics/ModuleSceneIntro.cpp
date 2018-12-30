#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModulePlayer.h"
#include "PhysVehicle3D.h"
#include "ModuleAudio.h"

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
	Cube* c = new Cube(3000, -1, 3000);
	c->color.Set(125, 125, 125, 0.25F);
	map.PushBack(c);

	//Map creation
	Cube road(1, 15, 1);
	road.color.Set(0.0f, 0.0f, 0.0f);
	int road_width = 15;
	int radius = 20;

	CreateRect(-7.0f, 0, -25, road_width, 125, road, ORIENTATION::NORTH);
	CreateCurve(-27.0f, 0, 100, road_width, 0, 90, road, radius);
	CreateRect(-25, 0, 120, road_width, 150, road, ORIENTATION::EAST);
	CreateCurve(-175.0f, 0.0f, 100, road_width, 90, 270, road, radius);
	CreateCurve(-175.0f, 0.0f, 45, road_width, 0, 90, road, radius);
	CreateRect(-155, 0, 43, road_width, 300, road,ORIENTATION::SOUTH);
	CreateCurve(-120, 0, -257, road_width, 180, 270, road, radius);
	CreateRect(-117, 0, -292, road_width, 150, road, ORIENTATION::WEST);
	CreateCurve(33, 0, -312, road_width, 0, 90, road, radius);
	CreateRect(53, 0, -314, road_width, 150, road, ORIENTATION::SOUTH);
	CreateCurve(88, 0, -465, road_width, 180, 360, road, radius);
	CreateRect(108, 0, -462, road_width, 200, road, ORIENTATION::NORTH);
	CreateCurve(88, 0, -262, road_width, 0, 90, road, radius);
	CreateCurve(86, 0, -207, road_width, 90, 270, road, radius);
	CreateRect(88, 0, -187, road_width, 150, road, ORIENTATION::WEST);
	CreateCurve(238, 0, -152, road_width, -90, 90, road, radius);
	CreateRect(235, 0, -132, road_width, 206, road, ORIENTATION::EAST);
	CreateCurve(29, 0, -152, road_width, 90, 180, road, radius);
	CreateRect(-6, 0, -154, road_width, 30, road, ORIENTATION::SOUTH);
	CreateCurve(-26, 0, -184, road_width, 180, 360, road, radius);
	CreateRect(-61, 0, -182, road_width, 100, road, ORIENTATION::NORTH);
	CreateCurve(-26, 0, -82, road_width, 95, 180, road, radius);
	CreateCurve(-27, 0, -27, road_width, -90, 0, road, radius);

	// Nitro Objects
	NitroObject({ -23, 1, 30 }, 3, 20);
	
	//Checkpoint Objects
	CreateCheckpoint({0, 1, 50});

	// Timer
	timer.Start();

	return ret;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	current_time = SDL_GetTicks() - start_time;

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
		if(nitro_objects[i] != nullptr)
			nitro_objects[i]->Render();
	}

	// Checkpoints Scene Objects
	for (uint i = 0; i < checkpoint_objects.Count(); i++)
	{
		checkpoint_objects[i]->Render();
	}

	if(timer.Read()/1000 == 235)
		App->audio->PlayMusic("Audio/Music/60 Seconds.ogg");

	char title[80];
	sprintf_s(title, "Velocity: %.1F Km/h || Nitro: %d || Timer: %.2d:%.2d", App->player->vehicle->GetKmh(), App->player->nitro, timer.Read()/60000,timer.Read()/1000 % 60);
	App->window->SetTitle(title);
	return UPDATE_CONTINUE;
}


// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	for (uint i = 0; i < map.Count(); ++i)
		delete map[i];

	for (int i = 0; i < nitro_objects.Count(); i++)
		delete nitro_objects[i];

	for (int i = 0; i < checkpoint_objects.Count(); i++)
		delete checkpoint_objects[i];

	return true;
}

void ModuleSceneIntro::CreateRect(const float & x, const float & y, const float & z, const float & width, const float &length, const Cube & cube, ORIENTATION orientation)
{
	
	Cube* c1 = nullptr;
	Cube* c2 = nullptr;
	
	for (int i = 0; i < length; i=i+3) {
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
	
		phys1->SetState(PhysBody3D::Tag::WALL);
		phys2->SetState(PhysBody3D::Tag::WALL);

		map.PushBack(c1);
		map.PushBack(c2);
	}	
}

void ModuleSceneIntro::CreateCurve(const float & x, const float & y, const float & z, const float & width, const float & angle_i, const float & angle_f, const Cube & cube, const float & radius)
{
	Cube* c1 = nullptr;
	Cube* c2 = nullptr;
	
	for (int i = angle_i; i <= angle_f; i = i + cube.size.x * 5) {
		c1 = new Cube(cube);
		c2 = new Cube(cube);
		
		c1->SetPos(x + radius * cos(i*3.1415 / 180), y, z + radius * sin(i*3.1415 / 180));
		c2->SetPos(x + (radius + width) * cos(i*3.1415 / 180), y, z + (radius + width) * sin(i*3.1415 / 180));
		
		phys_curve1 = App->physics->AddBody(*c1, 0.0F);
		phys_curve2 = App->physics->AddBody(*c2, 0.0F);

		phys_curve1->SetState(PhysBody3D::Tag::WALL);
		phys2->SetState(PhysBody3D::Tag::WALL);

		map.PushBack(c1);
		map.PushBack(c2);
	}
	
}

//void ModuleSceneIntro::CreateRamp(const float & x, const float & y, const float & z, const float & inclination, const float & rotation)
//{
//	Cube* c = new Cube(10, 10, 10);
//	c->SetPos(x, y, z);
//	mat4x4 mat = c->transform;
//	vec3 u(0,0,0);
//	float angle = 0.0f;
//	Angles2AxisAngle(inclination, rotation,u,angle);
//	LOG("axis %.2f %.2f %.2f angle %.2f", u.x, u.y, u.z, angle);
//	c->SetRotation(angle, u);
//
//	//c->SetRotation(rotation, vec3(0.0f, 1.0f, 0.0f));
//	PhysBody3D* phys = App->physics->AddBody(*c, 0.0f);
//	
//	map.PushBack(c);
//}

void ModuleSceneIntro::NitroObject(vec3 pos, int size, int distance_to)
{
	Sphere* nitro_obj = nullptr;
	for (int i = 0; i < size; i++)
	{
		if (active)
		{
			nitro_obj = new Sphere(1);
			nitro_obj->color.Set(255, 0, 128, 1.F);
			nitro_obj->SetPos(pos.x + distance_to, pos.y, pos.z);
			nitro_objects.PushBack(nitro_obj);
			distance_to += 4;
			PhysBody3D* sensor = App->physics->AddBody(*nitro_obj, 0);
			sensor->SetAsSensor(true);
			sensor->SetState(PhysBody3D::Tag::NITRO);
			nitro_objects_body.PushBack(sensor);
		}
		
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

			/*if (nitro_objects[i] != nullptr)
			{
				delete nitro_objects[i];
				nitro_objects[i] = nullptr;
			}*/
			//SetActive(active, nitro_objects_body[i]);
		}
	}
	App->player->nitro = true;
}

void ModuleSceneIntro::CreateCheckpoint(vec3 pos)
{
	Cube* checkpoint_obj = nullptr;
	checkpoint_obj = new Cube(15, 2, 2);
	checkpoint_obj->color.Set(0, 255, 0, 1.F);
	checkpoint_obj->SetPos(pos.x, pos.y, pos.z);
	checkpoint_objects.PushBack(checkpoint_obj);
	PhysBody3D* sensor = App->physics->AddBody(*checkpoint_obj, 0);
	sensor->SetAsSensor(true);
	sensor->SetState(PhysBody3D::Tag::CHECKPOINT);
	checkpoint_objects_body.PushBack(sensor);
}

void ModuleSceneIntro::Checkpoint(PhysBody3D* checkpoint_body)
{
	for (uint i = 0; i < checkpoint_objects_body.Count(); i++)
	{
		if (checkpoint_objects_body[i] == checkpoint_body)
		{
			checkpoint_objects.Pop(checkpoint_objects[i]);
			checkpoint_objects_body.Pop(checkpoint_objects_body[i]);
		}
	}
	//Checkpoints 1 - 4
	if(App->player->checkpoint_value >= 4)
		App->player->checkpoint_value = 0;
	else if(App->player->checkpoint_value == 0)
		App->player->checkpoint_value =1;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	if (body1->on_collision && body2->on_collision)
	{
		if (body2->GetState() == PhysBody3D::Tag::NITRO)
		{
			PickUpNitroObject(body2);
			App->physics->DestroyBody(body2);
			App->player->vehicle->nitro_off = 0;
			App->player->vehicle->nitro_on = 255;

			if (current_time >= 1000)
			{
				App->audio->PlayFx(App->player->fx_nitro_pick_up);
				start_time = SDL_GetTicks();
			}
		}

		if (body2->GetState() == PhysBody3D::Tag::CHECKPOINT)
		{
			Checkpoint(body2);
			App->physics->DestroyBody(body2);
			
			if (current_time >= 1000)
			{
				App->audio->PlayFx(App->player->fx_checkpoint);
				start_time = SDL_GetTicks();
			}
		}

		if (body2->GetState() == PhysBody3D::Tag::WALL)
		{

			if (current_time >= 500)
			{
				App->audio->PlayFx(App->player->fx_crash);
				start_time = SDL_GetTicks();
			}
		}
	}
}

void ModuleSceneIntro::SetActive(bool set_active, PhysBody3D * body)
{
	if (set_active)
	{
		active = false;
	}
	else
	{

	}
}

//void ModuleSceneIntro::Angles2AxisAngle(const float & alpha_x, const float & beta_y, vec3 & u, float & angle)
//{
//	double c1 = cos(alpha_x / 2);
//	double s1 = sin(alpha_x / 2);
//	double c2 = cos(beta_y / 2);
//	double s2 = sin(beta_y / 2);
//	double c3 = cos(0 / 2);
//	double s3 = cos(0 / 2);
//	double c1c2 = c1 * c2;
//	double s1s2 = s1 * s2;
//	int w = c1c2 * c3 - s1s2 * s3;
//	int x = c1c2 * s3 + s1s2 * c3;
//	int y = s1 * c2*c3 + c1 * s2*s3;
//	int z = c1 * s2*c3 - s1 * c2*s3;
//	angle = 2 * acos(w);
//	double norm = x * x + y * y + z * z;
//	if (norm < 0.001) { // when all euler angles are zero angle = 0 so
//		// we can set axis to anything to avoid divide by zero
//		x = 1;
//		y = z = 0;
//	}
//	else {
//		norm = sqrt(norm);
//		x /= norm;
//		y /= norm;
//		z /= norm;
//	}
//	u = vec3(x, y, z);
//}
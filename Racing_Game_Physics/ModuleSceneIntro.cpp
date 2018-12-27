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
	CreateCurve(-20-7.0f, 0.0f, 100, 15, 180, 0, road, 20);
	
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

void ModuleSceneIntro::CreateCurve(const float & x, const float & y, const float & z, const float & width, const float & angle_i, const float & angle_f, const Cube & cube, const float & radius)
{

	//PhysBody3D* phys = nullptr;
	Cube* c = nullptr;
	//float l = 2 * 3.1415F * radius * angle_f / 360;
	
	for (int i = angle_f; i < angle_i - angle_f; i = i + cube.size.x*10) {
		c = new Cube(cube);
		c->SetPos(x + radius * cos(i*3.1415 / 360), y, z + radius * sin(i*3.1415 / 360));
		LOG("%i sin(i) = %.2f", i, z + radius * sin(i*3.1415 / 360));
		vec3 pos = App->physics->AddBody(*c, 0.0F)->GetPosition();
		//LOG("pos (%.2f, %.2f)", pos.x, pos.z);
		map.PushBack(c);
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
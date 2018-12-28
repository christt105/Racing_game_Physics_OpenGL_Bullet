#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "ModuleInput.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0F;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	VehicleInfo car;

	// Car properties ----------------------------------------
	car.chassis_size.Set(2, 2, 4);
	car.chassis_offset.Set(0, 1.5, 0);
	car.mass = 500.0F;
	car.suspensionStiffness = 15.88F;
	car.suspensionCompression = 0.83F;
	car.suspensionDamping = 0.88F;
	car.maxSuspensionTravelCm = 1000.0F;
	car.frictionSlip = 50.5F;
	car.maxSuspensionForce = 6000.0F;

	// Wheel properties ---------------------------------------
	float connection_height = 1.2F;
	float wheel_radius = 0.6F;
	float wheel_width = 0.5F;
	float suspensionRestLength = 1.2F;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x*0.5F;
	float half_length = car.chassis_size.z*0.5F;

	direction = {0, -1, 0};
	axis = { -1, 0, 0 };
	
	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width - 0.3F * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width + 0.3F * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width - 0.3F * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width + 0.3F * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;

	vehicle = App->physics->AddVehicle(car);
	vehicle->SetState(PhysBody3D::state::PLAYER);
	vehicle->SetPos(0, 1, 0);
	vehicle->collision_listeners.add(App->scene_intro);

	App->camera->Position.Set(vehicle->GetPosition().x - vehicle->GetLocalPosition().x*CAMERA_OFFSET_X,
		vehicle->GetPosition().y + CAMERA_OFFSET_Y,
		vehicle->GetPosition().z - vehicle->GetLocalPosition().z * CAMERA_OFFSET_Z);
	App->camera->LookAt(vehicle->GetPosition());


	ghost = App->physics->AddVehicle(car);
	ghost->SetState(PhysBody3D::state::GHOST);
	ghost->SetPos(0, 1, 0);
	ghost->SetListener(true);

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	// Inputs
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
		switch (checkpoint_value)
		{
		case 0:
			vehicle->SetPos(0, 1, 0);
			break;
		case 1:
			vehicle->SetPos(0, 1, 50);
			break;
		default:
			vehicle->SetPos(0, 1, 0);
			break;
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN) {
		save_ghost_data = !save_ghost_data;
		timer.Start();
	}
	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) {
		//vehicle->vehicle->getRigidBody()->getOrientation();
		
		ghost->vehicle->getRigidBody()->setWorldTransform(vehicle->vehicle->getRigidBody()->getWorldTransform());
		//vehicle->vehicle->getChassisWorldTransform()
		/*float* mat;
		vehicle->GetTransform(mat);
		ghost->SetTransform(mat);*/
	}

	if (save_ghost_data) {
		if (timer.Read() > 10) {
			ghost_pos.PushBack(Attitude({ vehicle->GetPosition(),vehicle->GetLocalPosition() }));
			LOG("Saved position(%.2f, %.2f, %.2f) on %i", ghost_pos[ghost_pos.Count()-1].pos.x, ghost_pos[ghost_pos.Count()-1].pos.y, ghost_pos[ghost_pos.Count()-1].pos.z, ghost_pos.Count());
			timer.Start();
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {
		timer.Stop();
		timer.Start();
		save_ghost_data = false;
		path_ghost = true;
		iterator_ghost = 0;
		ghost->SetPos(ghost_pos[0].pos);
		ghost->SetLocalPosition(ghost_pos[0].rot);
	}

	if (path_ghost) {
		if (timer.Read() > 10) {
			/*if (iterator_ghost+1 >= ghost_pos.Count())
				iterator_ghost = 0;*/
			if (iterator_ghost + 1 >= ghost_pos.Count()) {
				path_ghost = false;
				ghost_pos.Clear();
				timer.Stop();
				/*for (int i = 0; i < ghost_pos.Count(); ++i) {
					delete ghost_pos[i];
				}*/
			}
			else {
				ghost->SetPos(ghost_pos[++iterator_ghost].pos);
				//ghost.
				ghost->SetLocalPosition(ghost_pos[iterator_ghost].rot);
				timer.Start();
			}
		}
	}
	turn = acceleration = brake = 0.0F;

	if (vehicle->GetKmh() >= 1 && accelerating)
		acceleration = -MAX_ACCELERATION;
	else if (vehicle->GetKmh() <= -1 && decelerating)
		acceleration = MAX_ACCELERATION;
	else
	{
		acceleration = 0;
		accelerating = false;
		decelerating = false;
	}

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		
		if (vehicle->GetKmh() >= 80)
			acceleration = 0;
		else
			acceleration = MAX_ACCELERATION;

		accelerating = true;
	}

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		if(turn < TURN_DEGREES)
			turn +=  TURN_DEGREES;
	}

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		if(turn > -TURN_DEGREES)
			turn -= TURN_DEGREES;
	}

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		if (vehicle->GetKmh() <= -50)
			acceleration = 0;
		else
			acceleration = MAX_DECCELERATION;
		decelerating = true;    
	}
	
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
	{
		NitroSpeed();
	}

	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);

	vehicle->Render();

	if (!App->scene_intro->camera_free) {
		App->camera->Position.Set(vehicle->GetPosition().x - vehicle->GetLocalPosition().x*CAMERA_OFFSET_X,
			vehicle->GetPosition().y + CAMERA_OFFSET_Y,
			vehicle->GetPosition().z - vehicle->GetLocalPosition().z * CAMERA_OFFSET_Z);
		App->camera->LookAt(vehicle->GetPosition());
	}

	return UPDATE_CONTINUE;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

void ModulePlayer::NitroSpeed()
{
	if (nitro)
	{
		start_time = SDL_GetTicks(); 
		nitro = false;
	}

	current_time = SDL_GetTicks() - start_time;
	
	if (current_time <= 1500)
	{
  		acceleration = MAX_ACCELERATION * 2;
		accelerating = true;
	}
	
}
#ifndef __PhysBody3D_H__
#define __PhysBody3D_H__

#include "p2List.h"

class btRigidBody;
class Module;
class vec3;

// =================================================
struct PhysBody3D
{
	friend class ModulePhysics3D;
	enum class state { PLAYER, NITRO };

public:
	PhysBody3D(btRigidBody* body);
	~PhysBody3D();

	void Push(float x, float y, float z);
	void GetTransform(float* matrix) const;
	vec3 GetPosition() const;
	void SetListener(bool listener);
	void SetTransform(const float* matrix) const;
	void SetPos(float x, float y, float z);
	state GetState();
	void SetState(state current_state);

private:
	btRigidBody* body = nullptr;
	bool listener_on = false;
	state current_state;

public:
	p2List<Module*> collision_listeners;
	bool on_collision = true;
};

#endif // __PhysBody3D_H__
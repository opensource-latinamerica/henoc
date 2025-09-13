/*
 * TT102 HENOC 
 * 2008 @ ESCOM-IPN
 * Define los metodos para las clases geometricas y la clase Wordl
 */


#include <ode/ode.h>
#include <cassert>
#include <stack>
#include <algorithm>
#include "henocUniverse.h"

using namespace HenocUniverse;

ObjectProperties Object::defaults = { 5, (dReal)dInfinity, 0.35, 0, ~0U, ~0U, 0 };
std::stack<ObjectProperties> Object::defaultStack;

Object::Object() : properties(defaults) {}

Geometry::Geometry(){
	axis = vec2(1, 0);
	center = vec2(0, 0);
	bounds = aabb(0,0,0,0);
    geom = 0;
}

void Object::Move(){
	assert(IsDynamic() && GetBody());
	const dReal *R = dBodyGetRotation(GetBody());
	{
		dReal c[3] = {1, 0, 0};
		dReal a[4];
		dMultiply0(a, R, c, 4, 3, 1);
		GetGeometry().SetAxis(vec2(a));
	}

	const dReal *position = dBodyGetPosition(GetBody());
	GetGeometry().SetCenter(vec2(position));
	GetGeometry().UpdateBounds();
}

void Object::Rotate(dReal theta){
	GetGeometry().SetAxis(vec2(0, 1));
	GetGeometry().Rotate(theta);
	if (IsDynamic() && GetBody()){
		const dReal degreesToRadians = atan(1.0) / 45;
		dMatrix3 R;
		dRFromAxisAndAngle(R, 0, 0, 1, theta * degreesToRadians);
		dBodySetRotation(GetBody(), R);
	}
}

void Object::SetCenter(const vec2 &center){
	GetGeometry().SetCenter(center);
	if (IsDynamic() && GetBody())
		dBodySetPosition(GetBody(), center.x, center.y, 0);
}

void Geometry::Rotate(dReal theta){
	SetAxis(this->axis.rotate(theta));
}

void Geometry::Rotate(const vec2 &xform){
	SetAxis(this->axis.rotate(xform));
}

void Geometry::SetCenter(const vec2 &center){
	this->center = center;
}

void Geometry::SetAxis(const vec2 &axis){
	this->axis = axis;
}

World::World(){
	world = dWorldCreate();
	contactGroup = dJointGroupCreate(0);
	contactCount = 0;
}

World::~World(){
	dJointGroupDestroy(contactGroup);
	dWorldDestroy(world);
}

void World::QuickStep(dReal f){
	dWorldQuickStep(world, f);
}

Body World::BodyCreate(){
	return dBodyCreate(world);
}

void World::SetAutoDisableLinearThreshold(dReal f){
	dWorldSetAutoDisableLinearThreshold(world, f);
}

void World::SetAutoDisableAngularThreshold(dReal f){
	dWorldSetAutoDisableAngularThreshold(world, f);
}

void World::SetCFM(dReal f){
	dWorldSetCFM(world, f);
}

void World::SetAutoDisableFlag(bool b){
	dWorldSetAutoDisableFlag(world, b ? 1 : 0);
}

void World::SetERP(dReal f){
	dWorldSetERP(world, f);
}

void World::SetContactMaxCorrectingVel(dReal f){
	dWorldSetContactMaxCorrectingVel(world, f);
}

void World::SetContactSurfaceLayer(dReal f){
	dWorldSetContactSurfaceLayer(world, f);
}

void World::SetGravity(const vec2 &v){
	dWorldSetGravity(world, v.x, v.y, 0);
}

dJointID World::AddMotor(Object &object){
	dJointID joint = dJointCreateAMotor(world, 0);
	dJointAttach(joint, object.GetBody(), 0);
	dJointSetAMotorNumAxes(joint, 1);
	dJointSetAMotorAxis(joint, 0, 1, 0, 0, 1);
	dJointSetAMotorParam(joint, dParamFMax, dInfinity);
	return joint;
}

dJointID World::Glue(Object &object1, Object &object2){
	dJointID joint = dJointCreateFixed(world, 0);
	dJointAttach(joint, object1.GetBody(), object2.GetBody());
	dJointSetFixed(joint);
	return joint;
}

dJointID World::AnchorAxis(Object &object, const vec2 &axis){
	dJointID joint = dJointCreateSlider(world, 0);
	dJointAttach(joint, object.GetBody(), 0);
	dJointSetSliderAxis(joint, axis.x, axis.y, 0);
	return joint;
}

dJointID World::Anchor(Object &o1, Object &o2, const vec2 &point, dReal mu, dReal erp){
	dJointID joint = dJointCreateHinge(world, 0);
	dJointAttach(joint, o1.GetBody(), o2.GetBody());
	dJointSetHingeAnchor(joint, point.x, point.y, 0);
	dJointSetHingeAxis(joint, 0, 0, 1);
    dJointSetPUParam(joint, dParamERP, erp);

	if (mu){
		dJointID friction = dJointCreateAMotor(world, 0);
		dJointAttach(friction, o1.GetBody(), o2.GetBody());

		dJointSetAMotorNumAxes(friction, 1);
		dJointSetAMotorAxis(friction, 0, 1, 0, 0, 1);
		dJointSetAMotorParam(friction, dParamFMax, mu);
		dJointSetAMotorParam(friction, dParamVel, 0);
	}
	return joint;
}

dJointID World::Anchor(Object &o1, const vec2 &point, dReal mu, dReal erp){
	dJointID joint = dJointCreateHinge(world, 0);
	dJointAttach(joint, o1.GetBody(), 0);
	dJointSetHingeAnchor(joint, point.x, point.y, 0);
	dJointSetHingeAxis(joint, 0, 0, 1);
    dJointSetPUParam(joint, dParamERP, erp);

	if (mu){
		dJointID friction = dJointCreateAMotor(world, 0);
		dJointAttach(friction, o1.GetBody(), 0);

		dJointSetAMotorNumAxes(friction, 1);
		dJointSetAMotorAxis(friction, 0, 1, 0, 0, 1);
		dJointSetAMotorParam(friction, dParamFMax, mu);
		dJointSetAMotorParam(friction, dParamVel, 0);
	}
	return joint;
}

void World::DeleteJoint(dJointID joint){
	dJointDestroy(joint);
}

void World::SetMotorVelocity(dJointID joint, dReal velocity){
	dJointSetAMotorParam(joint, dParamVel, velocity);
}

dReal World::GetMotorVelocity(dJointID joint){
	return dJointGetAMotorParam(joint, dParamVel);
}

ContactList::ContactList(){
	for (int i = 0; i < Max; ++i){
		contacts[i].geom.pos[2] = 0;
		contacts[i].geom.normal[2] = 0;
	}
	Reset(0, 0);
}

void ContactList::Reset(Object *o1, Object *o2){
	count = 0;
	invertNormals = false;
	this->o1 = o1;
	this->o2 = o2;
}

void ContactList::AddContact(const vec2 &position, const vec2 &normal, dReal depth){
	assert(count < Max - 1);
	dContactGeom &cg = contacts[count].geom;
	cg.pos[0] = position.x;
	cg.pos[1] = position.y;
	if (invertNormals){
		cg.normal[0] = -normal.x;
		cg.normal[1] = -normal.y;
	}
	else{
		cg.normal[0] = normal.x;
		cg.normal[1] = normal.y;
	}
	cg.depth = depth;
	++count;
}

void ContactList::Finalize(){
	ObjectProperties &p1 = o1->Property();
	ObjectProperties &p2 = o2->Property();

	for (int i = 0; i < count; ++i){
		dContact &c = contacts[i];
		c.surface.mode = dContactBounce;

		if (p1.frictionMask & p2.frictionMask){
			if (p1.friction == dInfinity || p2.friction == dInfinity)
				c.surface.mu = dInfinity;
			else
				c.surface.mu = p1.friction * p2.friction;
		}
		else
			c.surface.mu = 0;

		c.surface.bounce = (p1.bounceFactor + p2.bounceFactor) / 2;
		c.surface.bounce_vel = (p1.bounceVelocity + p2.bounceVelocity) / 2;

		dContactGeom &cg = contacts[i].geom;
		cg.g1 = o1->GetGeometry().GetGeomID();
		cg.g2 = o2->GetGeometry().GetGeomID();
	}

	if (p1.callback) p1.callback(*this);

	if (p2.callback){
		std::swap(o1, o2);
		p2.callback(*this);
		std::swap(o1, o2);
	}
}

void ContactList::CreateJoints(dWorldID world, dJointGroupID contactGroup) const{
	for (int i = 0; i < count; ++i){
		const dContact &c = contacts[i];
		dJointID joint = dJointCreateContact(world, contactGroup, &c);

		const dContactGeom &cg = c.geom;
		dJointAttach(joint, dGeomGetBody(cg.g1), dGeomGetBody(cg.g2));
	}
}

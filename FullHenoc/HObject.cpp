#include<HObject.h>

HObject::HObject(){
	mass = 5.0;
	friction = 1.0;
	bounceFactor = 0.35;
	bounceVelocity = 0;
	colMask =  ~0;
	frictionMask =  ~0;
	rotation = 0;	
	type = 0;
	l = 0;
	w = 0;
	h = 0;
	color = 0.0;
	lastRotation = 0.0;
}
;
float HObject::getMass(){ return mass; };
float HObject::getFriction(){ return friction; };
float HObject::getBounceFactor(){ return bounceFactor; };
float HObject::getBounceVelocity(){ return bounceVelocity; };
int HObject::getColMask(){ return colMask; };
int HObject::getFrictionMask(){ return frictionMask; };
float HObject::getRotation(){ return rotation; };
int HObject::getType(){ return type; };
int HObject::length(){ return l; };
int HObject::width(){ return w; };
int HObject::height(){ return h; };
float HObject::getColor(){ return color; };

void HObject::setMass(float m){ mass = m; };
void HObject::setFriction(float f){ friction = f; };
void HObject::setBounceFactor(float b){ bounceFactor = b; };
void HObject::setBounceVelocity(float b){ bounceVelocity = b; };
void HObject::setColMask(int c){ colMask = c; };
void HObject::setFrictionMask(int f){ frictionMask = f; };
void HObject::setRotation(float r){ rotation = r; };	
void HObject::setType(int t){ type = t; };
void HObject::setLength(int pl){ l = pl; };
void HObject::setWidth(int pw){ w = pw; };
void HObject::setHeight(int ph){ h = ph; };
void HObject::setColor(float c){ color = c; };


#ifndef __HOBJECT_H__
#define __HOBJECT_H__

#include<QRectF>

class HObject{
	private:
		int type;
		float mass;
		float friction;
		float bounceFactor;
		float bounceVelocity;
		int colMask;
		int frictionMask;
		float rotation;
		int l;
		int w;
		int h;
		float color;
	public:
		HObject();
		float getMass();
		float getFriction();
		float getBounceFactor();
		float getBounceVelocity();
		int getColMask();
		int getFrictionMask();
		float getRotation();
		int getType();	
		int length();
		int width();
		int height();
		float getColor();
		float lastRotation;

		void setMass(float m);
		void setFriction(float f);
		void setBounceFactor(float b);
		void setBounceVelocity(float b);
		void setColMask(int c);
		void setFrictionMask(int f);
		void setRotation(float r);
		void setType(int);
		void setLength(int);
		void setWidth(int);
		void setHeight(int);
		void setColor(float);
};

#endif

#include <QtGui>
#include <QtOpenGL>

#include <math.h>

#include "glwidget.h"
#include "henocUniverseI.cpp"

#include<iostream>

using namespace std;
using namespace HenocUniverseI;

const int MaxObjects = 40;

void AddFallingObject(Space &space, Object *object){
	if (space.size() >= MaxObjects)
		space.pop();

	Color pastel(60.0f + (rand() % 90) - 45);
	object->Property().outlineColor = pastel;
	pastel.IncreaseBrightness(0.5);
	object->Property().fillColor = pastel;
	object->Property().outlineThickness = 1;
	object->GetFlatlandObject()->SetMass(5.1);    
	space.push_back(object);
}

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent){
	//app(width(), height() );
}

GLWidget::~GLWidget(){
	exit(0);
}

QSize GLWidget::minimumSizeHint() const{
	return QSize(600, 500);
}

QSize GLWidget::sizeHint() const{
	return QSize(800, 600);
}


void GLWidget::initializeGL(){
	app.InitGl();
	app.InitOde();

	// Insert some objects into the space.
	Space &space = app.GetSpace();
	//space << new Demo::Line(vec2(0, 0), vec2(655, 517));
	//space << new Demo::Line(vec2(800, 0), vec2(800, 600));
	//space << new Demo::Line(vec2(0, 0), vec2(0, 600));
	/*{
		Wheel *wheel = new Wheel(vec2(390, 550), 50);
		wheel->GetFlatlandObject()->SetMass(200);
		//wheel->GetFlatlandObject()->SetMass(11000);
		wheel->Property().outlineThickness = 1;
		wheel->Insert(space);
	}*/

	// Catapult
	/*{
		vec2 center(500, 200);
		float width = 200;
		float height = 5;
		vec2 hinge(center.x - width / 4, center.y);
		Catapult *c = new Catapult(center, width, height, hinge);
		c->Rotate(-22);
		c->Insert(space);
		c->InsertSouthEastAnchor(space);

		center += vec2(90, -50);
		Block *b = new Block(center, 10, 10);
		b->GetFlatlandObject()->SetMass(5.5);
		b->GetFlatlandObject()->Property().frictionMask = 1;//dInfinity;
		b->Property().fillColor = Color::PaleYellow;
		b->Property().outlineColor = Color::PaleYellow;
		b->Insert(space);

		center.x -= 14;
		b = new Block(center, 10, 10);
		b->GetFlatlandObject()->SetMass(0.1);
		b->GetFlatlandObject()->Property().frictionMask = 1;//dInfinity;
		b->GetFlatlandObject()->Property().friction = 1;//dInfinity;
		b->Property().fillColor = Color::PaleYellow;
		b->Property().outlineColor = Color::PaleYellow;
		b->Insert(space);
	}*/
	space.mark();
}

void GLWidget::paintGL(){
	app.Draw();
}

void GLWidget::resizeGL(int width, int height){
	app.Resize(width, height);
}


void GLWidget::animate(){
	const float fps = 40.0;
	static int stepCount = 0;
	// Simulation step size; proportional to speed and inversely proportional to precision.
	const float delta = 0.25;
    
/*    unsigned int currentTime = osGetMilliseconds();
    unsigned int previousSampleTime = currentTime;
    unsigned int previousDrawTime = currentTime;
    const unsigned int sampleDelay = 5000;
    const unsigned int drawDelay = (unsigned int) (1000.0 / fps);

    // Count the simulation "steps" (iterations) since the last performance sample.
    unsigned int stepCount = 0;
*/


	Space &space = app.GetSpace();
	app.Draw();
	app.Step(delta);
	++stepCount;
    //
    //
/*    if (space.size() < MaxObjects)
        {
            if (!(stepCount % 100))
                AddFallingObject(space, new Ball(vec2(400, 0), 15));
            if (!((stepCount + 50) % 100))
                AddFallingObject(space, new Block(vec2(400, 0), 20, 20));
        }
*/
    //AddFallingObject(space, new Ball(vec2(400, 0), 25));
	updateGL();
    
}

void GLWidget::addBox(int x, int y, int w, int h, float mass, float friction, float bounceFactor, float bounceVelocity, int colMask, int frictionMask, int rotation, float pColor ){
	Space &space = app.GetSpace();
	Object *object = new Block(vec2(x,y), w, h);
	Color pastel(pColor);
	object->Property().outlineColor = pastel;
	pastel.IncreaseBrightness(0.5);
	object->Property().fillColor = pastel;
	object->Property().outlineThickness = 1;
	object->GetFlatlandObject()->SetMass(mass);

	object->GetFlatlandObject()->Property().friction = ( friction > 99 ) ? dInfinity : friction ;		
	object->GetFlatlandObject()->Property().bounceFactor = bounceFactor;		
	object->GetFlatlandObject()->Property().bounceVelocity = bounceVelocity;		
	object->GetFlatlandObject()->Property().bounceFactor = bounceFactor;
	object->GetFlatlandObject()->Property().collisionMask = (!colMask) ? ~0 : colMask;
	object->GetFlatlandObject()->Property().frictionMask = (!frictionMask) ? ~0 : frictionMask;

	if(rotation){
		object->GetFlatlandObject()->Rotate(rotation);
	}

	space.push_back(object);
}

void GLWidget::addBall(int x, int y, int r, float mass, float friction, float bounceFactor, float bounceVelocity, int colMask, int frictionMask, int rotation, float pColor ){

	//cout << "x=" << x << endl;
	//cout << "y=" << y << endl;
	//cout << "r=" << y << endl;
	//cout << "mass=" << mass << endl;

	Space &space = app.GetSpace();
	Object *object = new Ball(vec2(x,y), r);
	Color pastel(pColor);
	//Color pastel(60.0f + (rand() % 90) - 45);
	object->Property().outlineColor = pastel;
	pastel.IncreaseBrightness(0.5);
	object->Property().fillColor = pastel;
	object->Property().outlineThickness = 1;
	object->GetFlatlandObject()->SetMass(mass);

	object->GetFlatlandObject()->Property().friction = ( friction > 99 ) ? dInfinity : friction ;		
	object->GetFlatlandObject()->Property().bounceFactor = bounceFactor;		
	object->GetFlatlandObject()->Property().bounceVelocity = bounceVelocity;		
	object->GetFlatlandObject()->Property().bounceFactor = bounceFactor;
	object->GetFlatlandObject()->Property().collisionMask = (!colMask) ? ~0 : colMask;
	object->GetFlatlandObject()->Property().frictionMask = (!frictionMask) ? ~0 : frictionMask;

//	if(rotation){
//		object->GetFlatlandObject()->Rotate(rotation);
//	}

	space.push_back(object);
}

void GLWidget::addLine(int x, int y, int w, int h, float friction, int colMask, int frictionMask, float pColor){
	Space &space = app.GetSpace();

	Line *l = new Line(vec2(x, y), vec2(w, h));
	//l->GetFlatlandObject()->Property().friction = friction;		
	//l->GetFlatlandObject()->Property().frictionMask =  (!frictionMask) ? ~0 : frictionMask;
	//l->GetFlatlandObject()->Property().collisionMask =  (!colMask) ? ~0 : colMask;
	Color pastel(pColor);
	l->Property().outlineColor = pastel;

	l->Insert(space);

//	space << new HenocUniverseI::Line(vec2(x, y), vec2(w, h));
}

void GLWidget::delSpace(){
	Space &space = app.GetSpace();
	space.destruye();
}

void GLWidget::setWorldParams(const Whstc &pw){
	app.setOdeW(pw);
}



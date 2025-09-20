/*
 * TT102 HENOC 
 * 2008 @ ESCOM-IPN
 * Implementacion de HenocUniverse
 */


#include <ode/objects.h>
#include <shapes.h>
#include <fstream>
#include <string>
#include "henocUniverseI.h"

using HenocUniverse::vec2;
using namespace HenocUniverseI;

// Singleton instance is created on first HU_Init() call.

Color Color::Transparent(0,0,0,0);
Color Color::Black(0,0,0,1);
Color Color::White(1,1,1,1);
Color Color::Red(1,0,0,1);
Color Color::Green(0,1,0,1);
Color Color::Blue(0,0,1,1);
Color Color::PaleRed(1,0.6f,0.6f,1);
Color Color::PaleYellow(0.9f,0.8f,0.5f,1);
Color Color::PaleGreen(0.75f,0.85f,0.75f,1);

const float Point::Size = 10;

void HenocUniverseI::glColor(const Color &color) { glColor4fv((float*) &color); }
void HenocUniverseI::glVertex(const vec2 &v) { glVertex2fv((float*) &v); }

ObjectProperties Object::defaults ={
	2,              // outlineThickness
	{1, 1, 1, 0.5}, // fillColor
	{1, 1, 1, 0},   // altFillColor
	{1, 1, 1, 1},   // outlineColor
	false           // offset
};

std::stack<ObjectProperties> Object::defaultStack;

#define AppInstance HenocWrapper::GetSingleton()
#define World AppInstance.GetWorld()

void HenocUniverseI::Draw(const HenocUniverse::Geometry &g, const ObjectProperties &appearance){
	switch (g.GetShape()){
		case HenocUniverse::Shape::Circle: DrawCircle(g, appearance); return;
		case HenocUniverse::Shape::Quad: DrawQuad(g, appearance); return;
	}
}

void HenocUniverseI::DrawCircle(const HenocUniverse::Geometry &g, const ObjectProperties &appearance){
	assert(g.GetShape() == HenocUniverse::Shape::Circle);
	const HenocUniverse::Circle &circle = static_cast<const HenocUniverse::Circle&>(g);

	const float width = appearance.outlineThickness;
	const Color &fill = appearance.fillColor;
	const Color &outline = appearance.outlineColor;
	const float offset = appearance.offset ? width / 2 : 0;

	float radius = circle.Radius() - offset;
	const int slices = (int) (radius <= 10 ? 20 : radius);
	const float delta = M_PI * 2 / slices;

	if (fill.a){
		glColor(fill);
		glBegin(GL_POLYGON);
		for (float theta = 0; theta < M_PI * 2; theta += delta)
			glVertex(circle.Center() + vec2(sinf(theta), cosf(theta)) * radius);
			glEnd();
	}

	if (!width || !outline.a) return;

	if (width > 2){
		float inner = radius - width / 2;
		float outer = radius + width / 2;

		glColor(outline);
		glBegin(GL_QUAD_STRIP);
		for (float theta = 0; theta < M_PI * 2 + delta; theta += delta){
			glVertex(circle.Center() + vec2(sinf(theta), cosf(theta)) * inner);
			glVertex(circle.Center() + vec2(sinf(theta), cosf(theta)) * outer);
		}
		glEnd();

		glLineWidth(1);
		glBegin(GL_LINE_LOOP);
		for (float theta = 0; theta < M_PI * 2; theta += delta)
			glVertex(circle.Center() + vec2(sinf(theta), cosf(theta)) * inner);
		glEnd();
		glBegin(GL_LINE_LOOP);
		for (float theta = 0; theta < M_PI * 2; theta += delta)
			glVertex(circle.Center() + vec2(sinf(theta), cosf(theta)) * outer);
		glEnd();

		return;
	}

	glLineWidth(width);
	glColor(outline);
	glBegin(GL_LINE_LOOP);
	for (float theta = 0; theta < M_PI * 2; theta += delta)
		glVertex(circle.Center() + vec2(sinf(theta), cosf(theta)) * radius);
	glEnd();
}

void HenocUniverseI::DrawQuad(const HenocUniverse::Geometry &g, const ObjectProperties &appearance){
	assert(g.GetShape() == HenocUniverse::Shape::Quad);
	const HenocUniverse::Quad &quad = static_cast<const HenocUniverse::Quad&>(g);

	const float width = appearance.outlineThickness;
	const Color &fill = appearance.fillColor;
	const Color &outline = appearance.outlineColor;
	const float offset = appearance.offset ? width / 2 : 0;

	vec2 eaxis[2] ={
		quad.Axis(0) * (quad.Extent(0) - offset),
		quad.Axis(1) * (quad.Extent(1) - offset)
	};

	vec2 corners[4];
	const vec2 &center = quad.Center();
	corners[0] = center - eaxis[0] - eaxis[1];
	corners[1] = center + eaxis[0] - eaxis[1];
	corners[2] = center + eaxis[0] + eaxis[1];
	corners[3] = center - eaxis[0] + eaxis[1];

	if (fill.a){
		glColor(fill);
		glBegin(GL_QUADS);
		glVertex(corners[0]);
		glVertex(corners[1]);
		glVertex(corners[2]);
		glVertex(corners[3]);
		glEnd();
	}

	if (!width || !outline.a) return;

	glLineWidth(width);
	glColor(outline);
	glBegin(GL_LINE_LOOP);
	glVertex(corners[0]);
	glVertex(corners[1]);
	glVertex(corners[2]);
	glVertex(corners[3]);
	glEnd();

	glPointSize(width);
	glBegin(GL_POINTS);
	glVertex(corners[0]);
	glVertex(corners[1]);
	glVertex(corners[2]);
	glVertex(corners[3]);
	glEnd();
	glPointSize(Point::Size);
}

Space::~Space(){
	for (m = list.begin(); m != list.end(); ++m)
		delete *m;
}

Wall::Wall(float left, float top, float right, float bottom) :
object(new HenocUniverse::Block(left, top, right, bottom)){
}

Wall::Wall(vec2 center, float width, float height) :
object(new HenocUniverse::Block(center, width, height)){
}

Block::Block(vec2 center, float width, float height) :
object(new HenocUniverse::Block(center, width, height), World.BodyCreate()){
}

Beam::Beam(vec2 start, vec2 end, float thickness) : Composite((start + end) / 2){
	vec2 center = (start + end) / 2;
	float length = (end - start).length();
	vec2 axis(length / 2, 0);

	HenocUniverse::Block *spine = new HenocUniverse::Block(center, length, thickness);
	HenocUniverse::Circle *end1 = new HenocUniverse::Circle(center + axis, thickness / 2);
	HenocUniverse::Circle *end2 = new HenocUniverse::Circle(center - axis, thickness / 2);

	Add(spine);
	Add(end1);
	Add(end2);

	theta = 180 / M_PI * atan2f(end.y - start.y, end.x - start.x);
}

void Beam::Insert(Space &space){
	Composite::Insert(space);
	GetFlatlandObject()->Rotate(theta);
}

void Beam::InsertFront(Space &space){
	Composite::InsertFront(space);
	GetFlatlandObject()->Rotate(theta);
}

Line::Line(vec2 a, vec2 b) : object(new HenocUniverse::Line(a, b)) {}
RopeSegment::RopeSegment(vec2 a, vec2 b) : object(new HenocUniverse::Line(a, b), World.BodyCreate()){
}

Terrain::Terrain(vec2 start, float bottom, bool borders, bool borderVisibility) : bottom(bottom),
borders(borders), borderVisibility(borderVisibility), object(new HenocUniverse::Terrain(start)){
}

Ball::Ball(HenocUniverse::vec2 center, float radius) :
object(new HenocUniverse::Circle(center, radius), World.BodyCreate()){
	object.Property().friction = 0;
}

Point::Point(HenocUniverse::vec2 center) :
object(new HenocUniverse::Circle(center, Size / 2), World.BodyCreate()){
	Property().fillColor = Color(1,0,0,1);
}

Wheel::Wheel(HenocUniverse::vec2 center, float radius) :
object(new HenocUniverse::Circle(center, radius), World.BodyCreate()){
	texture = 0;
}

Composite::Composite(vec2 centroid) :
object(new HenocUniverse::Composite(centroid), World.BodyCreate()){
}

void Terrain::Add(const HenocUniverse::vec2 &v){
	object.GetGeometry().push_back(v);
}

void Composite::Add(HenocUniverse::Geometry *geometry){
	object.GetGeometry().push_back(geometry);
}

void Wall::Draw() const{
	DrawQuad(object.GetGeometry(), Property());
}

void Ball::Draw() const{
	DrawCircle(object.GetGeometry(), Property());
}

void Point::Draw() const{
	glColor(Property().fillColor);
	glBegin(GL_POINTS);
	glVertex(object.GetGeometry().Center());
	glEnd();
}

void Block::Draw() const{
	DrawQuad(object.GetGeometry(), Property());
}

void Wheel::Draw() const{
	if (texture){
		float radius;
		glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &radius);
		radius /= 2;

		vec2 axis0 = object.GetGeometry().Axis(0);
		vec2 axis1 = object.GetGeometry().Axis(1);
		vec2 eaxis[2] = { axis0 * radius, axis1 * radius };

		vec2 corners[4];
		const vec2 &center = object.GetGeometry().Center();
		corners[0] = center - eaxis[0] - eaxis[1];
		corners[1] = center + eaxis[0] - eaxis[1];
		corners[2] = center + eaxis[0] + eaxis[1];
		corners[3] = center - eaxis[0] + eaxis[1];

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
		glColor(Color::White);
		glPushMatrix();
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex(corners[0]);
		glTexCoord2f(1, 0);
		glVertex(corners[1]);
		glTexCoord2f(1, 1);
		glVertex(corners[2]);
		glTexCoord2f(0, 1);
		glVertex(corners[3]);
		glEnd();
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
		return;  
	}

	const float radius = object.GetGeometry().Radius();
	const vec2 axis = object.GetGeometry().Axis();
	const vec2 center = object.GetGeometry().Center();
	const int slices = (int) radius;
	const float delta = M_PI / slices;

	glColor(Property().fillColor);
	glBegin(GL_POLYGON);
	for (float theta = 0; theta < M_PI * 2 + delta / 2; theta += delta){
		vec2 spine(sinf(theta), cosf(theta));
		glVertex(center + spine.rotate(axis.flip()) * radius);
	}
	glEnd();

	glColor(Property().altFillColor);
	glBegin(GL_POLYGON);
	for (float theta = M_PI; theta < M_PI * 2 + delta / 2; theta += delta){
		vec2 spine(sinf(theta), cosf(theta));
		glVertex(center + spine.rotate(axis.flip()) * radius);
	}
	glEnd();

	if (!Property().outlineThickness)
		return;

	glLineWidth(Property().outlineThickness);
	glColor(Property().outlineColor);
	glBegin(GL_LINE_STRIP);
	for (float theta = 0; theta < M_PI * 2 + delta / 2; theta += delta)
		glVertex(center + vec2(sinf(theta), cosf(theta)) * radius);
	glEnd();
	glBegin(GL_LINES);
	glVertex(center + axis * radius);
	glVertex(center - axis * radius);
	glEnd();
}

void Composite::Draw() const{
	ObjectProperties filled = Property();
	ObjectProperties outline = Property();
	filled.outlineColor = Color::Transparent;
	outline.fillColor = Color::Transparent;
	outline.offset = false;
	filled.offset = false;

	const HenocUniverse::Composite &geom = object.GetGeometry();

	for (HenocUniverse::Composite::const_iterator g = geom.begin(); g != geom.end(); ++g)
		::Draw(**g, outline);

	for (HenocUniverse::Composite::const_iterator g = geom.begin(); g != geom.end(); ++g)
		::Draw(**g, filled);

	glLineWidth(1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	for (HenocUniverse::Composite::const_iterator g = geom.begin(); g != geom.end(); ++g)
		::Draw(**g, filled);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Line::Draw() const{
	glLineWidth(Property().outlineThickness);
	glColor(Property().outlineColor);
	glBegin(GL_LINES);
	glVertex(object.GetGeometry().Origin());
	glVertex(object.GetGeometry().End());
	glEnd();
}

void Terrain::Draw() const{
	const HenocUniverse::Terrain &geom = object.GetGeometry();
	glBegin(GL_QUAD_STRIP);
	for (HenocUniverse::Terrain::const_iterator v = geom.begin(); v != geom.end(); ++v){
		float mu;
		if (v->y > bottom)
			mu = (v->y - bottom) / (geom.GetBounds().bottom - bottom);
		else
			mu = (bottom - v->y) / (bottom - geom.GetBounds().top);

		glColor(Color::Blend(Property().fillColor, Property().altFillColor, mu));
		glVertex2f(v->x, v->y + Property().outlineThickness / 2);
		glColor(Property().altFillColor);
		glVertex2f(v->x, bottom);
	}
	glEnd();

	if (!Property().outlineThickness) return;

	glLineWidth(Property().outlineThickness);
	glColor(Property().outlineColor);
	glBegin(GL_LINE_STRIP);

	for (HenocUniverse::Terrain::const_iterator v = geom.begin(); v != geom.end(); ++v){
		if (borders && borderVisibility && v == geom.begin())
			glVertex2f(v->x - 0.5f, v->y + Property().outlineThickness / 2);
		else if (borders && !borderVisibility && v == --geom.end())
			glVertex2f(v->x - 0.5f, v->y + Property().outlineThickness / 2);
		else
			glVertex2f(v->x, v->y + Property().outlineThickness / 2);
	}
	glEnd();
}

Line *Terrain::InsertSouthBorder(Space &space) const{
	vec2 begin = object.GetGeometry().front();
	vec2 end = object.GetGeometry().back();
	Line *l = new Line(vec2(begin.x - 0.5f, bottom), vec2(end.x, bottom));
	space << l;
	return l;
}

Line *Terrain::InsertEastBorder(Space &space) const{ 
	vec2 begin = object.GetGeometry().front();
	vec2 end = object.GetGeometry().back();
	Line *l = new Line(end, vec2(end.x, bottom));
	space << l;
	return l;
}

Line *Terrain::InsertWestBorder(Space &space) const{ 
	vec2 begin = object.GetGeometry().front();
	float top = begin.y;
	float x = begin.x - 0.5f;
	Line *l = new Line(vec2(x, top), vec2(x, bottom));
	space << l;
	return l;
}

void HenocUniverseI::DrawContact(const dContact &contact){
	const dContactGeom &cg = contact.geom;
	vec2 position(cg.pos);
	vec2 normal(cg.normal);

	glColor4f(1.0, 1.0, 1.0, 1.0);
	glLineWidth(5);
	glBegin(GL_LINES);
	glVertex(position);
	glColor4f(0, 0, 0, 0);
	glVertex(position + normal * 40);
	glEnd();
}

void Object::Insert(Space &space){
	space.push_back(this);
}

void Object::InsertFront(Space &space){
	space.push_front(this);
}

HenocWrapper *HenocWrapper::singleton = 0;

HenocWrapper &HenocWrapper::GetSingleton(){
	assert(singleton);
	return *singleton;
}

HenocWrapper::HenocWrapper(){
	assert(!singleton);
	singleton = this;

	width = 655;
	height = 517;
	center = target = velocity = vec2(0, 0);
	zoom = 0.5f;
}

void HenocWrapper::InitGl(){
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glClearColor( 1.0, 1.0, 1.0, 0.0 );
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glLineWidth(2);
	glPointSize(Point::Size);

	Resize(width, height);
}

void HenocWrapper::InitOde(){
	world.SetCFM(0);
	world.SetAutoDisableFlag(false);
	world.SetAutoDisableLinearThreshold(0.25f);
	world.SetERP(0.2f);
	world.SetContactMaxCorrectingVel(1);
	world.SetContactSurfaceLayer(0.01f);
	world.SetGravity(vec2(0, 0.5f));
}

void HenocWrapper::Draw() const{
	Clear();
	DrawScene();
}

void HenocWrapper::Clear() const{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void HenocWrapper::DrawScene() const{
	float tx = round(-center.x) + 0.25f;
	float ty = round(-center.y) + 0.25f;
	glTranslatef(tx, ty, 0);

	for (Space::const_iterator i = space.begin(); i != space.end(); ++i)
		(*i)->Draw();
}

void HenocWrapper::Resize(int width, int height){
	float cx = width / 2.0f;
	float cy = height / 2.0f;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(cx - width * zoom, cx + width * zoom, cy + height * zoom, cy - height * zoom, -2, 2);
	glMatrixMode(GL_MODELVIEW);

	this->width = width;
	this->height = height;
}

Color::Color(float theta){
	if (theta < 0)
		theta = 360 + theta;
	if (theta >= 360)
		theta -= 360;

	if (theta < 120){
		g = theta / 120;
		r = 1 - g;
		b = 0;
	}
	else if (theta < 240){    
		b = (theta - 120) / 120;
		g = 1 - b;
		r = 0;
	}
	else{
		r = (theta - 240) / 120;
		b = 1 - r;
		g = 0;
	}
	a = 1;
}

void Color::IncreaseBrightness(float factor){
	r += factor;
	g += factor;
	b += factor;
	if (r > 1)
		r = 1;
	if (g > 1)
		g = 1;
	if (b > 1)
		b = 1;
}

void Color::DecreaseBrightness(float factor){
	r -= factor;
	g -= factor;
	b -= factor;
	if (r < 0)
		r = 0;
	if (g < 0)
		g = 0;
	if (b < 0)
		b = 0;
}

Color Color::Blend(const Color &a, const Color &b, float mu){
	Color c;
	c.r = a.r * mu + b.r * (1 - mu);
	c.g = a.g * mu + b.g * (1 - mu);
	c.b = a.b * mu + b.b * (1 - mu);
	c.a = a.a * mu + b.a * (1 - mu);
	return c;
}

void HenocWrapper::GetImageSize(const char *filename, int &width, int &height){
	assert(sizeof(int) == 4);

	std::ifstream infile(filename, std::ios_base::binary);
	if (!infile)
		fprintf(stderr,"unable to open '%s'", filename);

	infile.read((char*) &width, 4);
	infile.read((char*) &height, 4);
}

void HenocWrapper::GetImageData(const char *filename, void *pixels){
	assert(sizeof(int) == 4);

	std::ifstream infile(filename, std::ios_base::binary);
	if (!infile)
		fprintf(stderr,"unable to open '%s'", filename);

	int width, height;
	infile.read((char*) &width, 4);
	infile.read((char*) &height, 4);
	infile.read((char*) pixels, width * height * 4);
}

Texture HenocWrapper::LoadTexture(const char *filename){
	int width, height;
	std::string path = "../tests/textures/";
	std::string fullpath = path + filename;

	std::ifstream dummy(fullpath.c_str(), std::ios_base::binary);
	if (!dummy)
		fullpath = "../" + path + filename;
	dummy.close();

	GetImageSize(fullpath.c_str(), width, height);
	char *pixels = new char[width * height * 4];
	GetImageData(fullpath.c_str(), pixels);
	Texture texId = LoadTexture(width, height, pixels);
	delete pixels;
	return texId;
}

Texture HenocWrapper::LoadTexture(int width, int height, const void *data){
	Texture texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, width);
	glPixelStorei(GL_UNPACK_LSB_FIRST, GL_TRUE);
	glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	return texId;
}

void HenocWrapper::Step(float delta){
	world.GenerateContacts(space);
	world.QuickStep(delta);
	assert(!world.IsCorrupt(space));
}

int HenocWrapper::TextureWidth(Texture id){
	int width = 0;
	glBindTexture(GL_TEXTURE_2D, id);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
	return width;
}

int HenocWrapper::TextureHeight(Texture id){
	int height = 0;
	glBindTexture(GL_TEXTURE_2D, id);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &height);
	return height;
}

void HenocWrapper::Blit(Texture texId, float left, float top, Color color, bool flip){
	float width;
	float height;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texId);
	glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
	glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &height);
	glColor(color);
	glPushMatrix();
	glTranslatef(left,top,0);
	glBegin(GL_QUADS);
	glTexCoord2i(0, flip ? 1 : 0);
	glVertex2f(0,0);
	glTexCoord2i(1, flip ? 1 : 0);
	glVertex2f(width,0);
	glTexCoord2i(1, flip ? 0 : 1);
	glVertex2f(width,height);
	glTexCoord2i(0, flip ? 0 : 1);
	glVertex2f(0,height);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void HenocWrapper::Zoom(float factor){
	zoom += factor;
	Resize(width, height);
}

dJointID HenocWrapper::Glue(Object *a, Object *b){
	return world.Glue(*a->GetFlatlandObject(), *b->GetFlatlandObject());
}

dJointID HenocWrapper::Anchor(Object *a, Object *b, vec2 p, float mu, float erp){
	return world.Anchor(*a->GetFlatlandObject(), *b->GetFlatlandObject(), p, mu, erp);
}

dJointID HenocWrapper::Anchor(Object *a, vec2 p, float mu, float erp){
	return world.Anchor(*a->GetFlatlandObject(), p, mu, erp);
}

dJointID HenocWrapper::AnchorAxis(Object *a, vec2 axis){
	return world.AnchorAxis(*a->GetFlatlandObject(), axis);
}

Rope::Rope(vec2 start, vec2 end, float length, float tautness) : start(start), end(end), terrain(false){
	const float erp = 0.01f;
	const float dx = 20;
	vec2 dir = (end - start).hat();
	vec2 delta = dir * dx;

	RopeSegment *previous = new RopeSegment(start, start + delta);
	segments.push_back(previous);
	start += delta;

	for (float x = dx; x < length; x += dx){
		vec2 end = start + delta;
		RopeSegment *current = new RopeSegment(start, end);
		segments.push_back(current);
		AppInstance.Anchor(previous, current, start, tautness, erp);
		previous = current;
		start = end;
	}
	previous->GetFlatlandObject()->SetCenter(end - delta / 2);
}

void Rope::Insert(Space &space){
	for (RopeVector::iterator i = segments.begin(); i != segments.end(); ++i)
		space << *i;
	space << this;
}

void Rope::Terrainify(float bottom){
	this->bottom = bottom;
	terrain = true;
}

void Rope::Draw() const{
	if (terrain){
		float minY = std::min(start.y, end.y);
		float maxY = std::max(start.y, end.y);

		Color color = Property().fillColor;
		glBegin(GL_QUAD_STRIP);

		vec2 v = start;

		float mu;
		if (v.y > bottom)
			mu = (v.y - bottom) / (maxY - bottom);
		else
			mu = (bottom - v.y) / (bottom - minY);

		glColor(Color::Blend(Property().fillColor, Property().altFillColor, mu));
		glVertex2f(v.x, v.y + Property().outlineThickness / 2);
		glColor(Property().altFillColor);
		glVertex2f(v.x, bottom);

		for (RopeVector::const_iterator i = segments.begin(); i != segments.end(); ++i){
			v = (i == segments.end() - 1) ? end : (*i)->End();
			float mu;
			if (v.y > bottom)
				mu = (v.y - bottom) / (maxY - bottom);
			else
				mu = (bottom - v.y) / (bottom - minY);

			glColor(Color::Blend(Property().fillColor, Property().altFillColor, mu));
			glVertex2f(v.x, v.y + Property().outlineThickness / 2);
			glColor(Property().altFillColor);
			glVertex2f(v.x, bottom);
		}
		glEnd();

		if (!Property().outlineThickness) return;
		glLineWidth(Property().outlineThickness);
		glColor(Property().outlineColor);
		glBegin(GL_LINE_STRIP);
		v = start;
		glVertex2f(v.x, v.y + Property().outlineThickness / 2);
		for (RopeVector::const_iterator i = segments.begin(); i != segments.end(); ++i){
			v = (i == segments.end() - 1) ? end : (*i)->End();
			glVertex2f(v.x, v.y + Property().outlineThickness / 2);
		}
		glEnd();
		return;
	}

	glLineWidth(Property().outlineThickness);
	glColor(Property().outlineColor);
	glBegin(GL_LINE_STRIP);
	glVertex(segments.front()->Origin());
	for (RopeVector::const_iterator i = segments.begin(); i != segments.end(); ++i)
		glVertex((*i)->End());
	glEnd();
}

Catapult::Catapult(vec2 center, float width, float height, vec2 hinge){
	arm = new Block(center, width, height);
	arm->GetFlatlandObject()->SetMass(1);
	AppInstance.Anchor(arm, hinge);

	this->hinge = new Point(hinge);
	this->hinge->GetFlatlandObject()->Property().collisionMask = 0;
	AppInstance.Anchor(this->hinge, hinge);
}

void Catapult::Insert(Space &space){
	space << arm;
	space << hinge; 
}

void Catapult::Rotate(float theta){
	arm->GetFlatlandObject()->Rotate(theta);
}

Point *Catapult::InsertSouthEastAnchor(Space &space){
	const float offset = Point::Size / 2;
	HenocUniverse::Quad &quad = static_cast<HenocUniverse::Quad&>(arm->GetFlatlandObject()->GetGeometry());
	const vec2* corners = quad.GetCorners();
	vec2 v = corners[2] +  vec2(offset, offset);
	Point *p = new Point(v);
	AppInstance.Anchor(p, v);
	space << p;
	return p;
}

Point *Catapult::InsertSouthWestAnchor(Space &space){
	const float offset = Point::Size / 2;
	HenocUniverse::Quad &quad = static_cast<HenocUniverse::Quad&>(arm->GetFlatlandObject()->GetGeometry());
	const vec2* corners = quad.GetCorners();
	vec2 v = corners[3] + vec2(-offset, offset);
	Point *p = new Point(v);
	AppInstance.Anchor(p, v);
	space << p;
	return p;
}

Point *Catapult::InsertNorthEastAnchor(Space &space){
	const float offset = Point::Size / 2;
	HenocUniverse::Quad &quad = static_cast<HenocUniverse::Quad&>(arm->GetFlatlandObject()->GetGeometry());
	const vec2* corners = quad.GetCorners();
	vec2 v = corners[1] + vec2(offset, -offset);
	Point *p = new Point(v);
	AppInstance.Anchor(p, v);
	space << p;
	return p;
}

Point *Catapult::InsertNorthWestAnchor(Space &space){
	const float offset = Point::Size / 2;
	HenocUniverse::Quad &quad = static_cast<HenocUniverse::Quad&>(arm->GetFlatlandObject()->GetGeometry());
	const vec2* corners = quad.GetCorners();
	vec2 v = corners[0] + vec2(-offset, -offset);
	Point *p = new Point(v);
	AppInstance.Anchor(p, v);
	space << p;
	return p;
}

void Terrain::Insert(Space &space){
	object.GetGeometry().Finalize();
	if (borders){
		Object::PushProperties();
		if (!borderVisibility){
			Object::Default().outlineColor = Color::Transparent;
			Object::Default().fillColor = Color::Transparent;
		}
		InsertSouthBorder(space);
		InsertEastBorder(space);
		InsertWestBorder(space);
		Object::PopProperties();
	}
	Object::Insert(space);
}

void Composite::Insert(Space &space){
	object.GetGeometry().Finalize();
	Object::Insert(space);
}

void Composite::InsertFront(Space &space){
	object.GetGeometry().Finalize();
	Object::InsertFront(space);
}

void HenocWrapper::setOdeW(const Whstc &p){
	world.SetCFM(p.co);
	world.SetAutoDisableFlag(false);
	world.SetAutoDisableLinearThreshold(p.lin);
	world.SetERP(p.erp);
	world.SetContactMaxCorrectingVel(p.cmv);
	world.SetContactSurfaceLayer(p.fcs);
	world.SetGravity(vec2(0,p.gravity));
}

// Public, minimal API wrappers for external code (Qt widget) ------------------
namespace HenocUniverseI {
    void HU_Init(){
        static HenocWrapper instance; // constructs and sets singleton
        (void)instance;
        AppInstance.InitGl();
        AppInstance.InitOde();
    }

    void HU_Resize(int w, int h){
        AppInstance.Resize(w, h);
    }

    void HU_SetWorldParams(const Whstc &p){
        AppInstance.setOdeW(p);
    }

    void HU_ClearSpace(){
        // delete and clear all objects in space
        for (Space::iterator it = AppInstance.space.begin(); it != AppInstance.space.end(); ++it)
            delete *it;
        AppInstance.space.clear();
    }

    static Color makeFill(float theta){ Color c(theta); c.IncreaseBrightness(0.5f); return c; }
    static Color makeStroke(float theta){ return Color(theta); }

    void HU_AddBox(int cx, int cy, int w, int h, float mass, float friction, float bounceFactor, float bounceVelocity, int colMask, int frictionMask, float rotationDeg, float thetaColor){
        Block *b = new Block(vec2((float)cx, (float)cy), (float)w, (float)h);
        b->GetFlatlandObject()->Property().density = std::max(0.001f, mass);
        b->GetFlatlandObject()->Property().friction = friction;
        b->GetFlatlandObject()->Property().bounceFactor = bounceFactor;
        b->GetFlatlandObject()->Property().bounceVelocity = bounceVelocity;
        b->GetFlatlandObject()->Property().collisionMask = (HenocUniverse::Mask)colMask;
        b->GetFlatlandObject()->Property().frictionMask = (HenocUniverse::Mask)frictionMask;
        b->Property().fillColor = makeFill(thetaColor);
        b->Property().outlineColor = makeStroke(thetaColor);
        b->GetFlatlandObject()->Rotate(rotationDeg);
        b->Insert(AppInstance.space);
    }

    void HU_AddBall(int cx, int cy, int r, float mass, float friction, float bounceFactor, float bounceVelocity, int colMask, int frictionMask, float rotationDeg, float thetaColor){
        Ball *ball = new Ball(vec2((float)cx, (float)cy), (float)r);
        ball->GetFlatlandObject()->Property().density = std::max(0.001f, mass);
        ball->GetFlatlandObject()->Property().friction = friction;
        ball->GetFlatlandObject()->Property().bounceFactor = bounceFactor;
        ball->GetFlatlandObject()->Property().bounceVelocity = bounceVelocity;
        ball->GetFlatlandObject()->Property().collisionMask = (HenocUniverse::Mask)colMask;
        ball->GetFlatlandObject()->Property().frictionMask = (HenocUniverse::Mask)frictionMask;
        ball->Property().fillColor = makeFill(thetaColor);
        ball->Property().outlineColor = makeStroke(thetaColor);
        ball->GetFlatlandObject()->Rotate(rotationDeg);
        ball->Insert(AppInstance.space);
    }

    void HU_AddLine(int x1, int y1, int x2, int y2, float friction, int colMask, int frictionMask, float thetaColor){
        Line *ln = new Line(vec2((float)x1, (float)y1), vec2((float)x2, (float)y2));
        // Set visual properties
        ln->Property().outlineThickness = 2;
        ln->Property().outlineColor = makeStroke(thetaColor);
        // Physical surface properties live on the flatland object
        ln->GetFlatlandObject()->Property().friction = friction;
        ln->GetFlatlandObject()->Property().collisionMask = (HenocUniverse::Mask)colMask;
        ln->GetFlatlandObject()->Property().frictionMask = (HenocUniverse::Mask)frictionMask;
        ln->Insert(AppInstance.space);
    }

    void HU_StepAndDraw(float delta){
        AppInstance.Step(delta);
        AppInstance.Draw();
    }
}

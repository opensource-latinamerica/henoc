/*
 * TT102 HENOC 
 * 2008 @ ESCOM-IPN
 * Implementacion de HenocUniverse
 */

#ifndef __HENOC_HENOCUNIVERSEI_H__
#define __HENOC_HENOCUNIVERSEI_H__


#include <ode/ode.h>
#include <shapes.h>
#include <henocUniverse.h>
#include <list>
#include <stack>

using HenocUniverse::vec2;
using HenocUniverse::Dynamic;
using HenocUniverse::Static;
using HenocUniverse::round;

namespace HenocUniverseI{
	class Space;
    
	typedef unsigned int Texture;
	struct RawColor { float r, g, b, a; };

	//vector RGBA. 
	struct Color {
		Color() {}
		Color(float theta);
		void IncreaseBrightness(float factor); 
		void DecreaseBrightness(float factor);
		static Color Blend(const Color &a, const Color &b, float mu);
		Color(RawColor raw) : raw(raw) {}
		operator RawColor() { return raw; }
		operator RawColor() const { return raw; }
		Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
		Color(const float *f) : r(f[0]), g(f[1]), b(f[2]), a(f[3]) {}
		union{
			struct { float r, g, b, a; };
			RawColor raw;
		};
		static Color Transparent, Black, White, PaleYellow, PaleGreen, PaleRed, Red, Green, Blue;
	};

	struct Whstc{
		int fAnim;
		float delta;
		int cmv;
		float fcs;
		float gravity;
		float erp;
		float lin;
		int co;
	};

	//Propiedades visuales.
	struct ObjectProperties{
		float outlineThickness;
		RawColor fillColor;
		RawColor altFillColor;
		RawColor outlineColor;
		bool offset;
	};


	class Object{
		public:
			Object() : properties(defaults) {}
			virtual ~Object() {}
			virtual void Draw() const = 0;
			virtual HenocUniverse::Object *GetFlatlandObject() { return 0; }
			virtual void Insert(Space &space);
			virtual void InsertFront(Space &space);
		public:
			ObjectProperties &Property() { return properties; }
			const ObjectProperties &Property() const { return properties; }
			static ObjectProperties &Default() { return defaults; }
			static void PushProperties() { defaultStack.push(defaults); }
			static void PopProperties() { defaults = defaultStack.top(); defaultStack.pop(); }
		private:
			ObjectProperties properties;
			static ObjectProperties defaults;
			static std::stack<ObjectProperties> defaultStack;
	};


	class Block : public Object{
		public:
			Block(vec2 center, float width, float height);
			void Draw() const;
			HenocUniverse::Object *GetFlatlandObject() { return &object; }
		private:
			Dynamic<HenocUniverse::Block> object;
	};

	class Wall : public Object{
		public:
			Wall(vec2 center, float width, float height);
			Wall(float left, float top, float right, float bottom);
			void Draw() const;
			HenocUniverse::Object *GetFlatlandObject() { return &object; }
		private:
			Static<HenocUniverse::Block> object;
	};


	class Line : public Object{
		public:
			Line(vec2 a, vec2 b);
			void Draw() const;
			HenocUniverse::Object *GetFlatlandObject() { return &object; }
		private:
			Static<HenocUniverse::Line> object;
	};

	class RopeSegment : public Object{
		public:
			RopeSegment(vec2 a, vec2 b);
			void Draw() const {}
			vec2 Origin() const { return object.GetGeometry().Origin(); }
			vec2 End() const { return object.GetGeometry().End(); }
			HenocUniverse::Object *GetFlatlandObject() { return &object; }
		private:
			Dynamic<HenocUniverse::Line> object;
	};

	typedef std::vector<RopeSegment*> RopeVector;

	class Rope : public Object{
		public:
			Rope(vec2 start, vec2 end, float length, float tautness);
			RopeSegment *front() { return segments.front(); }
			RopeSegment *back() { return segments.back(); }
			void Draw() const;
			void Terrainify(float bottom);
			void Insert(Space &space);
		private:
			vec2 start, end;
			RopeVector segments;
			float bottom;
			bool terrain;
	};

	class Terrain : public Object{
		public:
			Terrain(vec2 start, float bottom, bool borders = false, bool borderVisibility = true);
			void Add(const vec2 &v);
			void Insert(Space &space);
			void Draw() const;
			HenocUniverse::Object *GetFlatlandObject() { return &object; }
			Line *InsertWestBorder(Space &space) const;
			Line *InsertEastBorder(Space &space) const;
			Line *InsertSouthBorder(Space &space) const;
		private:
			Static<HenocUniverse::Terrain> object;
			float bottom;
			bool borders;
			bool borderVisibility;
	};

	class Ball : public Object{
		public:
			Ball(vec2 center, float radius);
			void Draw() const;
			HenocUniverse::Object *GetFlatlandObject() { return &object; }
		private:
			Dynamic<HenocUniverse::Circle> object;
	};

	class Point : public Object{
		public:
			Point(vec2 center);
			void Draw() const;
			HenocUniverse::Object *GetFlatlandObject() { return &object; }
			static const float Size;
		private:
			Dynamic<HenocUniverse::Circle> object;
	};

	class Wheel : public Object{
		public:
			Wheel(vec2 center, float radius);
			void Draw() const;
			void SetTexture(Texture t) { texture = t; }
			HenocUniverse::Object *GetFlatlandObject() { return &object; }
		private:
			Dynamic<HenocUniverse::Circle> object;
			Texture texture;
	};

	class Catapult : public Object{
		public:
			Catapult(vec2 center, float width, float height, vec2 hinge);
			void Rotate(float theta);
			Point *InsertSouthEastAnchor(Space &space);
			Point *InsertSouthWestAnchor(Space &space);
			Point *InsertNorthEastAnchor(Space &space);
			Point *InsertNorthWestAnchor(Space &space);
			void Draw() const {}
			void Insert(Space &space);
		private:
			Point *hinge;
			Block *arm;
	};

	class Composite : public Object{
		public:
			Composite(vec2 centroid);
			void Draw() const;
			HenocUniverse::Object *GetFlatlandObject() { return &object; }
			virtual void Insert(Space &space);
			virtual void InsertFront(Space &space);
			void Add(HenocUniverse::Geometry *geometry);
		private:
			Dynamic<HenocUniverse::Composite> object;
	};

	class Beam : public Composite{
		public:
			Beam(vec2 start, vec2 end, float thickness);
			void Insert(Space &space);
			void InsertFront(Space &space);
		private:
		float theta;
	};

	typedef std::list<Object*> OList;

	class Space{
		public:
			~Space();
			size_t size() const { return list.size(); }
			Space &operator<<(Object *o) { list.push_back(o); return *this; }
			typedef OList::const_iterator const_iterator;
			const_iterator begin() const { return list.begin(); }
			const_iterator end() const { return list.end(); }
			void pop() { delete *m; m = list.erase(m); }
			void push_back(Object *o) { list.push_back(o); }
			void push_front(Object *o) { list.push_front(o); }
			void mark() { m = --list.end(); }
			void destruye(){
				for (m = list.begin(); m != list.end(); ++m)
					delete *m;
				list.clear();
			}
		private:
			OList list;
			OList::iterator m;
	};

	void DrawContact(const dContact &contact);

	class HenocWrapper{
		public:
			HenocWrapper();
			void InitGl();
			void chingatelo(){ space.destruye(); }
			void InitOde();
			void setOdeW(const Whstc &);
			virtual void Draw() const;
			Space &GetSpace() { return space; }
			HenocUniverse::World &GetWorld() { return world; }
			void Clear() const;
			void DrawScene() const;
			void Step(float delta);
			void SetTarget(const vec2 &target) { velocity = (target - center) / 100; }
			void UpdateCenter() { center += velocity; }
			void Resize(int width, int height);
			vec2 Center() const { return center; }
			void Zoom(float factor);
			dJointID Glue(Object *a, Object *b);
			dJointID Anchor(Object *a, Object *b, vec2 p, float mu = 0, float erp = 0.01);
			dJointID Anchor(Object *a, vec2 p, float mu = 0, float erp = 0.01);
			dJointID AnchorAxis(Object *a, vec2 axis);
			static Texture LoadTexture(int width, int height, const void *data);
			static Texture LoadTexture(const char *filename);
			static int TextureWidth(Texture id);
			static int TextureHeight(Texture id);
			static void GetImageData(const char *filename, void *data);
			static void GetImageSize(const char *filename, int &width, int &height);
			static void Blit(Texture id, float left, float top, Color color = Color::White, bool flip = false);
			static HenocWrapper &GetSingleton();
		protected:
			Space space;
			HenocUniverse::World world;
			vec2 center;
			vec2 velocity;
			vec2 target;
			static HenocWrapper *singleton;
			float zoom;
			int width, height;
	};

	void glColor(const Color &color);
	void glVertex(const vec2 &v);

	void Draw(const HenocUniverse::Geometry &g, const ObjectProperties &properties);
	void DrawCircle(const HenocUniverse::Geometry &g, const ObjectProperties &properties);
	void DrawQuad(const HenocUniverse::Geometry &g, const ObjectProperties &properties);
}


#endif

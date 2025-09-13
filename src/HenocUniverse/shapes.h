/*
 * TT102 HENOC 
 * 2008 @ ESCOM-IPN
 *
 */
#ifndef _HENOC_SHAPES_H
#define _HENOC_SHAPES_H

#include <henocUniverse.h>
#include <vector.h>
#include <vector>
#include <list>
#include <map>

namespace HenocUniverse{
	class Quad : public Geometry{
		public:
			Quad(dReal width, dReal height);
			void UpdateBounds();
			void SetMass(Body body, dReal density) const;
			Shape GetShape() const { return Shape::Quad; }
			virtual dReal Extent(int i) const { return extent[i]; }
			vec2 EAxis(int i) const { return Axis(i) * extent[i]; } 
			const vec2* GetCorners() const { return corners; }
			bool Contains(const vec2 &v) const;
		protected:
			dReal extent[2];
			vec2 corners[4];
	};

	class Block : public Quad{
		public:
			Block(vec2 topleft, vec2 bottomright);
			Block(vec2 center, dReal width, dReal height);
			Block(dReal left, dReal top, dReal right, dReal bottom);
	};

	class Line : public Quad{
		public:
			Line(vec2 origin, vec2 end);
			vec2 Origin() const { return center - axis * extent[0]; } 
			vec2 End() const { return center + axis * extent[0]; }
	};

	typedef std::vector<vec2> VList;
	typedef std::vector<Line> LineStrip;
	typedef std::map<dReal, int> SpanMap;

	class Terrain : public Geometry{
		public:
			Terrain(const vec2 &start);
			void UpdateBounds();
			void Finalize() { UpdateBounds(); }
			Shape GetShape() const { return Shape::Terrain; }
			bool GetIndexRange(dReal left, dReal right, int &lower, int &upper) const;

			typedef VList::const_iterator const_iterator;
			const_iterator begin() const { return vertices.begin(); }
			const_iterator end() const { return vertices.end(); }
			const vec2 &front() const { return vertices.front(); }
			const vec2 &back() const { return vertices.back(); }
			void push_back(const vec2 &v);
			size_t size() const { return lines.size(); }
			bool empty() const { return lines.empty(); }
			Line &operator[](int i) { return lines[i]; }
			const Line &operator[](int i) const { return lines[i]; }
		private:
			vec2 previous;
			LineStrip lines;
			SpanMap spans;
			VList vertices;
	};

	class Circle : public Geometry{
		public:
			Circle(const vec2 &center, dReal radius);
			void UpdateBounds();
			void SetMass(Body body, dReal density) const;
			Shape GetShape() const { return Shape::Circle; }
			dReal Radius() const { return radius; }
		private:
			dReal radius;
	};

	typedef std::list<Geometry*> GList;

	class Composite : public Geometry{
		public:
			Composite(vec2 centroid);
			~Composite();
			void UpdateBounds();
			void Finalize() { UpdateBounds(); }
			Shape GetShape() const { return Shape::Composite; }
			void SetCenter(const vec2 &center);
			void SetAxis(const vec2 &axis);
			void SetMass(Body body, dReal density) const;
			typedef GList::const_iterator const_iterator;
			typedef GList::iterator iterator;
			const_iterator begin() const { return geometries.begin(); }
			const_iterator end() const { return geometries.end(); }
			const Geometry &front() const { return *geometries.front(); }
			const Geometry &back() const { return *geometries.back(); }
			void push_back(Geometry *geometry);
			size_t size() const { return geometries.size(); }
		private:
			GList geometries;
	};
}

#endif

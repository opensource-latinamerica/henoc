/*
 * TT102 HENOC 
 * 2008 @ ESCOM-IPN
 *
 */

#ifndef _HENOC_ENUMS_H
#define _HENOC_ENUMS_H

namespace HenocUniverse{
	static const char *ShapeTable[] ={
		"Quad",
		"Line",
		"Circle",
		"Terrain",
		"Composite",
	};

	struct Shape{
		enum EShape{
			Quad = 0,
			Line = 1,
			Circle = 2,
			Terrain = 3,
			Composite = 4,
			Count = 5,
			Invalid = 5,
		};

		union{
			EShape name;
			unsigned int value;
		};

		Shape(EShape name) : name(name) {}
		Shape(unsigned int value) : value(value) {}
		Shape() : value(Invalid) {}
		operator EShape() const { return name; }
		unsigned int operator++() { return ++value; }
		unsigned int operator++(int) { return value++; }
		unsigned int operator--() { return --value; }
		unsigned int operator--(int) { return value--; }
		const char *GetString() const { return ShapeTable[value]; }
	};
}

#endif

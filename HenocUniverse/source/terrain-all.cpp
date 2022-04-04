/*
 * TT102 HENOC 
 * 2008 @ ESCOM-IPN
 * Intersecciones circulo-circulo y generador de contactos
 */

#include <shapes.h>
#include <intersection.h>

namespace{
	int lower, upper;
}

using namespace HenocUniverse;

bool TestTerrainGeometry(const Geometry &g1, const Geometry &g2){
	assert(g1.GetShape() == Shape::Terrain);
	const Terrain &t = static_cast<const Terrain&>(g1);
	const Geometry &g = g2;

	if (!t.GetIndexRange(g.GetBounds().left, g.GetBounds().right, lower, upper))
		return false;

	for (int index = lower; index <= upper; ++index){
		assert(index >= 0 && index < (int) t.size());
		const Line &line = t[index];
		if (Intersection::Test(line, g)) return true;
	}

	return false;
}

void FindTerrainGeometry(const Geometry &g1, const Geometry &g2, ContactList &contacts){
	assert(g1.GetShape() == Shape::Terrain);
	const Terrain &t = static_cast<const Terrain&>(g1);
	const Geometry &g = g2;

	for (int index = lower; index <= upper; ++index){
		const Line &line = t[index];
		if (Intersection::Test(line, g)) 
			Intersection::Find(line, g, contacts);
	}
}

void Intersection::FindTerrainQuad(const Geometry &g1, const Geometry &g2, ContactList &contacts){
	FindTerrainGeometry(g1, g2, contacts);
}

void Intersection::FindTerrainCircle(const Geometry &g1, const Geometry &g2, ContactList &contacts){
	FindTerrainGeometry(g1, g2, contacts);
}

void Intersection::FindTerrainTerrain(const Geometry &g1, const Geometry &g2, ContactList &contacts){
	FindTerrainGeometry(g1, g2, contacts);
}

void Intersection::FindQuadTerrain(const Geometry &g1, const Geometry &g2, ContactList &contacts){
	contacts.ToggleNormalInversion();
	FindTerrainGeometry(g2, g1, contacts);
	contacts.ToggleNormalInversion();
}

void Intersection::FindCircleTerrain(const Geometry &g1, const Geometry &g2, ContactList &contacts){
	contacts.ToggleNormalInversion();
	FindTerrainGeometry(g2, g1, contacts);
	contacts.ToggleNormalInversion();
}

bool Intersection::TestTerrainQuad(const Geometry &g1, const Geometry &g2){
	return TestTerrainGeometry(g1, g2);
}

bool Intersection::TestTerrainCircle(const Geometry &g1, const Geometry &g2){
	return TestTerrainGeometry(g1, g2);
}

bool Intersection::TestTerrainTerrain(const Geometry &g1, const Geometry &g2){
	return TestTerrainGeometry(g1, g2);
}

bool Intersection::TestQuadTerrain(const Geometry &g1, const Geometry &g2){
	return TestTerrainGeometry(g2, g1);
}

bool Intersection::TestCircleTerrain(const Geometry &g1, const Geometry &g2){
	return TestTerrainGeometry(g2, g1);
}



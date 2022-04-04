/*
 * TT102 HENOC 
 * 2008 @ ESCOM-IPN
 *
 */
#ifndef _HENOC_INTERSECTION_H
#define _HENOC_INTERSECTION_H


namespace HenocUniverse{
	class Geometry;
	class ContactList;
	class Quad;
	class Circle;
	class Terrain;

	namespace Intersection{
		void Find(const Geometry &g1, const Geometry &g2, ContactList &contacts);

		void FindQuadQuad(const Geometry &g1, const Geometry &g2, ContactList &contacts);
		void FindQuadCircle(const Geometry &g1, const Geometry &g2, ContactList &contacts);
		void FindQuadTerrain(const Geometry &g1, const Geometry &g2, ContactList &contacts);
		void FindQuadComposite(const Geometry &g1, const Geometry &g2, ContactList &contacts);

		void FindCircleQuad(const Geometry &g1, const Geometry &g2, ContactList &contacts);
		void FindCircleCircle(const Geometry &g1, const Geometry &g2, ContactList &contacts);
		void FindCircleTerrain(const Geometry &g1, const Geometry &g2, ContactList &contacts);
		void FindCircleComposite(const Geometry &g1, const Geometry &g2, ContactList &contacts);

		void FindTerrainQuad(const Geometry &g1, const Geometry &g2, ContactList &contacts);
		void FindTerrainCircle(const Geometry &g1, const Geometry &g2, ContactList &contacts);
		void FindTerrainTerrain(const Geometry &g1, const Geometry &g2, ContactList &contacts);
		void FindTerrainComposite(const Geometry &g1, const Geometry &g2, ContactList &contacts);

		void FindCompositeQuad(const Geometry &g1, const Geometry &g2, ContactList &contacts);
		void FindCompositeCircle(const Geometry &g1, const Geometry &g2, ContactList &contacts);
		void FindCompositeTerrain(const Geometry &g1, const Geometry &g2, ContactList &contacts);
		void FindCompositeComposite(const Geometry &g1, const Geometry &g2, ContactList &contacts);

		bool Test(const Geometry &g1, const Geometry &g2);

		bool TestQuadQuad(const Geometry &g1, const Geometry &g2);
		bool TestQuadCircle(const Geometry &g1, const Geometry &g2);
		bool TestQuadTerrain(const Geometry &g1, const Geometry &g2);
		bool TestQuadComposite(const Geometry &g1, const Geometry &g2);

		bool TestCircleQuad(const Geometry &g1, const Geometry &g2);
		bool TestCircleCircle(const Geometry &g1, const Geometry &g2);
		bool TestCircleTerrain(const Geometry &g1, const Geometry &g2);
		bool TestCircleComposite(const Geometry &g1, const Geometry &g2);

		bool TestTerrainQuad(const Geometry &g1, const Geometry &g2);
		bool TestTerrainCircle(const Geometry &g1, const Geometry &g2);
		bool TestTerrainTerrain(const Geometry &g1, const Geometry &g2);
		bool TestTerrainComposite(const Geometry &g1, const Geometry &g2);

		bool TestCompositeQuad(const Geometry &g1, const Geometry &g2);
		bool TestCompositeCircle(const Geometry &g1, const Geometry &g2);
		bool TestCompositeTerrain(const Geometry &g1, const Geometry &g2);
		bool TestCompositeComposite(const Geometry &g1, const Geometry &g2);
	}
}

#endif

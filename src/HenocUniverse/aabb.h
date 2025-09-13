/*
 * TT102 HENOC 
 * 2008 @ ESCOM-IPN
 *
 */
#ifndef _HENOC_AABB_H
#define _HENOC_AABB_H

#include <ode/ode.h>

namespace HenocUniverse{
	struct aabb{
		aabb() {}
		aabb(dReal left, dReal top, dReal right, dReal bottom) :
			left(left), top(top), right(right), bottom(bottom) {}
		bool intersects(const aabb &x) const{
			return ((left < x.right && x.left < right) && (top < x.bottom && x.top < bottom)); 
		}
		dReal left;
		dReal top;
		dReal right;
		dReal bottom;
	};
}

#endif

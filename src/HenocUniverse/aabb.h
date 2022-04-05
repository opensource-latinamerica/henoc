/*
 * TT102 HENOC 
 * 2008 @ ESCOM-IPN
 *
 */
#ifndef _HENOC_AABB_H
#define _HENOC_AABB_H

namespace HenocUniverse{
	struct aabb{
		aabb() {}
		aabb(float left, float top, float right, float bottom) :
			left(left), top(top), right(right), bottom(bottom) {}
		bool intersects(const aabb &x) const{
			return ((left < x.right && x.left < right) && (top < x.bottom && x.top < bottom)); 
		}
		float left;
		float top;
		float right;
		float bottom;
	};
}

#endif

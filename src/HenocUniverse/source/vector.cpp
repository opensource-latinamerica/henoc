/*
 * TT102 HENOC 
 * 2008 @ ESCOM-IPN
 * Define vec2
 */

#include <vector.h>
#include <math.h>

using namespace HenocUniverse;

const float d2r = pi / 180;

vec2 vec2::rotate(float d) const { return rotate(vec2(cosf(d2r * d), sinf(d2r * d))); }
float vec2::length() const { return sqrtf(dot(*this, *this)); }

bool HenocUniverse::is_nan(float f){
	unsigned long bits = *((unsigned long*) &f);

	if (bits >= 0xffc00001 && bits <= 0xffffffff) return true;

	if (bits == 0xffc00000) return true;

	if (bits >= 0xff800001 && bits <= 0xffbfffff) return true;

	if (bits == 0xff800000) return true;

	if (bits == 0x7f800000) return true;

	if (bits >= 0x7f800001 && bits <= 0x7fbfffff) return true;

	if (bits >= 0x7fc00000 && bits <= 0x7fffffff) return true;

	return false;
}

float HenocUniverse::round(float f){
	return (float) ((int) (f + 0.5f));
}

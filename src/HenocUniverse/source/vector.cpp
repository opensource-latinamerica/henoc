/*
 * TT102 HENOC 
 * 2008 @ ESCOM-IPN
 * Define vec2
 */

#include "vector.h"
#include <cmath>

using namespace HenocUniverse;

const dReal d2r = pi / 180;

vec2 vec2::rotate(dReal d) const { return rotate(vec2(cos(d2r * d), sin(d2r * d))); }
dReal vec2::length() const { return sqrt(dot(*this, *this)); }

bool HenocUniverse::is_nan(dReal f){
    return std::isnan(f);
}

dReal HenocUniverse::round(dReal f){
    return std::round(f);
}

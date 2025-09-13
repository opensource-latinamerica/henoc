/*
 * TT102 HENOC 
 * 2008 @ ESCOM-IPN
 * Implementacion de HenocUniverse
 */
 

#ifndef HENOCUNIVERSEI_H
#define HENOCUNIVERSEI_H

/*
 * This header acts as an intermediary to include the ODE library headers.
 * It resolves a name collision for the 'round' function which exists in both
 * the standard C++ library and the ODE library. This is a common issue when
 * integrating older C libraries with modern C++ code.
 */

// The ODE library can conflict with the standard C++ library over the
// definition of 'round'. To prevent a compilation error, we undefine
// 'round' before including the ODE header.
#ifdef round
#undef round
#endif

#include <ode/ode.h>
#include "henocUniverse.h"

#endif // HENOCUNIVERSEI_H

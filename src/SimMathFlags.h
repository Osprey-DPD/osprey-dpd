// Header file to provide access to platform-dependent libraries.
//
// This file must be included in all .cpp files that use conditional compilation
// to access the <cmath> library.
//
// **********************************************************************
//
// Version History
// ***************
//
//	29/8/03     Baseline version.
//  13/7/04     I added a new platform for the Code Warrior IDE 5.5 on Mac OS X 
//              and it needs math.h not cmath.
//  04/05/06    I copied the CW55MAC flags to XCMAC.
//
// **********************************************************************

	#include <math.h>

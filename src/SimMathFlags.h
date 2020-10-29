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

#if Platform == DECALPHA 
	#include <math.h>
#elif Platform == SGICC
	#include <math.h>
#elif Platform == CRAYJ90
	#include <math.h>
#elif Platform == BORLAND6
	#include <math>
#elif Platform == I7XEON
	#include <cmath>
#elif Platform == I7ITANIUM
#elif Platform == GCC
#elif Platform == CW55MAC
	#include <math.h>
#elif Platform == XCMAC
	#include <math.h>
#elif Platform == NEWPLATFORM1
#elif Platform == NEWPLATFORM2
#elif Platform == NEWPLATFORM3
#elif Platform == NEWPLATFORM4
#elif Platform == NEWPLATFORM5
#elif Platform == NEWPLATFORM6
#elif Platform == NEWPLATFORM7
#elif Platform == NEWPLATFORM8
#elif Platform == NEWPLATFORM9
#elif Platform == NEWPLATFORM10
#else							
	#include <cmath>			
#endif					

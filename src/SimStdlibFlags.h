// Header file to provide access to platform-dependent libraries.
//
// This file must be included in all .cpp files that use conditional compilation
// to access the STL <cstdlib> classes.
//
// **********************************************************************
//
// Version History
// ***************
//
//	29/8/03     Baseline version.
//  04/05/06   I copied the CW55MAC flags to XCMAC.
//
// **********************************************************************

#if Platform == DECALPHA 
	#include <stdlib.h>
#elif Platform == SGICC
	#include <stdlib.h>
#elif Platform == CRAYJ90
	#include <stdlib.h>
#elif Platform == BORLAND6
	#include <stdlib>
#elif Platform == I7XEON
#elif Platform == I7ITANIUM
#elif Platform == GCC
#elif Platform == CW55MAC
	#include <stdlib.h>
#elif Platform == XCMAC
	#include <stdlib.h>
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
	#include <cstdlib>
#endif					


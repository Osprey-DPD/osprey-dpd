// Header file to provide access to platform-dependent libraries.
//
// This file must be included in all .cpp files that use conditional compilation
// to access the STL <functional> classes.
//
// **********************************************************************
//
// Version History
// ***************
//
//	29/8/03    Baseline version.
//  04/05/06   I copied the CW55MAC flags to XCMAC.
//
// **********************************************************************

#if Platform == DECALPHA 
	#include <functional>
#elif Platform == SGICC
#elif Platform == CRAYJ90
#elif Platform == BORLAND6
	#include <functional>
#elif Platform == I7XEON
#elif Platform == I7ITANIUM
#elif Platform == GCC
#elif Platform == CW55MAC
	#include <functional>
#elif Platform == XCMAC
	#include <functional>
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
	#include <functional>
#endif					


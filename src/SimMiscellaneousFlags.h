// Header file to hold flags used to exclude miscellaneous classes from a build.
//
// This file must be included in all classes whose code inclusion/exclusion
// is to be toggled using release-dependent flags.
//
// **********************************************************************
//
// Version History
// ***************
//
//	20/3/06    Baseline version.
//  04/05/06   I copied the CW55MAC flags to XCMAC.
//  04/05/10   I added a flag to toggle the calculation of the stress tensor in non-cartesian coordinate systems.
// **********************************************************************

#define SimMiscEnabled	1
#define SimMiscDisabled	2

#if Platform   == DECALPHA 
#elif Platform == SGICC
#elif Platform == CRAYJ90
#elif Platform == BORLAND6
#elif Platform == I7XEON
	#define EnableMiscClasses               SimMiscEnabled
#elif Platform == I7ITANIUM
#elif Platform == GCC
	#define EnableMiscClasses               SimMiscEnabled
#elif Platform == CW55MAC
	#define EnableMiscClasses               SimMiscEnabled
#elif Platform == XCMAC
	#define EnableMiscClasses               SimMiscEnabled
	#define EnableStressTensorSphere        SimMiscDisabled
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
	#define EnableMiscClasses               SimMiscEnabled

#endif					


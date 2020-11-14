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

	#define EnableMiscClasses               SimMiscEnabled
	#define EnableStressTensorSphere        SimMiscDisabled


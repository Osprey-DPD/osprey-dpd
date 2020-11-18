// Header file to hold flags and options relating to the commands.
//
// This file must be included in all commands whose inclusion/exclusion
// is to be toggled using release-dependent flags.
//
// **********************************************************************
//
// Version History
// ***************
//
//	13/2/06    Baseline version.
//             I defined the EnableMonitorCommand flag only for now.
//  14/2/06    I added the EnableProcessCommand flag.
//  27/3/06    I added the EnableTargetCommand flag to allow toggling the 
//             inclusion of all ctXXX command implementations.
//  28/3/06    I removed the EnableCommandDocumentCommand flag as its
//             behaviour is implemented by the EnableCommandDocumentProcess flag.
//  10/4/06    I created a new flag EnableConstraintCommand to allow removal
//             of some constraint commands, such as ccFreezeBeadsInSlice
//             that are not removed by any other flags.
//  04/05/06   I copied the CW55MAC flags to XCMAC.
//  06/07/06   I added a new flag to allow commands that are normally inherited
//             using virtual inheritance to be implemented directory in the SimBox.
//             This is needed when compiling using gcc or Intel 9.1 on linux.
//             For W2K and mac, this flag must be disabled. Note that if the CW55MAC
//             flag is enabled for compiling on linux, the flag must be enabled.
//  15/01/08   I added a new flag to control compilation of the command groups feature.
//
// **********************************************************************

#define SimCommandEnabled	1
#define SimCommandDisabled	2

	#define EnableACNCommand               SimCommandEnabled
	#define EnableCommandGroups            SimCommandEnabled
	#define EnableConstraintCommand        SimCommandEnabled
	#define EnableMonitorCommand           SimCommandEnabled
	#define EnableProcessCommand           SimCommandEnabled
	#define EnableTargetCommand            SimCommandEnabled
	#define EnableDirectImplCommand        SimCommandDisabled


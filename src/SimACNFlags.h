// Header file to hold flags and options relating to the Active Cell Networks
// and shadow SimBox. 
//
// This file must be included in all .h and .cpp files that refer to any of 
// the shadow SimBox-related classes. Note that because it is used inside
// other header files, it should NOT contain any further #include statements.
//
// **********************************************************************
//
// Version History
// ***************
//
//	10/2/06    Baseline version.
//             The flag EnableShadowSimBox must be set to allow any active
//             cell network functionality, and for any of the other
//             flags to have effect. 
//  04/05/06   I copied the CW55MAC flags to XCMAC.
//  01/12/08   I disabled the ACN features for the demo code for Karl Travis 
//             (changelists 1633, 1634)
//  03/12/08   I re-enabled the ACN flags for normal use.
//
// **********************************************************************

#define SimACNEnabled	1
#define SimACNDisabled	2

	#define EnableShadowSimBox    SimACNEnabled
	#define EnableACNCommands     SimACNEnabled
	#define EnableACNProcesses    SimACNEnabled


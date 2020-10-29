// Header file to hold flags and options relating to the parallel code.
//
// This file must be included in all .h and .cpp files that refer to any of 
// the parallelised classes. Note that because it is used inside
// other header files, it should NOT contain any further #include statements.
//
// **********************************************************************
//
// Version History
// ***************
//
//	22/10/07    Baseline version. 
//              General flags relating to the parallel code are in SimDefs.h
//              where they are included in every compilation unit. This 
//              header file contains parallel-aware flags that only need to be
//              included in a limited number of files and, particularly, 
//              header files that need to distinguish between the standard code
//              and the parallelised version.
//  23/10/07    I added a flag (SimMPSSystemShape) showing the shape of the system. 
//              It can take one of the following values for the corresponding number
//              of processors:
//
//              SimMPSLinearShape = 1,   1, LZ
//              SimMPSPlanarShape = LX, LY, 1
//              SimMPSCubicShape  = LX, LY, LZ
//  30/10/07    I added more flags to allow different code features to be
//              toggled between serial and parallel implementations.
//           
// **********************************************************************

#define SimMPSEnabled	1
#define SimMPSDisabled	2

#if Platform   == DECALPHA 
#elif Platform == SGICC
#elif Platform == CRAYJ90
#elif Platform == BORLAND6
#elif Platform == I7XEON
#elif Platform == I7ITANIUM
#elif Platform == GCC
#elif Platform == CW55MAC
	#define EnableParallelACN           SimMPSDisabled
	#define EnableParallelAggregates    SimMPSDisabled
	#define EnableParallelCommands      SimMPSEnabled
	#define EnableParallelEvents        SimMPSDisabled
	#define EnableParallelExperiment    SimMPSEnabled
	#define EnableParallelMonitor       SimMPSEnabled
	#define EnableParallelProcesses     SimMPSDisabled
	#define EnableParallelRestart       SimMPSDisabled
	#define EnableParallelSimBox        SimMPSEnabled
	#define EnableParallelTargets       SimMPSDisabled
#elif Platform == XCMAC
	#define EnableParallelACN           SimMPSDisabled
	#define EnableParallelAggregates    SimMPSDisabled
	#define EnableParallelCommands      SimMPSDisabled
	#define EnableParallelEvents        SimMPSDisabled
	#define EnableParallelExperiment    SimMPSDisabled
	#define EnableParallelMonitor       SimMPSDisabled
	#define EnableParallelProcesses     SimMPSDisabled
	#define EnableParallelRestart       SimMPSDisabled
	#define EnableParallelSimBox        SimMPSDisabled
	#define EnableParallelTargets       SimMPSDisabled
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
	#define EnableParallelACN           SimMPSDisabled
	#define EnableParallelAggregates    SimMPSDisabled
	#define EnableParallelCommands      SimMPSDisabled
	#define EnableParallelEvents        SimMPSDisabled
	#define EnableParallelExperiment    SimMPSDisabled
	#define EnableParallelMonitor       SimMPSDisabled
	#define EnableParallelProcesses     SimMPSDisabled
	#define EnableParallelRestart       SimMPSDisabled
	#define EnableParallelSimBox        SimMPSDisabled
	#define EnableParallelTargets       SimMPSDisabled
#endif					


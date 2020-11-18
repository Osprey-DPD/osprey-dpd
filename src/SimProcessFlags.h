// Header file to hold flags and options relating to the processes.
// Each process that is to be conditionally-compiled must define a
// unique flag here.
//
// This file is include in the xxProcess.h header file so that it is
// automatically visible to all process whose inclusion/exclusion
// is to be toggled using release-dependent flags.
//
// **********************************************************************
//
// Version History
// ***************
//
//	14/2/06    Baseline version.
//             I defined a flag for each process but they are not all implemented yet.
//             EnableCommandDocumentProcess
//             EnableCommandQueueProcess
//             EnableLightFieldProcess
//             EnableProbeFieldProcess
//             EnableBilayerProcess
//             EnableBilayerFusionProcess
//             EnableBilayerRaftProcess
//             EnableBilayerRuptSurfProcess
//             EnableBilayerRuptTensProcess
//             EnableBLMVesicleFusionProcess
//             EnableVesicleFusionProcess
//             EnableVesicleShearProcess
//             EnablefActinProcess
//             EnableForminProcess
//
//  29/03/06   I added a new flag "EnableAnalysisEvents" controlling the 
//             use of evXXX events for specific CAnalysis derived classes. 
//             Currently this is for: evLamellaXXX, evMotorXXX, evVesicleXXX.
//
//             I added a new flag to enable modifiable processes.
//  04/05/06   I copied the CW55MAC flags to XCMAC.
//
// **********************************************************************

#define SimProcessEnabled	1
#define SimProcessDisabled	2

	#define EnableModifiableProcess        SimProcessEnabled
	#define EnableCommandDocumentProcess   SimProcessEnabled
	#define EnableCommandQueueProcess      SimProcessEnabled
	#define EnableLightFieldProcess        SimProcessDisabled
	#define EnableProbeFieldProcess        SimProcessDisabled
	#define EnableBilayerProcess           SimProcessDisabled
	#define EnableBilayerFusionProcess     SimProcessDisabled
	#define EnableBilayerRaftProcess       SimProcessDisabled
	#define EnableBilayerRuptSurfProcess   SimProcessDisabled
	#define EnableBilayerRuptTensProcess   SimProcessDisabled
	#define EnableBLMVesicleFusionProcess  SimProcessEnabled
	#define EnableVesicleFusionProcess     SimProcessDisabled
	#define EnableVesicleShearProcess      SimProcessDisabled
	#define EnablefActinProcess            SimProcessEnabled
	#define EnableForminProcess            SimProcessEnabled
	#define EnableAnalysisEvents           SimProcessDisabled


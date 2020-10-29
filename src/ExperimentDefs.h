// Header file to define global parameters relating to the simulation.
// This file must be included in all .cpp files that use conditional
// compilation to alter the code to be compiled.
//
// **********************************************************************
//
// Version History
// ***************
//
//	19/5/04   Baseline version.
//            I added a flag to allow an experiment to use the protocol
//            document or control data file.
//  28/3/06   I removed the EnableProtocol flag as its behaviour is implemented
//            by EnableXMLProtocol flag.
//  10/10/07  I added a flag showing if a liquid-gas DPD simulation is
//            being performed. If not set, all lgDPD functions are compiled out.
//
// **********************************************************************


// ***********************
// Experiment/Simulation stand alone

#define ExperimentEnabled   1
#define ExperimentDisabled  2

#if Platform   == DECALPHA 
#elif Platform == SGICC
#elif Platform == CRAYJ90
#elif Platform == BORLAND6
#elif Platform == I7XEON
#elif Platform == I7ITANIUM
#elif Platform == GCC
#elif Platform == CW55MAC
	#define EnableDPDLG               ExperimentDisabled
#elif Platform == XCMAC
	#define EnableDPDLG               ExperimentDisabled
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
	#define EnableDPDLG               ExperimentDisabled
#endif					




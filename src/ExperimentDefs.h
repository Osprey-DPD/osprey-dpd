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

	#define EnableDPDLG               ExperimentDisabled




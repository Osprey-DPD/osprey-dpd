// Header file to define global parameters relating to the simulation.
// This file must be included in all .cpp files that use conditional
// compilation to alter the code to be compiled.
//
// **********************************************************************
//
// Version History
// ***************
//
//	 9/7/99		Dimension of simulation box: 
//
//				2	 two-dimensional SimBox
//				3	 three     "       "
//
//	18/8/99		Use TRACE for debug output in Windows but printf otherwise.
//
//  24/8/99     Added SimIdentifier to distinguish DPD, MD, and other types of
//              simulation we may want to do.
//
//              DPD
//              MD
//
//   9/9/99     Added Platform to distinguish between the various kinds.
//              It can have the following values:
//
//				MFC			for a windows MFC version
//				CONSOLE		for Windows console application
//				UNIX		for unix/linux application
//              CRAYJ90     for the Juelich KFA Cray machines
//
// 2/11/99      Defined UNIX identifier. Note that the pragma warning should
//              be commented out for non-Windows platforms.
//
// 26/1/00      The statements testing for a difference between SimIdentifier
//              do not seem to work. It always evaluates to DPD. Perhaps they
//              have to be given numberical values to be used in an == expression.
//
//  8/3/00      I added a new platform identifier for the Cray machine.
//
//  3/4/00      I added a variable to distinguish between the original version
//              of RandomBuilder.cpp and the version that incorporates the
//              check on the initial bead coordinates. This is to allow a 
//              simple comparison with the standard 050 data. When the variable
//              PreRandomBuilderSimBoxCheck is defined the old code is produced.
//
//	13/4/00		I used the PreRandomBuilderSimBoxCheck flag to isolate the
//				use of fabs() in CRandomBuilder. This was causing the standard
//				050 results to differ because the old abs() funtion used an integer
//				argument.
//
//   8/5/00     I replaced the platform identifier UNIX with different ones for
//              each type of machine: DECALPHA, SGICC and CRAYJ90 .
//  19/6/00     I removed SimDefs.h from source control so that I can change the
//				build type from MD to DPD without having to check it in and out.
//
//				I added a flag, PreVelocity3dNormalisation, to use the old, wrong, 
//				velocity normalisation.
//	22/6/00     I added a flag, UseDPDBeadRadii, to allow the interaction radius of
//				DPD beads to be explicitly set for each bead type. This uses 
//				overriding functions and conditional compilation to access the 
//				radius so that the original functions and code are still available. 
//				If the flag is not set, the original bead types (without a radius) 
//				are used.
//
//  25/9/00     I moved the flag PreRandomBuilderSimBoxCheck to the implementation file
//				RandomBuilder.cpp, and PreVelocity3dNormalisation to the files
//				Builder.cpp and RandomBuilder.cpp. They are only used in these files
//				to recreate previous versions of the code for testing purposes,
//				and this avoids having to recompile the whole code when testing
//				the random initial configuration runs.
//				
//  22/8/02     I added the flag SAXProtocol to allow the SAXProtocolHandler to 
//				inherit from either SAX 1.0 or SAX 2.0 handlers.
//
//	29/8/03     I added new platforms for the Borland 6 compiler, Dell Xeon cluster,
//				and the GCC compiler.
//
//  13/7/04     I added new platforms for the Code Warrior compiler on Mac OS X 
//				and the Mac XCode developer toos. I also added 10 generic new 
//				platform identifiers to ease future platform additions.
//
//  03/5/06     I added a new simulation type "BD" for Brownian Dynamics simulations.
//
//  17/10/07    I added a new flag to allow the parallel version of DPD to 
//              be conditionally-compiled in: SimMPS.
//
// **********************************************************************

// True/false flags used to distinguish binary compilation options.

#define SimulationEnabled   1
#define SimulationDisabled  2

// ***********************
// Simulation type

#define DPD	1
#define MD  2
#define BD  3

#define SimIdentifier  DPD


// ***********************
// Simulation dimension

#define SimDimension	3

// ***********************
// Parallel implementation using MPI. We put the include statement here so that it
// is included in every compilation unit if needed.

#define SimMPS          SimulationDisabled
#define SimStubMPS      SimulationDisabled

#if SimMPS == SimulationEnabled
#include "mpi.h"
#endif



// ***********************
// Platform or compiler identifier to allow different implementations of the STL
// and other features. There is a corresponding flag, xxBasePlatform in xxBase.h.

#define MFC			1
#define CONSOLE			2
#define DECALPHA		3
#define SGICC			4
#define CRAYJ90			5
#define BORLAND6		6
#define I7XEON			7
#define I7ITANIUM		8
#define GCC 			9
#define CW55MAC 		10
#define XCMAC 			11
#define NEWPLATFORM1 	12
#define NEWPLATFORM2 	13
#define NEWPLATFORM3 	14
#define NEWPLATFORM4 	15
#define NEWPLATFORM5 	16
#define NEWPLATFORM6 	17
#define NEWPLATFORM7 	18
#define NEWPLATFORM8 	19
#define NEWPLATFORM9 	20
#define NEWPLATFORM10 	21


// I find that using the same platform flag for all linux-based platforms works fine.

#define Platform XCMAC

// ***********************
// Uncomment the next variable to allow each DPD bead type to have an explicit
// radius of interaction. This is not relevant for MD simulations.

//#define UseDPDBeadRadii 1

// Uncomment the next pragma to avoid some warnings when compiling on a PC

//#pragma warning (disable:4786)

//#define TraceOn				// output debug messages

// ***********************
// Select the SAX version for parsing the protocol files using the following flag

#define SAX10		1
#define	SAX20		2


#define SAXProtocol	SAX10

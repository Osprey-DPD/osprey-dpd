// Header file to hold flags and options relating to the XML-enabling of the code.
//
// This file must be included in all .cpp files that use conditional compilation
// to read/write XML-enabled files.
//
// **********************************************************************
//
// Version History
// ***************
//
//	8/12/03    Baseline version.
//			   I used a flag to show if commands are XML enabled or not.
//			   The flags can be set for each platform independently.
//	9/12/03    I added flags for processes, aggregate analysis, events and
//			   messages written to the CLogState file.
//  28/1/04    I added a flag to allow the protocol file to be written in xml.
//			   Note that the EnableXMLCommands flag must be enabled if the
//			   protocol file is to be written correctly.
//  11/3/04    I subsumed the analysis and event flags in the processes flag.
//			   This is required because they use the CTimeSeriesData class
//			   and it is common to the history state, aggregate states and
//			   process state classes.
//	2/8/04     I turned off XML output for processes so that I can graph data
//			   from the ACNs.
//  04/05/06   I copied the CW55MAC flags to XCMAC.
//  16/08/06   I added a new flag (EnableXMLDecorators) to control the 
//             serialisation of decorator data.
//  30/10/07   I added a new flag (EnableXMLParallelMessages) to control the 
//             messages passed in the parallel code.

// **********************************************************************

#define SimXMLEnabled	1
#define SimXMLDisabled	2

#if Platform   == DECALPHA 
#elif Platform == SGICC
#elif Platform == CRAYJ90
#elif Platform == BORLAND6
#elif Platform == I7XEON
#elif Platform == I7ITANIUM
#elif Platform == GCC
#elif Platform == CW55MAC
	#define EnableXMLProtocol               SimXMLDisabled
	#define EnableXMLProcesses              SimXMLDisabled
	#define EnableXMLDecorators             SimXMLDisabled
	#define EnableXMLCommands               SimXMLDisabled
	#define EnableXMLParallelMessagess      SimXMLDisabled
#elif Platform == XCMAC
	#define EnableXMLProtocol               SimXMLDisabled
	#define EnableXMLProcesses              SimXMLDisabled
	#define EnableXMLDecorators             SimXMLDisabled
	#define EnableXMLCommands               SimXMLDisabled
	#define EnableXMLParallelMessagess      SimXMLDisabled
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
	#define EnableXMLProtocol               SimXMLDisabled
	#define EnableXMLProcesses              SimXMLDisabled
	#define EnableXMLDecorators             SimXMLDisabled
	#define EnableXMLCommands               SimXMLDisabled
	#define EnableXMLParallelMessagess      SimXMLDisabled
#endif					


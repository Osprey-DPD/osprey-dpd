// Header file to provide access to platform-dependent libraries.
//
// This file must be included in all .cpp files that use conditional compilation
// to access the STL <algorithm> classes.
//
// **********************************************************************
//
// Version History
// ***************
//
//	29/8/03    Baseline version.
//  15/9/03    I added using declarations for highly-used algorithms copy,
//			   find, find_if.
//  04/05/06    I copied the CW55MAC flags to XCMAC.
//  08/04/08   I copied the using declarations from the console platform
//             to the CW55MAC platform as the compilation on aelfun failed
//             without them as it could not locate "find" etc.
//  20/11/09   I added the copy,find, find_if using declarations to the XCMAC platform.
//
//
// **********************************************************************

#if Platform == DECALPHA 
	#include <algorithm>
#elif Platform == SGICC
	#include <algo.h>
#elif Platform == CRAYJ90
	#include <algo.h>
#elif Platform == BORLAND6
	#include <algorithm>
	using std::copy;
	using std::find;
	using std::find_if;
	using std::back_inserter;
#elif Platform == I7XEON
	#include <algorithm>
	using std::copy;
	using std::find;
	using std::find_if;
	using std::back_inserter;
#elif Platform == I7ITANIUM
#elif Platform == GCC
#elif Platform == CW55MAC
	#include <algorithm>
	using std::copy;
	using std::find;
	using std::find_if;
	using std::back_inserter;
	using std::pair;
	using std::make_pair;
#elif Platform == XCMAC
	#include <algorithm>
	using std::copy;
	using std::find;
	using std::find_if;
	using std::back_inserter;
	using std::pair;
	using std::make_pair;
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
	#include <algorithm>
	using std::copy;
	using std::find;
	using std::find_if;
	using std::back_inserter;
	using std::pair;
	using std::make_pair;
#endif					


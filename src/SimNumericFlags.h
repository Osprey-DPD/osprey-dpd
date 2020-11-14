// Header file to provide access to platform-dependent libraries.
//
// This file must be included in all .cpp files that use conditional compilation
// to access the STL <numeric> classes.
//
// **********************************************************************
//
// Version History
// ***************
//
//	29/8/03     Baseline version.
//  04/05/06    I copied the CW55MAC flags to XCMAC.
//  17/06/09    I added a using declaration for accumulate.
//
// **********************************************************************

	#include <numeric>
    using std::accumulate;


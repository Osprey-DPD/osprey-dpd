/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// xxBase.cpp: implementation of the xxBase class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "xxBase.h"

#include "IGlobalSimBox.h"
#include "LogSimErrorTrace.h"
#include "LogSimTrace.h"

//////////////////////////////////////////////////////////////////////
// Static variable definitions
//////////////////////////////////////////////////////////////////////

const double xxBase::m_globalPI		 = 3.141592653589793238;
const double xxBase::m_globalTwoPI	 = 2.0*m_globalPI;
const double xxBase::m_globalFourPI	 = 4.0*m_globalPI;
const double xxBase::m_globalPIByTwo = 0.5*m_globalPI;

// **********************************************************************
// Global Functions.
//
// Global static function to return the string used to prefix all file names. 
// The value of this string depends on the type of build. The following list 
// gives the current set:
//
// dmpc    - default run for my use
//
// Note that the first function returns just the string without the "." that
// separates the prefix from the extension, but that all subsequent functions
// return the "prefix." so that they can be used directly to name a file
// by simply appending the extension string.
//
// We use  a static member here so that changing the prefix does not require 
// a complete recompilation.

const zString xxBase::GetFilePrefix()
{
    return "dmpc";
}
    
// Return the prefix for each input/output file type. Note that the "." between
// the prefix and extension IS included in these strings.

const zString xxBase::GetAAPrefix()
{
    return GetFilePrefix()+"aa.";
}
    
const zString xxBase::GetASPrefix()
{
    return GetFilePrefix()+"as.";
}

const zString xxBase::GetCDFPrefix()
{
    return GetFilePrefix()+"i.";
}
    
const zString xxBase::GetCHPrefix()
{
    return GetFilePrefix()+"ch.";
}
    
const zString xxBase::GetCQPrefix()
{
    return GetFilePrefix()+"q.";
}
    
const zString xxBase::GetCSPrefix()
{
    return GetFilePrefix()+"cs.";
}
    
const zString xxBase::GetDPPrefix()
{
    return GetFilePrefix()+"dp.";
}
    
const zString xxBase::GetDSPrefix()
{
    return GetFilePrefix()+"ds.";
}
    
const zString xxBase::GetEADPrefix()
{
    return GetFilePrefix()+"ead.";
}
    
const zString xxBase::GetHSPrefix()
{
    return GetFilePrefix()+"hs.";
}
    
const zString xxBase::GetISPrefix()
{
    return GetFilePrefix()+"is.";
}
    
const zString xxBase::GetLSPrefix()
{
    return GetFilePrefix()+"ls.";
}
    
const zString xxBase::GetOSPrefix()
{
    return GetFilePrefix()+"os.";
}
    
const zString xxBase::GetPSPrefix()
{
    return GetFilePrefix()+"ps.";
}
    
const zString xxBase::GetPSDFPrefix()
{
    return GetFilePrefix()+"psdf.";
}

const zString xxBase::GetRAPrefix()
{
    return GetFilePrefix()+"ra.";
}
  
const zString xxBase::GetRSPrefix()
{
    return GetFilePrefix()+"rs.";
}

const zString xxBase::GetTADSPrefix()
{
    return GetFilePrefix()+"tads.";
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//
xxBase::xxBase()
{
}

// Copy constructor - this class has no state so nothing to copy.

xxBase::xxBase(const xxBase& oldBase)
{
}

xxBase::~xxBase()
{
}

// General function for writing out error messages for all classes that inherit
// from xxBase. If the IGlobalSimBox() instance is non-null we get the current 
// time and log an error message. If the instance is null, we assume the simulation
// has not yet been initialised and output the message to the screen.
//
// We disable the error message in the parallel code as it crashes for
// an unknown reason.

bool xxBase::ErrorTrace(zString errStr) const
{
#if SimMPS == SimulationDisabled

	if(IGlobalSimBox::Instance())
	{
		CLogSimErrorTrace* pMsg = new CLogSimErrorTrace(IGlobalSimBox::Instance()->GetCurrentTime(), errStr);
    }
	else
	{
		TraceEndl();
		TraceStringNoEndl(errStr);
		TraceEndl();
	}

#endif

	return false;
}

// General function for writing out informational messages for all classes that 
// inherit from xxBase. If the IGlobalSimBox() instance is non-null we get 
// the current time and log an information message. If the instance is null, 
// we assume the simulation has not yet been initialised and output the message 
// to the screen.

void xxBase::Trace(zString msgStr) const
{
#if SimMPS == SimulationDisabled

	if(IGlobalSimBox::Instance())
	{
		CLogSimTrace* pMsg = new CLogSimTrace(IGlobalSimBox::Instance()->GetCurrentTime(), msgStr);
    }
	else
	{
		TraceEndl();
		TraceStringNoEndl(msgStr);
		TraceEndl();
	}

#endif
}

// General functions for writing out debug information for all classes that
// inherit from xxBase. Because the data are written to the screen these 
// functions should not be used in production runs as they may crash the
// job in batch mode if access to the screen is forbidden.
//
// For simplicity, we only output simple strings and the values of a restricted
// number of variables with limited formatting. Endline characters must be
// added explicitly to allow for strings and variables to be concatenated on
// the same line.
//

void xxBase::TraceInt(zString msgStr, long var1) const
{
	TraceStringNoEndl(msgStr);
	TraceIntNoEndl(var1);
	TraceEndl();
}

void xxBase::TraceInt2(zString msgStr, long var1, long var2) const
{
	TraceStringNoEndl(msgStr);
	TraceIntNoEndl(var1);
	TraceIntNoEndl(var2);
	TraceEndl();
}

void xxBase::TraceInt3(zString msgStr, long var1, long var2, long var3) const
{
	TraceStringNoEndl(msgStr);
	TraceIntNoEndl(var1);
	TraceIntNoEndl(var2);
	TraceIntNoEndl(var3);
	TraceEndl();
}

void xxBase::TraceDouble(zString msgStr, double var1) const
{
	TraceStringNoEndl(msgStr);
	TraceDoubleNoEndl(var1);
	TraceEndl();
}

void xxBase::TraceDouble2(zString msgStr, double var1, double var2) const
{
	TraceStringNoEndl(msgStr);
	TraceDoubleNoEndl(var1);
	TraceDoubleNoEndl(var2);
	TraceEndl();
}

// 
// Platform-specific functions to write out debug output
//

void xxBase::TraceEndl() const
{
#if SimMPS == SimulationDisabled

	std::cout << zEndl;

#endif
}

void xxBase::TraceStringNoEndl(zString msgStr) const
{
#if SimMPS == SimulationDisabled

	std::cout << msgStr.c_str() << "  ";

#endif
}

void xxBase::TraceIntNoEndl(long var1) const
{
#if SimMPS == SimulationDisabled

	std::cout << var1 << " ";

#endif
}

void xxBase::TraceDoubleNoEndl(double var1) const
{
#if SimMPS == SimulationDisabled

	std::cout << var1 << " ";

#endif

}

// Function to write out the value of a vector (3 doubles) using a specific
// format: this is used in CMonitor to check the values of vector observables.

void xxBase::TraceVector(zString msgStr, double var1, double var2, double var3) const
{
#if SimMPS == SimulationDisabled

	std::cout << msgStr.c_str() << "  " << var1 << "  " << var2 << "  " << var3 << zEndl;

#endif
}

// Function to convert an unsigned long int to a string. We just forward to the following overload.

zString xxBase::ToString(unsigned long x) const
{
    return ToString(static_cast<long>(x));
}

// Function to convert a long int to a zString. This is used, for example,
// in constructing filenames and observables that need to use a bead or bond type 
// to identify the data.
// Note that the returned string is not const so it can be modified if needed.

zString xxBase::ToString(long x) const
{
	zOutStringStream oss;

	oss << x;

	zInStringStream iss(oss.str());

	zString str;

	iss >> str;

	return str;
}

// Function to convert a double to a zString. This is used, for example,
// to embed bead coordinates inside log messages.
// Note that the returned string is not const so it can be modified if needed.

zString xxBase::ToString(double x) const
{
	zOutStringStream oss;

	oss << x;

	zInStringStream iss(oss.str());

	zString str;

	iss >> str;

	return str;
}




/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// acBinEventSuccessIntervals.cpp: implementation of the acBinEventSuccessIntervals class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "acBinEventSuccessIntervals.h"
#include "ISimCmd.h"
#include "ISimBox.h"
#include "InputData.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this command. 
// The static member function GetType() is invoked by the xxCommandObject 
// to compare the type read from the control data file with each
// xxCommand-derived class so that it can create the appropriate object 
// to hold the command data.

const zString acBinEventSuccessIntervals::m_Type = "BinEventSuccessIntervals";

const zString acBinEventSuccessIntervals::GetType()
{
	return m_Type;
}

// We use an anonymous namespace to wrap the call to the factory object
// so that it is not accessible from outside this file. The identifying
// string for the command is stored in the m_Type static member variable.
//
// Note that the Create() function is not a member function of the
// command class but a global function hidden in the namespace.

namespace
{
	xxCommand* Create(long executionTime) {return new acBinEventSuccessIntervals(executionTime);}

	const zString id = acBinEventSuccessIntervals::GetType();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

acBinEventSuccessIntervals::acBinEventSuccessIntervals(long executionTime) : acACNCreateEventAnalysis(executionTime),
                                                        m_BinTotal(0), m_Samples(0), m_BinWidth(0.0)
{
}

acBinEventSuccessIntervals::acBinEventSuccessIntervals(const acBinEventSuccessIntervals& oldCommand) : acACNCreateEventAnalysis(oldCommand),
                                                        m_BinTotal(oldCommand.m_BinTotal), 
                                                        m_Samples(oldCommand.m_Samples), m_BinWidth(oldCommand.m_BinWidth)

{
}


acBinEventSuccessIntervals::~acBinEventSuccessIntervals()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
// m_BinTotal  Number of bins in the histogram
// m_BinWidth  Fixed width of bins in the histogram: Note. One of these must be zero.
// m_Samples   Number of simulation sample periods over which to construct the
//             histogram of event successes

zOutStream& acBinEventSuccessIntervals::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);
	acACNCreateEventAnalysis::put(os);
	os << "<BinTotal>"    << m_BinTotal << "</BinTotal>"    << zEndl;
	os << "<BinWidth>"    << m_BinWidth << "</BinWidth>"    << zEndl;
	os << "<SampleTotal>" << m_Samples  << "</SampleTotal>" << zEndl;
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	acACNCreateEventAnalysis::put(os);
    os << " " << m_BinTotal << " " << m_BinWidth << " " << m_Samples;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& acBinEventSuccessIntervals::get(zInStream& is)
{
	// Read the base class first

    acACNCreateEventAnalysis::get(is);

    is >> m_BinTotal >> m_BinWidth >> m_Samples;

	if(!is.good() || (m_BinWidth < 1.0 && (m_BinTotal < 1 || m_BinTotal > 100000))
                  || (m_BinTotal < 1   && (m_BinWidth < 1.0))
                  || m_Samples < 1     || (m_BinTotal > 0 && m_BinWidth > 0.0))
	   SetCommandValid(false);

	return is;

}

// Non-static function to return the type of the command

const zString acBinEventSuccessIntervals::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* acBinEventSuccessIntervals::GetCommand() const
{
	return new acBinEventSuccessIntervals(*this);
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool acBinEventSuccessIntervals::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
        // Call the SimBox's implementation of this command to create a
        // taBinSuccessIntervalsEventAnalysis instance. 
        // Execution of the command is logged by the command's implementation
        // class.

        pISimCmd->BinEventSuccessIntervals(this);

		return true;
	}
	else
		return false;
}

// Function to check that the data defining the command is valid.
//
// Only the base class reads any data, so we just return its validation state.

bool acBinEventSuccessIntervals::IsDataValid(const CInputData& riData) const
{
    return acACNCreateEventAnalysis::IsDataValid(riData);
}

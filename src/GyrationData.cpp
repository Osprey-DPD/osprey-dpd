/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// GyrationData.cpp: implementation of the CGyrationData class.
//
// This analysis specification object is used to calculate the radius of gyration
// of a set of polymers. It only calculates the Rg**2 for the beads in the polymer, and
// various subsets of these beads. The complete radius of gyration tensor is not calculated.
//
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimXMLFlags.h"
#include "GyrationData.h"
#include "Gyration.h"


//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for the aggregates that are
// investigated by analysis objects of this class. It is used to select the 
// appropriate analysis object when the Analysis-Type variable is read in the 
// control data file. The static member function GetType() is invoked by the
// CAnalysisObject to compare the type read from the control data file with each
// analysis data class so that it can create the appropriate object to hold the
// analysis data.

zString CGyrationData::m_Type = "gyration";

const zString CGyrationData::GetType()
{
	return m_Type;
}

// We use an anonymous namespace to wrap the call to the factory object
// so that it is not accessible from outside this file. The identifying
// string is stored in the m_Type static member variable.
//
// Note that the Create() function is not a member function but a global 
// function hidden in the namespace.

namespace
{
	CAnalysisData* Create() {return new CGyrationData();}

	const zString id = CGyrationData::GetType();

	const bool bRegistered = acfAnalysisFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// We define default data for an Gyration analysis object in the constructor. 
// Because we cannot specify default polymer names, we set the IsDataValid flag 
// to false in the CAnalysisData base class to indicate that this object is not 
// ready to do analysis.
//
// The type of the object is specified as a const zString so that it can be used
// without fear of typos in several classes.

CGyrationData::CGyrationData() : CAnalysisData(false), m_bByType(false)
{
    m_Polymers.clear();

}

CGyrationData::~CGyrationData()
{
}


// Functions to read and write the data needed by the Gyration aggregate analysis.
// We call the base class's function first to get the more general data before
// that specific to each aggregate.

zOutStream& CGyrationData::put(zOutStream& os) const
{

#if EnableXMLProcesses == SimXMLEnabled

	putXMLStartTags(os);
    os << "<Polymers>" << zEndl;
    for(cStringSequenceIterator iterName=m_Polymers.begin(); iterName!=m_Polymers.end(); iterName++)
    {
        os << "<PolymerName>" << (*iterName) << "</PolymerName>" << zEndl;
    }
    os << "</Polymers>" << zEndl;
	putXMLEndTags(os);

#elif EnableXMLProcesses == SimXMLDisabled

	putASCIIStartTags(os);
    os << "    Polymers   ";
    
    for(cStringSequenceIterator iterName=m_Polymers.begin(); iterName!=m_Polymers.end(); iterName++)
    {
        os << " " << (*iterName);
    }
    
    os << zEndl;
    os << "    ByType		" << m_bByType << zEndl;
	putASCIIEndTags(os);

#endif

	return os;
}

// Function to read the analysis data from file. The keywords "Analysis"
// and "Type" and the value of type have already been read in by the CInputDPDFile 
// object so we only have to read the actual analysis data.

zInStream& CGyrationData::get(zInStream& is)
{
	// Read base class data first

	CAnalysisData::get(is);

    zString token   = "";
    zString sName   = "";
    
    is >> token;
    if(!is.good() || token != "Polymers")
    {
        SetDataValid(false);
        return is;
    }
    else
    {
        is >> sName;
        
        if(!is.good() || sName == "ByType"  || sName.empty())
        {
            SetDataValid(false);
            return is;
        }
        else
        {
            while(sName != "ByType")
            {
                if(is.good() && !sName.empty())
                    m_Polymers.push_back(sName);
                else
                {
                    SetDataValid(false);
                    return is;
                }
                
                is >> sName;
            }
        }
    }
    
    if(!is.good() || sName != "ByType")
    {
        SetDataValid(false);
        return is;
    }
    else
    {
        is >> m_bByType;
    }

    // Data has been read successfully so store it in the member variables
    
    SetDataValid(true);

	return is;
}


CAnalysis* CGyrationData::CreateAnalysis(const zString runId, long sample)
{
	return new CGyration(runId, sample, this);
}

// Virtual function to return the analysis type.

const zString CGyrationData::GetAnalysisType() const
{
	return m_Type;
}

// Function to check that the data defining the aggregate analysis is valid. We cannot check if the command
// target is valid yet as it may not be created until later. We defer this to the implementation class.

bool CGyrationData::ValidateData(const CInputData& riData) const
{

	return true;
}


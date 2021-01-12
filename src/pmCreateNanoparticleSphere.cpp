/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// pmCreateNanoparticleSphere.cpp: implementation of the pmCreateNanoparticleSphere class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "pmCreateNanoparticleSphere.h"
#include "ccCreateNanoparticleSphere.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this message. 

const zString pmCreateNanoparticleSphere::m_Type = "CreateNanoparticleSphere";

const zString pmCreateNanoparticleSphere::GetType()
{
	return m_Type;
}

// We use an anonymous namespace to wrap the call to the factory object
// so that it is not accessible from outside this file. The identifying
// string for the command is stored in the m_Type static member variable.
//
// Note that the Create() function is not a member function of the
// message class but a global function hidden in the namespace.

namespace
{
	mpmMessage* Create() {return new pmCreateNanoparticleSphere();}

	const zString id = pmCreateNanoparticleSphere::GetType();

	const bool bRegistered = acfParallelMessageFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// We store all parameters for all simulation types here, but conditionally
// compile the functions that access them depending on the SimIdentifier flag.

pmCreateNanoparticleSphere::pmCreateNanoparticleSphere() : mpmCommandWrapper(), m_PolymerName(""),
                                                           m_TargetProcId(0), m_ColourId(0), 
                                                           m_XC(0.0), m_YC(0.0), m_ZC(0.0), m_InnerRadius(0.0), m_OuterRadius(0.0),
                                                           m_MaxBonds(0), m_Range(0.0), m_Fraction(0.0), m_SpringConstant(0.0), m_UnstretchedLength(0.0),
                                                           m_NumberTypes(0)
{
     m_vBeadTypes.clear();
     m_vConsInt.clear();
}

// Constructor for use when creating the command internally. This is used in the parallel code when the command is sent to all 
// processors to create nanoparticles in specified processors' spaces.

pmCreateNanoparticleSphere::pmCreateNanoparticleSphere(const zString polyName, long procId, long colourId, 
                               double xc, double yc, double zc, double innerRadius, double outerRadius,
                               long maxBonds, double range, double fraction, double springConstant, double unstretchedLength, 
                               long numberTypes, zLongVector vBeadTypes, zDoubleVector vConsInt) : m_PolymerName(polyName),
                                                                                                   m_TargetProcId(procId),
                                                                                                   m_ColourId(colourId),
                                                                                                   m_XC(xc),
                                                                                                   m_YC(yc),
                                                                                                   m_ZC(zc),
                                                                                                   m_InnerRadius(innerRadius),
                                                                                                   m_OuterRadius(outerRadius),
                                                                                                   m_MaxBonds(maxBonds),
                                                                                                   m_Range(range),
                                                                                                   m_Fraction(fraction),
                                                                                                   m_SpringConstant(springConstant),
                                                                                                   m_UnstretchedLength(unstretchedLength),
                                                                                                   m_NumberTypes(numberTypes),
                                                                                                   m_vBeadTypes(vBeadTypes),
                                                                                                   m_vConsInt(vConsInt)

{                               
}

pmCreateNanoparticleSphere::pmCreateNanoparticleSphere(const pmCreateNanoparticleSphere& oldMessage) : mpmCommandWrapper(oldMessage),
													                                                   m_PolymerName(oldMessage.m_PolymerName),
                                                                                                       m_TargetProcId(oldMessage.m_TargetProcId),
                                                                                                       m_ColourId(oldMessage.m_ColourId),
                                                                                                       m_XC(oldMessage.m_XC),
                                                                                                       m_YC(oldMessage.m_YC),
                                                                                                       m_ZC(oldMessage.m_ZC),
                                                                                                       m_InnerRadius(oldMessage.m_InnerRadius),
                                                                                                       m_OuterRadius(oldMessage.m_OuterRadius),
                                                                                                       m_MaxBonds(oldMessage.m_MaxBonds),
                                                                                                       m_Range(oldMessage.m_Range),
                                                                                                       m_Fraction(oldMessage.m_Fraction),
                                                                                                       m_SpringConstant(oldMessage.m_SpringConstant),
                                                                                                       m_UnstretchedLength(oldMessage.m_UnstretchedLength),
                                                                                                       m_NumberTypes(oldMessage.m_NumberTypes),
                                                                                                       m_vBeadTypes(oldMessage.m_vBeadTypes),
                                                                                                       m_vConsInt(oldMessage.m_vConsInt)
{
}

pmCreateNanoparticleSphere::~pmCreateNanoparticleSphere()
{
}

// Member functions to read/write the data specific to the message.
//
// Arguments
// *********
//
//  m_PolymerName;          // Polymer type that composes the nanoparticle
//  m_TargetProcId;         // Processor that will create the nanoparticle
//  m_ColourId;             // Colour of nanoparticle (integer, see CurrentStateFormat.cpp for colour sequence)
//
//  m_XC;
//	m_YC;		            // Sphere centre as fractions of SimBox side lengths
//	m_ZC;
//	m_InnerRadius;	        // Inner radius of sphere	(units of bead diameter)
//	m_OuterRadius;	        // Outer radius of sphere	(units of bead diameter)
//    
//	m_MaxBonds;				// Max bonds per polymer
//	m_Range;				// Max separation within which to create bonds
//	m_Fraction;				// Fraction of neighbours to be bound
//	m_SpringConstant;		// Hookean parameters for bonds
//	m_UnstretchedLength;
    
//  m_NumberTypes;          // Number of bead types in simulation
//  m_vBeadTypes;           // Set of bead types in the polymer
//  m_vConsInt;             // Conservative interactions of the nanoparticle's beads with all other bead types

zOutStream& pmCreateNanoparticleSphere::put(zOutStream& os) const
{
#if EnableXMLParallelMessagess == SimXMLEnabled

	// XML output
	putXMLStartTags(os);
	os << "<MessageId>"    << GetId()   << "</MessageId>" << zEndl;
	putXMLEndTags(os);

#elif EnableXMLParallelMessagess == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	os << GetId();
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& pmCreateNanoparticleSphere::get(zInStream& is)
{
	return is;
}

// Non-static function to return the type of the message

const zString pmCreateNanoparticleSphere::GetMessageType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current message.

const mpmMessage* pmCreateNanoparticleSphere::GetMessage() const
{
	return new pmCreateNanoparticleSphere(*this);
}

// ****************************************
// Function to copy all the required data for the command into the message.
// Because each command message wraps a known command we can extract the
// data using dynamic cast.

void pmCreateNanoparticleSphere::SetMessageData(const xxCommand* const pCommand)
{
	const ccCreateNanoparticleSphere* const pCmd = dynamic_cast<const ccCreateNanoparticleSphere*>(pCommand);

    if(pCmd)
    {
	    m_PolymerName        = pCmd->GetPolymerName();
	    m_TargetProcId       = pCmd->GetProcessorId();
	    m_ColourId           = pCmd->GetColourId();
        m_XC                 = pCmd->GetXCentre();
        m_YC                 = pCmd->GetYCentre();
        m_ZC                 = pCmd->GetZCentre();
        m_InnerRadius        = pCmd->GetInnerRadius();
        m_OuterRadius        = pCmd->GetOuterRadius();
	    m_MaxBonds           = pCmd->GetMaxBondsPerPolymer();
	    m_Range              = pCmd->GetRange();
	    m_Fraction           = pCmd->GetBondFraction();
	    m_SpringConstant     = pCmd->GetSpringConstant();
	    m_UnstretchedLength  = pCmd->GetUnstretchedLength();
 	    m_NumberTypes        = pCmd->GetNumberTypes();
 	    m_vBeadTypes         = pCmd->GetBeadTypes();
 	    m_vConsInt           = pCmd->GetConsInt();
    }
    else
    {
	    m_PolymerName        = "";
	    m_TargetProcId       = 0;
	    m_ColourId           = 0;
        m_XC                 = 0.0;
        m_YC                 = 0.0;
        m_ZC                 = 0.0;
        m_InnerRadius        = 0.0;
        m_OuterRadius        = 0.0;
	    m_MaxBonds           = 0;
	    m_Range              = 0.0;
	    m_Fraction           = 0.0;
	    m_SpringConstant     = 0.0;
	    m_UnstretchedLength  = 0.0;
 	    m_NumberTypes        = 0;
 	    m_vBeadTypes.clear();
 	    m_vConsInt.clear();
    }
}

// ****************************************
// Pure virtual function to check that the data are valid prior to sending a message.

bool pmCreateNanoparticleSphere::Validate()
{
    bool bSuccess = true;
    
//    std::cout << "Proc " << GetProcId() << " validating arguments: " << zEndl;
//    std::cout << m_PolymerName << " " << m_TargetProcId << " " << m_ColourId << " ";
//    std::cout << m_XC << " " << m_YC << " " << m_ZC << " " << m_InnerRadius << " " << m_OuterRadius << " ";
//    std::cout << m_MaxBonds << " " << m_Range << " " << m_Fraction << " " << m_SpringConstant << " " << m_UnstretchedLength << " " ;
//    std::cout << m_NumberTypes << zEndl;
    
    if(m_NumberTypes != static_cast<long>(m_vBeadTypes.size()) || m_NumberTypes != static_cast<long>(m_vConsInt.size()))
    {
        std::cout << "Invalid number of beadtypes: " << m_NumberTypes << " " << m_vBeadTypes.size() << " " << m_vConsInt.size() << zEndl;
    }
	
    if( m_PolymerName.empty() || m_TargetProcId < 0 || m_ColourId < 0 ||
	     m_XC <= 0.0 ||  m_YC <= 0.0 ||  m_ZC <= 0.0 || 
	     m_XC >= 1.0 ||  m_YC >= 1.0 ||  m_ZC >= 1.0 || 
         m_InnerRadius < 0.0 || m_OuterRadius < m_InnerRadius ||
         m_MaxBonds < 1 || m_Range < 0.0 || m_Fraction < 0.0 || m_Fraction > 1.0 ||
         m_SpringConstant < 0.0 || m_UnstretchedLength < 0.0 || m_NumberTypes < 1 ||
         m_vBeadTypes.empty() || m_vConsInt.empty() || 
         static_cast<long>(m_vBeadTypes.size()) != m_NumberTypes || static_cast<long>(m_vConsInt.size()) != m_NumberTypes)
    {
        std::cout << "pmCreateNanoparticleSphere validation failed" << zEndl;
        bSuccess = false;
    }

    return bSuccess;
}

// Wrapper for the MPI function:
// MPI_Send(message, length, MPI_CHAR, m_ProcId, tag, MPI_COMM_WORLD);

void pmCreateNanoparticleSphere::SendAllP()
{
#if SimMPS == SimulationEnabled

	if(IsProcessZero())
	{
        char buffer[10000];
        int  position;

        char name[1000];
        strcpy(name, m_PolymerName.c_str());
        long nameLength = m_PolymerName.size() + 1;

        position = 0;
        MPI_Pack(&nameLength,           1,  MPI_LONG,   buffer, 10000, &position, MPI_COMM_WORLD);
        MPI_Pack(&name, nameLength,         MPI_CHAR,   buffer, 10000, &position, MPI_COMM_WORLD);
        MPI_Pack(&m_TargetProcId,       1,  MPI_LONG,   buffer, 10000, &position, MPI_COMM_WORLD);
        MPI_Pack(&m_ColourId,           1,  MPI_LONG,   buffer, 10000, &position, MPI_COMM_WORLD);
        MPI_Pack(&m_XC,                 1,  MPI_DOUBLE, buffer, 10000, &position, MPI_COMM_WORLD);
        MPI_Pack(&m_YC,                 1,  MPI_DOUBLE, buffer, 10000, &position, MPI_COMM_WORLD);
        MPI_Pack(&m_ZC,                 1,  MPI_DOUBLE, buffer, 10000, &position, MPI_COMM_WORLD);
        MPI_Pack(&m_InnerRadius,        1,  MPI_DOUBLE, buffer, 10000, &position, MPI_COMM_WORLD);
        MPI_Pack(&m_OuterRadius,        1,  MPI_DOUBLE, buffer, 10000, &position, MPI_COMM_WORLD);
        MPI_Pack(&m_MaxBonds,           1,  MPI_LONG,   buffer, 10000, &position, MPI_COMM_WORLD);
        MPI_Pack(&m_Range,              1,  MPI_DOUBLE, buffer, 10000, &position, MPI_COMM_WORLD);
        MPI_Pack(&m_Fraction,           1,  MPI_DOUBLE, buffer, 10000, &position, MPI_COMM_WORLD);
        MPI_Pack(&m_SpringConstant,     1,  MPI_DOUBLE, buffer, 10000, &position, MPI_COMM_WORLD);
        MPI_Pack(&m_UnstretchedLength,  1,  MPI_DOUBLE, buffer, 10000, &position, MPI_COMM_WORLD);
        MPI_Pack(&m_NumberTypes,        1,  MPI_LONG,   buffer, 10000, &position, MPI_COMM_WORLD);

        for(long i=0; i<m_NumberTypes; i++)
        {
            long beadType = m_vBeadTypes.at(i);
            double cons   = m_vConsInt.at(i);

            MPI_Pack(&beadType, 1, MPI_LONG, buffer, 10000, &position, MPI_COMM_WORLD);
            MPI_Pack(&cons,     1, MPI_DOUBLE, buffer, 10000, &position, MPI_COMM_WORLD);
        }
        SetTag(0);
        
        for(m_ProcId=1; m_ProcId<GetWorld(); m_ProcId++)
        {
            MPI_Send(buffer, 10000, MPI_PACKED, m_ProcId, m_Tag, MPI_COMM_WORLD);
        }
	}

#endif
}

// Wrapper for the Mpi function:
// MPI_Recv(message, length, MPI_CHAR, m_ProcId, tag, MPI_COMM_WORLD);

// We retrieve the data from the message and fill up this instance's member variables. 
// We do not check for validity here as we assume that the sending object has 
// already done it. Note that this function does not propagate the data out into 
// the code, it only fills up the receiving message instance. Each messaging class 
// is responsible for providing further functions to pass the data to the rest of the code.

void pmCreateNanoparticleSphere::Receive()
{
#if SimMPS == SimulationEnabled

    SetTag(0);
    MPI_Status status;

	m_ReceiverRank = GetRank();

    char buffer[10000];
    int position;

    char name[1000];
    long nameLength = 0;
	
    MPI_Recv(buffer, 10000, MPI_PACKED, 0, m_Tag, MPI_COMM_WORLD, &status);

	m_SenderRank = status.MPI_SOURCE;
	
    // Unpack the data into member variables

    position = 0;
    MPI_Unpack(buffer, 10000, &position, &nameLength,          1,  MPI_LONG, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 10000, &position, &name, nameLength,        MPI_CHAR, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 10000, &position, &m_TargetProcId,      1,  MPI_LONG, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 10000, &position, &m_ColourId,          1,  MPI_LONG, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 10000, &position, &m_XC,                1,  MPI_DOUBLE, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 10000, &position, &m_YC,                1,  MPI_DOUBLE, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 10000, &position, &m_ZC,                1,  MPI_DOUBLE, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 10000, &position, &m_InnerRadius,       1,  MPI_DOUBLE, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 10000, &position, &m_OuterRadius,       1,  MPI_DOUBLE, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 10000, &position, &m_MaxBonds,          1,  MPI_LONG, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 10000, &position, &m_Range,             1,  MPI_DOUBLE, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 10000, &position, &m_Fraction,          1,  MPI_DOUBLE, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 10000, &position, &m_SpringConstant,    1,  MPI_DOUBLE, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 10000, &position, &m_UnstretchedLength, 1,  MPI_DOUBLE, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 10000, &position, &m_NumberTypes,       1,  MPI_LONG, MPI_COMM_WORLD);
    
    long beadType = 0;
    double cons   = 0.0;

    for(long i=0; i<m_NumberTypes; i++)
    {
        MPI_Unpack(buffer, 10000, &position, &beadType, 1, MPI_LONG, MPI_COMM_WORLD);
        MPI_Unpack(buffer, 10000, &position, &cons, 1, MPI_DOUBLE, MPI_COMM_WORLD);

        m_vBeadTypes.push_back(beadType);
        m_vConsInt.push_back(cons);
    }

    m_PolymerName = name;

    // Now instantiate the concrete command class for all processors so that we can propagate the changes.
    // We use the internal constructor that takes the command's data as arguments and log the execution.
    
    m_pCommand = new ccCreateNanoparticleSphere(m_SimTime, m_PolymerName, m_TargetProcId, m_ColourId, m_XC, m_YC, m_ZC,
                                                m_InnerRadius, m_OuterRadius, m_MaxBonds, m_Range, m_Fraction, m_SpringConstant, m_UnstretchedLength, 
                                                m_NumberTypes, m_vBeadTypes, m_vConsInt);	
#endif
}

// Stub function for the overloaded Receive function to avoid compiler warnings on the Hermit1.hww.de platform.

void pmCreateNanoparticleSphere::Receive(long procId)
{
#if SimMPS == SimulationEnabled

#endif
}

// PVF to assemble the disparate data items into a struct suitable for sending to 
// the MPI messaging functions. All the data must be in the calling object, and the
// data type is not usable outside this class.
// The SimMPS flag must be defined for this function to work, otherwise it does nothing.

void pmCreateNanoparticleSphere::BuildDerivedType(MsgDataTypePtr pMsgDataType)
{
}


/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// pmISVesicle.cpp: implementation of the pmISVesicle class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "pmISVesicle.h"
#include "InitialStateVesicle.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this message. 

const zString pmISVesicle::m_Type = "vesicle";

const zString pmISVesicle::GetType()
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
	mpmMessage* Create() {return new pmISVesicle();}

	const zString id = pmISVesicle::GetType();

	const bool bRegistered = acfParallelMessageFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// We store the parameters needed to define the specified initial state here.
// Note that the polymerise feature is not yet complete.

pmISVesicle::pmISVesicle() : mpmInitialState("vesicle"),
                             m_XC(0), m_YC(0), m_ZC(0), 
                             m_OuterRadius(0.0), m_Thickness(0.0)
{
	m_Polymers.clear();
	m_InteriorPolymers.clear();
	m_OuterFractions.clear();
	m_bPatches[0] = false;
	m_bPatches[1] = false;
}

pmISVesicle::pmISVesicle(const pmISVesicle& oldMessage) : mpmInitialState(oldMessage),
                         m_Polymers(oldMessage.m_Polymers),
                         m_InteriorPolymers(oldMessage.m_InteriorPolymers),
                         m_XC(oldMessage.m_XC), m_YC(oldMessage.m_YC), m_ZC(oldMessage.m_ZC), 
                         m_OuterRadius(oldMessage.m_OuterRadius),
                         m_Thickness(oldMessage.m_Thickness),
                         m_OuterFractions(oldMessage.m_OuterFractions)
{
	m_bPatches[0] = oldMessage.m_bPatches[0];
	m_bPatches[1] = oldMessage.m_bPatches[1];
}

pmISVesicle::~pmISVesicle()
{
}

// Member functions to read/write the data specific to the message.
//
// Arguments
// *********
//

zOutStream& pmISVesicle::put(zOutStream& os) const
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

zInStream& pmISVesicle::get(zInStream& is)
{
	return is;
}

// Non-static function to return the type of the message

const zString pmISVesicle::GetMessageType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current message.

const mpmMessage* pmISVesicle::GetMessage() const
{
	return new pmISVesicle(*this);
}

// ****************************************
// Function to copy all the required data from the CInitialStateData-derived object
// into the message. Note that each concrete message class knows the type of
// ISD that it expects, so it can perform the dynamic_cast safely.

void pmISVesicle::SetMessageData(const CInitialStateData* const pISD)
{
	const CInitialStateVesicle* const pData = dynamic_cast<const CInitialStateVesicle*>(pISD);

    m_Polymers.push_back(pData->GetPolymer());
    m_InteriorPolymers.push_back(pData->GetInteriorPolymer());

    m_XC             = pData->GetXCentre();
    m_YC             = pData->GetYCentre();
    m_ZC             = pData->GetZCentre();
    m_OuterRadius    = pData->GetOuterRadius();
    m_Thickness      = pData->GetThickness();
	m_OuterFractions.push_back(pData->GetOuterFraction());
    m_bPatches[0]    = pData->GetOuterPatches();
    m_bPatches[1]    = pData->GetInnerPatches();

}

// ****************************************
// Pure virtual function to check that the data are valid prior to sending a message.

bool pmISVesicle::Validate()
{
    bool bSuccess = true;

    if(m_Polymers.empty())
    {
        bSuccess = false;
    }
    else if(m_InteriorPolymers.empty())
    {
        bSuccess = false;
    }
    else if(m_XC < 0.0 || m_YC < 0.0 || m_ZC < 0.0 ||
		    m_XC > 1.0 || m_YC > 1.0 || m_ZC > 1.0)
    {
        bSuccess = false;
    }
    else if(m_OuterRadius < 1.0 || m_Thickness < 1.0)
    {
        bSuccess = false;
    }
    else
    {
		// Check all outer fractions sum to unity

		double sum = 0.0;
		for(zDoubleVectorIterator outer=m_OuterFractions.begin(); outer!=m_OuterFractions.end(); outer++)
		{
           sum += (*outer);
		}
		
		if(sum < 0.999 || sum > 1.001)
		{
            bSuccess = false;
		}
    }

    return bSuccess;
}

// Wrapper for the MPI function:
// MPI_Send(message, length, MPI_CHAR, m_ProcId, tag, MPI_COMM_WORLD);

void pmISVesicle::SendAllP()
{
#if SimMPS == SimulationEnabled
    // Declare a local variable that is a pointer to an MPI_Datatype to hold the 
    // message type;  note that if MPI is not compiled in, this data type defaults 
    // to a long.

    // We send the polymer's name as a separate message because MPI needs to use
    // fixed size arrays for string data.

    char stringPolymer[1000];
    strcpy(stringPolymer, m_Polymers.at(0).c_str());

    MPI_Datatype  MsgDataType;

    // Pack the data into a derived MPI data type 

    BuildDerivedType(&MsgDataType);

    // and send it to all the other processors: note that we assume that
    // the sending processor is P0, so we start the loop at processor rank 1.

    int tag1 = 0;
    int tag2 = 1;
    for(m_ProcId=1; m_ProcId<GetWorld(); m_ProcId++)
    {
        MPI_Send(stringPolymer, strlen(stringPolymer)+1, MPI_CHAR, m_ProcId, tag1, MPI_COMM_WORLD);
        MPI_Send(&m_XC, 1, MsgDataType, m_ProcId, tag2, MPI_COMM_WORLD);
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

void pmISVesicle::Receive()
{
#if SimMPS == SimulationEnabled

    char stringPolymer[1000];

    MPI_Datatype  MsgDataType;

    // Pack the data into a derived MPI data type

    BuildDerivedType(&MsgDataType);

    int tag1 = 0;
    int tag2 = 1;
    MPI_Status status;
    m_SenderRank = 0;

    MPI_Recv(stringPolymer, 1000, MPI_CHAR, m_SenderRank, tag1, MPI_COMM_WORLD, &status);
    MPI_Recv(&m_XC, 1, MsgDataType, m_SenderRank, tag2, MPI_COMM_WORLD, &status);

    // Convert the c-style polymer name into a zString
	m_Polymers.clear();
    m_Polymers.push_back(stringPolymer);


#endif
}

// Stub function for the overloaded Receive function to avoid compiler warnings on the Hermit1.hww.de platform.

void pmISVesicle::Receive(long procId)
{
#if SimMPS == SimulationEnabled

#endif
}

// PVF to assemble the disparate data items into a struct suitable for sending to 
// the MPI messaging functions. All the data must be in the calling object, and the
// data type is not usable outside this class.
// The SimMPS flag must be defined for this function to work, otherwise it does nothing.
//
// We represent the boolean values as longs with values 0/1 for use by MPI.

void pmISVesicle::BuildDerivedType(MsgDataTypePtr pMsgDataType)
{
#if SimMPS == SimulationEnabled

    // Store the number of elements of each type in the blocks

    const long dataTotal = 6;

    int block_lengths[6];

    for(long i=0; i<dataTotal; i++)
    {
        block_lengths[i] = 1;  
    }

    // Define the types of the blocks of elements

    MPI_Datatype typelist[6];
    
    for(long id=0; id<dataTotal; id++)
    {
        typelist[id] = MPI_DOUBLE;
    }

    // Define variables used to calculate the location of each element

    MPI_Aint displacements[6];


    // Local variables used to calculate the start of each element
    MPI_Aint startAddress;
    MPI_Aint address;

    MPI_Get_address(&m_XC,  &startAddress);
    displacements[0] = 0;

    MPI_Get_address(&m_YC,  &address);
    displacements[1] = address - startAddress;

    MPI_Get_address(&m_ZC,  &address);
    displacements[2] = address - startAddress;

    MPI_Get_address(&m_OuterRadius,  &address);
    displacements[3] = address - startAddress;

    MPI_Get_address(&m_Thickness,  &address);
    displacements[4] = address - startAddress;

    MPI_Get_address(&m_OuterFractions.at(0),  &address);
    displacements[5] = address - startAddress;

    // Build the derived datatype and commit it: we use the m_pMPIDataType member
    // variable in the base class to hold the new type and pass it to the
    // other member functions.

    MPI_Type_create_struct(dataTotal, block_lengths, displacements, typelist, pMsgDataType);
    MPI_Type_commit(pMsgDataType);

#endif
}

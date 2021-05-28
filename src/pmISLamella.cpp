/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// pmISLamella.cpp: implementation of the pmISLamella class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "pmISLamella.h"
#include "InitialStateLamella.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this message. 

const zString pmISLamella::m_Type = "lamella";

const zString pmISLamella::GetType()
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
	mpmMessage* Create() {return new pmISLamella();}

	const zString id = pmISLamella::GetType();

	const bool bRegistered = acfParallelMessageFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// We store the parameters needed to define the specified initial state here.
// Note that the polymerise feature is not yet complete.

pmISLamella::pmISLamella() : mpmInitialState("lamella"),
                             m_Polymer(""), m_bLinearise(false), 
                             m_X(0), m_Y(0), m_Z(0), 
                             m_Centre(0.0), m_Thickness(0.0), m_UpperFraction(0.0),
                             m_LongLinearise(0)
{
}

pmISLamella::pmISLamella(const pmISLamella& oldMessage) : mpmInitialState(oldMessage),
                         m_Polymer(oldMessage.m_Polymer),
                         m_bLinearise(oldMessage.m_bLinearise),
                         m_X(oldMessage.m_X), m_Y(oldMessage.m_Y), m_Z(oldMessage.m_Z), 
                         m_Centre(oldMessage.m_Centre),
                         m_Thickness(oldMessage.m_Thickness),
                         m_UpperFraction(oldMessage.m_UpperFraction),
                         m_LongLinearise(oldMessage.m_LongLinearise)
{
}

pmISLamella::~pmISLamella()
{
}

// Member functions to read/write the data specific to the message.
//
// Arguments
// *********
//

zOutStream& pmISLamella::put(zOutStream& os) const
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

zInStream& pmISLamella::get(zInStream& is)
{
	return is;
}

// Non-static function to return the type of the message

const zString pmISLamella::GetMessageType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current message.

const mpmMessage* pmISLamella::GetMessage() const
{
	return new pmISLamella(*this);
}

// ****************************************
// Function to copy all the required data from the CInitialStateData-derived object
// into the message. Note that each concrete message class knows the type of
// ISD that it expects, so it can perform the dynamic_cast safely.

void pmISLamella::SetMessageData(const CInitialStateData* const pISD)
{
	const CInitialStateLamella* const pData = dynamic_cast<const CInitialStateLamella*>(pISD);

    m_Polymer       = pData->GetPolymer();
    m_bLinearise    = pData->GetLinearise();
    m_X             = pData->GetXNormal();
    m_Y             = pData->GetYNormal();
    m_Z             = pData->GetZNormal();
    m_Centre        = pData->GetCentre();
    m_Thickness     = pData->GetThickness();
    m_UpperFraction = pData->GetUpperFraction();

    // Conver the booleans values into longs for use by MPI

    if(m_bLinearise)
    {
        m_LongLinearise = 1;
    }
    else
    {
        m_LongLinearise = 0;
    }

}

// ****************************************
// Pure virtual function to check that the data are valid prior to sending a message.

bool pmISLamella::Validate()
{
    bool bSuccess = true;

    if(m_Polymer.empty())
    {
        bSuccess = false;
    }
    else if(m_X == 0 && m_Y ==0 && m_Z == 0)
    {
        bSuccess = false;
    }
    else if(m_Centre < 0.0 || m_Thickness < 1.0)
    {
        bSuccess = false;
    }
    else if(m_UpperFraction < 0.0 || m_UpperFraction > 1.0)
    {
        bSuccess = false;
    }

    return bSuccess;
}

// Wrapper for the MPI function:
// MPI_Send(message, length, MPI_CHAR, m_ProcId, tag, MPI_COMM_WORLD);

void pmISLamella::SendAllP()
{
#if SimMPS == SimulationEnabled
    // Declare a local variable that is a pointer to an MPI_Datatype to hold the 
    // message type;  note that if MPI is not compiled in, this data type defaults 
    // to a long.
	
//	std::cout << "P0 sending following ISLamella data to all PN " << m_Polymer << " " << m_bLinearise << " " << m_X << " " << m_Y << " " << m_Z << " " << m_Centre << " " << m_Thickness << " " << m_UpperFraction << zEndl;
	

    // We send the polymer's name as a separate message because MPI needs to use
    // fixed size arrays for string data.

    char stringPolymer[1000];
    strcpy(stringPolymer, m_Polymer.c_str());

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
        MPI_Send(&m_LongLinearise, 1, MsgDataType, m_ProcId, tag2, MPI_COMM_WORLD);
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

void pmISLamella::Receive()
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
    MPI_Recv(&m_LongLinearise, 1, MsgDataType, m_SenderRank, tag2, MPI_COMM_WORLD, &status);

    // Convert the c-style polymer name into a zString
    m_Polymer = stringPolymer;

    // and the booleans

    if(m_LongLinearise == 1)
    {
        m_bLinearise = true;
    }
    else
    {
        m_bLinearise = false;
    }

//	std::cout << "P " << GlobalGetRank() << " receiving following ISLamella data to all PN " << m_Polymer << " " << m_bLinearise << " " << m_X << " " << m_Y << " " << m_Z << " " << m_Centre << " " << m_Thickness << " " << m_UpperFraction << zEndl;

#endif
}

// Stub function for the overloaded Receive function to avoid compiler warnings on the Hermit1.hww.de platform.

void pmISLamella::Receive(long procId)
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

void pmISLamella::BuildDerivedType(MsgDataTypePtr pMsgDataType)
{
#if SimMPS == SimulationEnabled

    // Store the number of elements of each type in the blocks

    const long dataTotal = 7;

    int block_lengths[7];

    for(long i=0; i<dataTotal; i++)
    {
        block_lengths[i] = 1;  
    }

    // Define the types of the blocks of elements

    MPI_Datatype typelist[7];

    for(long il=0; il<4; il++)
    {
        typelist[il] = MPI_LONG;
    }

    for(long id=4; id<7; id++)
    {
        typelist[id] = MPI_DOUBLE;
    }

    // Define variables used to calculate the location of each element

    MPI_Aint displacements[7];


    // Local variables used to calculate the start of each element
    MPI_Aint startAddress;
    MPI_Aint address;

//    long linearise  = 0; Commented out 7/2/11 I don't know what it is here for JCS

    MPI_Get_address(&m_LongLinearise, &startAddress);
    displacements[0] = 0;

    MPI_Get_address(&m_X,  &address);
    displacements[1] = address - startAddress;

    MPI_Get_address(&m_Y,  &address);
    displacements[2] = address - startAddress;

    MPI_Get_address(&m_Z,  &address);
    displacements[3] = address - startAddress;

    MPI_Get_address(&m_Centre,  &address);
    displacements[4] = address - startAddress;

    MPI_Get_address(&m_Thickness,  &address);
    displacements[5] = address - startAddress;

    MPI_Get_address(&m_UpperFraction,  &address);
    displacements[6] = address - startAddress;

    // Build the derived datatype and commit it: we use the m_pMPIDataType member
    // variable in the base class to hold the new type and pass it to the
    // other member functions.

    MPI_Type_create_struct(dataTotal, block_lengths, displacements, typelist, pMsgDataType);
    MPI_Type_commit(pMsgDataType);

#endif
}

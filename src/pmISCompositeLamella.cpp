/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// pmISCompositeLamella.cpp: implementation of the pmISCompositeLamella class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "pmISCompositeLamella.h"
#include "InitialStateCompositeLamella.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this message. 

const zString pmISCompositeLamella::m_Type = "compositelamella";

const zString pmISCompositeLamella::GetType()
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
	mpmMessage* Create() {return new pmISCompositeLamella();}

	const zString id = pmISCompositeLamella::GetType();

	const bool bRegistered = acfParallelMessageFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// We store the parameters needed to define the specified initial state here.
// Note that the polymerise feature is not yet complete.

pmISCompositeLamella::pmISCompositeLamella() : mpmInitialState("compositelamella"),
                             m_X(0), m_Y(0), m_Z(0), 
                             m_Centre(0.0), m_Thickness(0.0), 
							 m_bLinearise(false), m_LongLinearise(0), m_LongUpperPatches(0), m_LongLowerPatches(0)
{
	m_Polymers.clear();
	m_UpperFractions.clear();
	
	m_bPatches[0] = false;
	m_bPatches[1] = false;
}

pmISCompositeLamella::pmISCompositeLamella(const pmISCompositeLamella& oldMessage) : mpmInitialState(oldMessage),
                         m_Polymers(oldMessage.m_Polymers),
                         m_UpperFractions(oldMessage.m_UpperFractions),
                         m_X(oldMessage.m_X), m_Y(oldMessage.m_Y), m_Z(oldMessage.m_Z), 
                         m_Centre(oldMessage.m_Centre),
                         m_Thickness(oldMessage.m_Thickness),
                         m_bLinearise(oldMessage.m_bLinearise),
                         m_LongLinearise(oldMessage.m_LongLinearise),
                         m_LongUpperPatches(oldMessage.m_LongUpperPatches),
                         m_LongLowerPatches(oldMessage.m_LongLowerPatches)
{
	m_bPatches[0] = oldMessage.m_bPatches[0];
	m_bPatches[1] = oldMessage.m_bPatches[1];
}

pmISCompositeLamella::~pmISCompositeLamella()
{
}

// Member functions to read/write the data specific to the message.
//
// Arguments
// *********
//

zOutStream& pmISCompositeLamella::put(zOutStream& os) const
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

zInStream& pmISCompositeLamella::get(zInStream& is)
{
	return is;
}

// Non-static function to return the type of the message

const zString pmISCompositeLamella::GetMessageType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current message.

const mpmMessage* pmISCompositeLamella::GetMessage() const
{
	return new pmISCompositeLamella(*this);
}

// ****************************************
// Function to copy all the required data from the CInitialStateData-derived object
// into the message. Note that each concrete message class knows the type of
// ISD that it expects, so it can perform the dynamic_cast safely.

void pmISCompositeLamella::SetMessageData(const CInitialStateData* const pISD)
{
	const CInitialStateCompositeLamella* const pData = dynamic_cast<const CInitialStateCompositeLamella*>(pISD);

    m_Polymers       = pData->GetPolymers();
    m_UpperFractions = pData->GetUpperFractions();
    m_X              = pData->GetXNormal();
    m_Y              = pData->GetYNormal();
    m_Z              = pData->GetZNormal();
    m_Centre         = pData->GetCentre();
    m_Thickness      = pData->GetThickness();
    m_bLinearise     = pData->GetLinearise();
	m_bPatches[0]    = pData->GetUpperPatchesFlag();
	m_bPatches[1]    = pData->GetLowerPatchesFlag();

    // Conver the booleans values into longs for use by MPI

    if(m_bLinearise)
    {
        m_LongLinearise = 1;
    }
    else
    {
        m_LongLinearise = 0;
    }
	
    if(m_bPatches[0])
    {
        m_LongUpperPatches = 1;
    }
    else
    {
        m_LongUpperPatches = 0;
    }
	
    if(m_bPatches[1])
    {
        m_LongLowerPatches = 1;
    }
    else
    {
        m_LongLowerPatches = 0;
    }
}

// ****************************************
// Pure virtual function to check that the data are valid prior to sending a message.

bool pmISCompositeLamella::Validate()
{
    bool bSuccess = true;
	
    if(m_Polymers.empty() || m_UpperFractions.empty() || m_Polymers.size() != m_UpperFractions.size())
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
    else 
    {
	    for(long unsigned int ip=0; ip<m_UpperFractions.size(); ip++)
		{
		    if(m_UpperFractions.at(ip) < 0.0 || m_UpperFractions.at(ip) > 1.0)
			{
                bSuccess = false;
			}
		}
    }
	
    return bSuccess;
}

// Wrapper for the MPI function:
// MPI_Send(message, length, MPI_CHAR, m_ProcId, tag, MPI_COMM_WORLD);

void pmISCompositeLamella::SendAllP()
{
#if SimMPS == SimulationEnabled
    // We send the polymer names and upper fractions as separate messages as we don't know how many are needed.
	// We allow for up to 20000 characters for now: this is approximately
    // 100 x 100 character polymer names.

    char buffer[20000];
    int  position;
    position = 0;

    long total = m_Polymers.size();
	
	// First, pack the total number of polymer names to be sent

    MPI_Pack(&total, 1, MPI_LONG, buffer, 20000, &position, MPI_COMM_WORLD);

    // Now iterate over the polymer names and pack the number of characters
    // and the name itself, and its corresponding upper monolayer fraction into the message one by one

    char cname[100];

    long iFraction=0;
    for(cStringSequenceIterator is=m_Polymers.begin(); is!=m_Polymers.end(); is++)
    {
        const zString name = *is;
        strcpy(cname, name.c_str());
        long length = name.size()+1;
		double upperFraction = m_UpperFractions.at(iFraction++);

        MPI_Pack(&length,        1, MPI_LONG,   buffer, 20000, &position, MPI_COMM_WORLD);
        MPI_Pack(&cname, length,    MPI_CHAR,   buffer, 20000, &position, MPI_COMM_WORLD);
		MPI_Pack(&upperFraction, 1, MPI_DOUBLE, buffer, 20000, &position, MPI_COMM_WORLD);
    }

    // Assemble the remaining data, which is easier as it has a fixed size
    // Declare a local variable that is a pointer to an MPI_Datatype to hold the 
    // message type;  note that if MPI is not compiled in, this data type defaults 
    // to a long.
		
    MPI_Datatype  MsgDataType;

    BuildDerivedType(&MsgDataType);

    // Send the data to all the other processors: we assume the sending processor is P0, so we start the loop at 1
	
    int tag1 = 0;
    int tag2 = 1;
	
    for(m_ProcId=1; m_ProcId<GetWorld(); m_ProcId++)
    {
        MPI_Send(buffer, 20000, MPI_PACKED,  m_ProcId, tag1, MPI_COMM_WORLD);
        MPI_Send(&m_X, 1, MsgDataType, m_ProcId, tag2, MPI_COMM_WORLD);
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

void pmISCompositeLamella::Receive()
{
#if SimMPS == SimulationEnabled

    char buffer[20000];
    int  position;

    long total;
    long length = 0;
	double upperFraction = 0.0;
    char cname[100];
    zString name;

    MPI_Datatype  MsgDataType;

    // Pack the data into a derived MPI data type

    BuildDerivedType(&MsgDataType);

    int tag1 = 0;
    int tag2 = 1;
    MPI_Status status;
    m_SenderRank = 0;

    MPI_Recv(buffer, 20000, MPI_PACKED,  m_SenderRank, tag1, MPI_COMM_WORLD, &status);
    MPI_Recv(&m_X,   1,     MsgDataType, m_SenderRank, tag2, MPI_COMM_WORLD, &status);

    // Unpack the data into member variables

    position = 0;
    MPI_Unpack(buffer, 20000, &position, &total, 1, MPI_LONG, MPI_COMM_WORLD);

    m_Polymers.clear();
	m_UpperFractions.clear();
	
    for(long i=0; i<total; i++)
    {
        MPI_Unpack(buffer, 20000, &position, &length, 1, MPI_LONG, MPI_COMM_WORLD);
        MPI_Unpack(buffer, 20000, &position, &cname, length, MPI_CHAR, MPI_COMM_WORLD);
        MPI_Unpack(buffer, 20000, &position, &upperFraction, 1, MPI_DOUBLE, MPI_COMM_WORLD);

        name = cname;
        m_Polymers.push_back(name);
		m_UpperFractions.push_back(upperFraction);
    }

    // Convert the booleans

    if(m_LongLinearise == 1)
    {
        m_bLinearise = true;
    }
    else
    {
        m_bLinearise = false;
    }

    if(m_LongUpperPatches == 1)
    {
        m_bPatches[0] = true;
    }
    else
    {
        m_bPatches[0] = false;
    }

    if(m_LongLowerPatches == 1)
    {
        m_bPatches[1] = true;
    }
    else
    {
        m_bPatches[1] = false;
    }

#endif
}

// Stub function for the overloaded Receive function to avoid compiler warnings on the Hermit1.hww.de platform.

void pmISCompositeLamella::Receive(long procId)
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

void pmISCompositeLamella::BuildDerivedType(MsgDataTypePtr pMsgDataType)
{
#if SimMPS == SimulationEnabled

    // Store the number of elements of each type in the blocks

    const long dataTotal = 8;

    int block_lengths[8];

    for(long i=0; i<dataTotal; i++)
    {
        block_lengths[i] = 1;  
    }

    // Define the types of the blocks of elements

    MPI_Datatype typelist[8];

    for(long il=0; il<6; il++)
    {
        typelist[il] = MPI_LONG;
    }

    for(long id=6; id<8; id++)
    {
        typelist[id] = MPI_DOUBLE;
    }

    // Define variables used to calculate the location of each element

    MPI_Aint displacements[8];

    // Local variables used to calculate the start of each element
	
    MPI_Aint startAddress;
    MPI_Aint address;

    MPI_Get_address(&m_X,  &startAddress);
    displacements[0] = 0;

    MPI_Get_address(&m_Y,  &address);
    displacements[1] = address - startAddress;

    MPI_Get_address(&m_Z,  &address);
    displacements[2] = address - startAddress;

    MPI_Get_address(&m_LongLinearise, &address);
    displacements[3] = address - startAddress;

    MPI_Get_address(&m_LongUpperPatches, &address);
    displacements[4] = address - startAddress;

    MPI_Get_address(&m_LongLowerPatches, &address);
    displacements[5] = address - startAddress;

    MPI_Get_address(&m_Centre,  &address);
    displacements[6] = address - startAddress;

    MPI_Get_address(&m_Thickness,  &address);
    displacements[7] = address - startAddress;

    // Build the derived datatype and commit it: we use the m_pMPIDataType member
    // variable in the base class to hold the new type and pass it to the
    // other member functions.

    MPI_Type_create_struct(dataTotal, block_lengths, displacements, typelist, pMsgDataType);
    MPI_Type_commit(pMsgDataType);

#endif
}

/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// pmInputData.cpp: implementation of the pmInputData class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "pmInputData.h"
#include "InputData.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this message. 

const zString pmInputData::m_Type = "InputData";

const zString pmInputData::GetType()
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
	mpmMessage* Create() {return new pmInputData();}

	const zString id = pmInputData::GetType();

	const bool bRegistered = acfParallelMessageFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// We store all parameters for all simulation types here, but conditionally
// compile the functions that access them depending on the SimIdentifier flag.

pmInputData::pmInputData() : mpmMessage(),
                             m_BeadTypes(0), m_BondTypes(0), m_BondPairTypes(0), m_PolymerTypes(0),
                             m_RNGSeed(0), m_TotalTime(0), 
                             m_SamplePeriod(0),  m_AnalysisPeriod(0), m_DensityPeriod(0),
                             m_DisplayPeriod(0), m_RestartPeriod(0),
                             m_ProcessorsXNo(0), m_ProcessorsYNo(0), m_ProcessorsZNo(0),
                             m_CNTXCellNo(0),    m_CNTYCellNo(0),    m_CNTZCellNo(0),
                             m_GridXCellNo(0),   m_GridYCellNo(0),   m_GridZCellNo(0),
                             m_BeadDensity(0.0), m_Temperature(0.0), m_StepSize(0.0),
                             m_CNTXCellWidth(0.0),  m_CNTYCellWidth(0.0),  m_CNTZCellWidth(0.0),
                             m_Lambda(0.0), 
                             m_TotalMCTime(0), m_RCutOff(0.0), m_MCStepSize(0.0)
{
}

pmInputData::pmInputData(const pmInputData& oldMessage) : mpmMessage(oldMessage)
{
}

pmInputData::~pmInputData()
{
}

// Member functions to read/write the data specific to the message.
//
// Arguments
// *********
//

zOutStream& pmInputData::put(zOutStream& os) const
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

zInStream& pmInputData::get(zInStream& is)
{
	return is;
}

// Non-static function to return the type of the message

const zString pmInputData::GetMessageType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current message.

const mpmMessage* pmInputData::GetMessage() const
{
	return new pmInputData(*this);
}

// ****************************************
// Function to copy all the required data from the CInputData object
// into the message. Note that this message can only be sent from P0
// to PN as it relies on P0 reading in the bead types, etc, so that it 
// can send the expected number of datatypes to the PN.

void pmInputData::SetMessageData(const CInputData *const pData)
{
    m_BeadTypes         =  pData->GetBeadTypeTotal();
    m_BondTypes         =  pData->GetBondTypeTotal();
    m_BondPairTypes     =  pData->GetBondPairTypeTotal();
    m_PolymerTypes      =  pData->GetPolymerTypeTotal();
    m_RNGSeed           =  pData->GetRNGSeed();
    m_TotalTime         =  pData->GetTotalTime();
    m_SamplePeriod      =  pData->GetSamplePeriod();
    m_AnalysisPeriod    =  pData->GetAnalysisPeriod();
    m_DensityPeriod     =  pData->GetDensityPeriod();
    m_DisplayPeriod     =  pData->GetDisplayPeriod();
    m_RestartPeriod     =  pData->GetRestartPeriod();
    m_BeadDensity       =  pData->GetAveBeadDensity();
    m_Temperature       =  pData->GetkT();
    m_StepSize          =  pData->GetStepSize();

    m_ProcessorsXNo     =  pData->GetProcessorsXNo();
    m_ProcessorsYNo     =  pData->GetProcessorsYNo();
    m_ProcessorsZNo     =  pData->GetProcessorsZNo();

    m_CNTXCellNo        =  pData->GetCNTXCellNo();
    m_CNTYCellNo        =  pData->GetCNTYCellNo();
    m_CNTZCellNo        =  pData->GetCNTZCellNo();

    m_CNTXCellWidth     =  pData->GetCNTXCellWidth();
    m_CNTYCellWidth     =  pData->GetCNTYCellWidth();
    m_CNTZCellWidth     =  pData->GetCNTZCellWidth();

    m_GridXCellNo       =  pData->GetGridXCellNo();
    m_GridYCellNo       =  pData->GetGridYCellNo();
    m_GridZCellNo       =  pData->GetGridZCellNo();

    // Simulation type-dependent parameters

#if SimIdentifier == BD
    m_Lambda            = pData->GetLambda();
#elif SimIdentifier == DPD
    m_Lambda            = pData->GetLambda();
#elif SimIdentifier == MD
    m_TotalMCTime       = pData->GetTotalMCTime();
    m_RCutOff           = pData->GetCutOffRadius();
    m_MCStepSize        = pData->GetMCStepSize();
#endif
}

// ****************************************
// Pure virtual function to check that the data are valid prior to sending a message.

bool pmInputData::Validate()
{
    bool bSuccess = true;

    if(m_BeadTypes <= 0)
    {
        bSuccess = false;
    }
    else if(m_BondTypes < 0)  // Zero bond types is allowed
    {
        bSuccess = false;
    }
    else if(m_BondPairTypes < 0)  // Zero bondpair types is allowed
    {
        bSuccess = false;
    }
    else if(m_PolymerTypes <= 0)
    {
        bSuccess = false;
    }
    else if(m_RNGSeed >= 0)
    {
        bSuccess = false;
    }
    else if(m_BeadDensity <= 0.0)
    {
        bSuccess = false;
    }
    else if(m_Temperature < 0.0)
    {
        bSuccess = false;
    }
    else if(m_StepSize < 0.000001)
    {
         bSuccess = false;
    }
    else if(m_SamplePeriod < 1 || m_TotalTime < m_SamplePeriod)
    {
        bSuccess = false;
    }
	else if(m_AnalysisPeriod < 1 || (m_AnalysisPeriod%m_SamplePeriod != 0)
								 ||  m_AnalysisPeriod > m_TotalTime)
    {
        bSuccess = false;
    }
    else if(m_DensityPeriod < 1 ||(m_DensityPeriod%m_SamplePeriod != 0)
						        || m_DensityPeriod > m_TotalTime)
    {
        bSuccess = false;
    }
    else  if(m_DisplayPeriod < 1 || m_DisplayPeriod > m_TotalTime)
    {
        bSuccess = false;
    }
    else if(m_RestartPeriod < 1 || m_RestartPeriod > m_TotalTime)
    {
        bSuccess = false;
    }
    else if(m_ProcessorsXNo < 1 || m_ProcessorsYNo < 1 || m_ProcessorsZNo < 1 ||
            m_ProcessorsXNo*m_ProcessorsYNo*m_ProcessorsZNo > GetWorld())
    {
        bSuccess = false;
    }
    else if(m_CNTXCellNo < 1 || m_CNTYCellNo < 1 || m_CNTZCellNo < 1)
    {
        bSuccess = false;
    }
    else if(m_CNTXCellWidth < 1.0 || m_CNTYCellWidth < 1.0 || m_CNTZCellWidth < 1.0)
    {
        bSuccess = false;
    }
    else if(m_GridXCellNo < 1 || m_GridYCellNo < 1 || m_GridZCellNo < 1)
    {
        bSuccess = false;
    }

#if SimIdentifier == BD
    if(m_Lambda < 0.0 || m_Lambda > 1.0)
    {
        bSuccess = false;
    }
#elif SimIdentifier == DPD
    if(m_Lambda < 0.0 || m_Lambda > 1.0)
    {
        bSuccess = false;
    }
#elif SimIdentifier == MD
    if(m_TotalMCTime < 0 || !(m_RCutOff > 0.0) || m_MCStepSize < 0.000001)
    {
        bSuccess = false;
    }

#endif

    return bSuccess;
}

// Wrapper for the MPI function:
// MPI_Send(message, length, MPI_CHAR, m_ProcId, tag, MPI_COMM_WORLD);

void pmInputData::SendAllP()
{
#if SimMPS == SimulationEnabled

	// Conver the m_RNGSeed to a positive number before packing it to avoid
	// problems with OpenMPI versions 1.1 and 1.2.that appear to send an MPI_INT
	// as unsigned and signed respectively.

    if(m_RNGSeed < 0)
	{
		m_RNGSeed *= -1;
	}

    // Declare a local variable that is an MPI_Datatype to hold the message;  
    // note that if MPI is not compiled in, this data type defaults 
    // to a long.

    MPI_Datatype  MsgDataType;

    // Pack the data into a derived MPI data type 

    BuildDerivedType(&MsgDataType);

    // and send it to all the other processors: note that we assume that
    // the sending processor is P0, so we start the loop at processor rank 1.

    for(m_ProcId=1; m_ProcId<GetWorld(); m_ProcId++)
    {
        MPI_Send(&m_BeadTypes, 1, MsgDataType, m_ProcId, 0, MPI_COMM_WORLD);
    }

	// Convert the m_RNGSeed back to a negative number if necessary.

	if(m_RNGSeed > 0)
	{
		m_RNGSeed *= -1;
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

void pmInputData::Receive()
{
#if SimMPS == SimulationEnabled

    MPI_Datatype  MsgDataType;

    // Pac the data into a derived MPI data type

    BuildDerivedType(&MsgDataType);

    int tag = 0;
    MPI_Status status;

    MPI_Recv(&m_BeadTypes, 1, MsgDataType, 0, tag, MPI_COMM_WORLD, &status);

    // Modify the RNG seed for each receiving processor so that we don't use
    // the same sequence of random numbers on all of them. We just subtract the rank
    // from the original seed. We also ensure that it is negative to start with.

	if(m_RNGSeed > 0)
	{
		m_RNGSeed *= -1;
	}

    m_RNGSeed -= GetRank();

#endif
}

// Stub function for the overloaded Receive function to avoid compiler warnings on the Hermit1.hww.de platform.

void pmInputData::Receive(long procId)
{
#if SimMPS == SimulationEnabled

#endif
}

// PVF to assemble the disparate data items into a struct suitable for sending to 
// the MPI messaging functions. All the data must be in the calling object, and the
// data type is not usable outside this class.
// The SimMPS flag must be defined for this function to work, otherwise it does nothing.

void pmInputData::BuildDerivedType(MsgDataTypePtr pMsgDataType)
{
#if SimMPS == SimulationEnabled

    // Store the number of elements of each type in the blocks

    const long dataTotal = 27;

    int block_lengths[27];

    for(long i=0; i<dataTotal; i++)
    {
        block_lengths[i] = 1;  
    }

    // Define the types of the blocks

    MPI_Datatype typelist[27];

    for(long il = 0; il < 20; il++)
    {
        typelist[il] = MPI_INT;
    }

    for(long id = 20; id < 27; id++)
    {
        typelist[id] = MPI_DOUBLE;
    }

    // Define the displacement of each element from the start of the new type

    MPI_Aint displacements[27];

    // Local variables used to calculate the start of each element
    MPI_Aint startAddress;
    MPI_Aint address;

    MPI_Get_address(&m_BeadTypes, &startAddress);
    displacements[0] = 0;

    MPI_Get_address(&m_BondTypes, &address);
    displacements[1] = address - startAddress;

    MPI_Get_address(&m_BondPairTypes, &address);
    displacements[2] = address - startAddress;

    MPI_Get_address(&m_PolymerTypes, &address);
    displacements[3] = address - startAddress;

    MPI_Get_address(&m_RNGSeed, &address);
    displacements[4] = address - startAddress;

    MPI_Get_address(&m_TotalTime,  &address);
    displacements[5] = address - startAddress;

    MPI_Get_address(&m_SamplePeriod,  &address);
    displacements[6] = address - startAddress;

    MPI_Get_address(&m_AnalysisPeriod,  &address);
    displacements[7] = address - startAddress;

    MPI_Get_address(&m_DensityPeriod,  &address);
    displacements[8] = address - startAddress;

    MPI_Get_address(&m_DisplayPeriod,  &address);
    displacements[9] = address - startAddress;

    MPI_Get_address(&m_RestartPeriod,  &address);
    displacements[10] = address - startAddress;

    MPI_Get_address(&m_ProcessorsXNo,  &address);
    displacements[11] = address - startAddress;

    MPI_Get_address(&m_ProcessorsYNo,  &address);
    displacements[12] = address - startAddress;

    MPI_Get_address(&m_ProcessorsZNo,  &address);
    displacements[13] = address - startAddress;

    MPI_Get_address(&m_CNTXCellNo,  &address);
    displacements[14] = address - startAddress;

    MPI_Get_address(&m_CNTYCellNo,  &address);
    displacements[15] = address - startAddress;

    MPI_Get_address(&m_CNTZCellNo,  &address);
    displacements[16] = address - startAddress;

    MPI_Get_address(&m_GridXCellNo,  &address);
    displacements[17] = address - startAddress;

    MPI_Get_address(&m_GridYCellNo,  &address);
    displacements[18] = address - startAddress;

    MPI_Get_address(&m_GridZCellNo,  &address);
    displacements[19] = address - startAddress;

    MPI_Get_address(&m_BeadDensity,  &address);
    displacements[20] = address - startAddress;

    MPI_Get_address(&m_Temperature,  &address);
    displacements[21] = address - startAddress;

    MPI_Get_address(&m_StepSize,  &address);
    displacements[22] = address - startAddress;

    MPI_Get_address(&m_CNTXCellWidth,  &address);
    displacements[23] = address - startAddress;

    MPI_Get_address(&m_CNTYCellWidth,  &address);
    displacements[24] = address - startAddress;

    MPI_Get_address(&m_CNTZCellWidth,  &address);
    displacements[25] = address - startAddress;

#if SimIdentifier == BD
    MPI_Get_address(&m_Lambda,  &address);
    displacements[26] = address - startAddress;
#elif SimIdentifier == DPD
    MPI_Get_address(&m_Lambda,  &address);
    displacements[26] = address - startAddress;
#elif SimIdentifier == MD
// Not implemented yet
#endif

    // Build the derived datatype and commit it

    MPI_Type_create_struct(dataTotal, block_lengths, displacements, typelist, pMsgDataType);
    MPI_Type_commit(pMsgDataType);

#endif
}

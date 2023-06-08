/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// CurrentState.cpp: implementation of the CCurrentState class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMPSFlags.h"
#include "CurrentState.h"
#include "CurrentStateFormat.h"
#include "ISimBox.h"
#include "InputData.h"
#include "Bead.h"
#include "Polymer.h"
#include "Bond.h"

// **********************************************************************
// Global Functions and members
//
// Static member function to clear the map prior to use.

LongLongMap CCurrentState::m_mBeadDisplayId;

void CCurrentState::ClearBeadDisplayIdMap()
{
	m_mBeadDisplayId.clear();
}

// Static member function to return the number of beads whose ids are stored.

long CCurrentState::GetBeadDisplayIdMapSize()
{
	return m_mBeadDisplayId.size();
}

// Static member function to return a reference to the map.

LongLongMap& CCurrentState::GetMap()
{
	return m_mBeadDisplayId;
}

// Static member function to return a bead's display id given its bead id.
// If the bead id is not a valid key, we return -1. This should be checked
// for in the calling routine.

long CCurrentState::GetBeadDisplayId(long beadId)
{
	if(m_mBeadDisplayId.find(beadId) != m_mBeadDisplayId.end())
		return (*m_mBeadDisplayId.find(beadId)).second;
	else
		return -1;
}

// Static member function to add a (beadId, displayId) pair to the map.

void CCurrentState::SetBeadDisplayId(long beadId, long displayId)
{
	// If the beadId does not already exist in the map as a key we add the new
	// (beadId, displayId) pair; if it does, we replace the old display Id with the new one.

	if(CCurrentState::GetBeadDisplayId(beadId) == -1)
	{
		m_mBeadDisplayId.insert(zPairLL(beadId,displayId));
	}
	else
	{
		m_mBeadDisplayId.erase(beadId);
		m_mBeadDisplayId.insert(zPairLL(beadId,displayId));
	}
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Constructor to use a CCurrentStateFormat object to output bead coordinates
// in the format required by the associated visualisation tool.
//
// Beads are excluded from the output if their internal visibility flag is set to false.
// The set of beads that are excluded by their type is stored in the CMonitor. 
// Beads in command targets can have their visibility toggled by command. The 
// bounding box may be toggled on and off in Povray snapshots; and a range can be 
// specified for the x, y, and z axes so that only beads whose coordinates lie 
// within the range are written to the output file. Note that the limits of the ranges 
// are specified as fractions of the SimBox side lengths in the constructor argument 
// list but converted to absolute units in the member variable initialisation list.
//
// This function works for both the serial code and the parallel code. We use
// the ISimState functions GetSimSpaceXLength() etc, to obtain the size of the
// (unique) SimBox in the serial code, and the whole simulation Space in the 
// parallel code. The bead coordinates are relative to the simulation Space,
// and the display bead ranges are converted to absolute values here.


CCurrentState::CCurrentState(const long currentTime, const zString runId, 
							 const ISimBox*  pISimBox,
							 CCurrentStateFormat* const pFormat,
							 bool bRestrictCoords,
							 double xmin, double ymin, double zmin,
                             double xmax, double ymax, double zmax) : xxState(xxBase::GetCSPrefix() + runId + ".con." + ToString(currentTime) + pFormat->GetFileExtension(), true, currentTime, runId),
															 m_pFormat(pFormat),
															 m_vAllBeads(pISimBox->GetBeads()),
															 m_vAllPolymers(pISimBox->GetPolymers()),
															 m_bRestrictCoords(bRestrictCoords),
															 m_SimBoxXLength(pISimBox->GetSimSpaceXLength()),
															 m_SimBoxYLength(pISimBox->GetSimSpaceYLength()),
															 m_SimBoxZLength(pISimBox->GetSimSpaceZLength()),
															 m_XMin(xmin*m_SimBoxXLength),
															 m_YMin(ymin*m_SimBoxYLength),
															 m_ZMin(zmin*m_SimBoxZLength),
															 m_XMax(xmax*m_SimBoxXLength),
															 m_YMax(ymax*m_SimBoxYLength),
															 m_ZMax(zmax*m_SimBoxZLength)
{



    // Store the bead names so that the format classes can use them instead of
    // the numeric types if needed.

    for(int type=0; type<pISimBox->GetBeadTypeTotal(); type++)
    {
        zString name = pISimBox->GetBeadNameFromType(type);
        m_BeadNames.push_back(name);
    }
}

CCurrentState::~CCurrentState()
{
	if(m_pFormat)
	{
		delete m_pFormat;
		m_pFormat = 0;
	}
}

// Serialize function to use the CCurrentStateFormat objects to output the
// data according to the type of visualisation tool being used.  Each format
// class implements the interface specified by the CCurrentStateFormat base
// class, and data common to all formats is stored in the base class.
//
// Two formats are currently recognised: Povray and Amira. We first write
// out the header information required by each format class, then
// we loop over the beads and pass their their coordinates to the format object.
// Note that if there are coordinate restrictions set for any axis, we use a
// separate loop from the unrestricted case. This is to speed up the output when
// no coordinate range is being used.

bool CCurrentState::Serialize()
{
	// Write out header information including the number of beads to draw

	m_pFormat->SerializeHeader(m_outStream, CountBeadsDisplayed());

// Bead types are excluded from the output files if their visibility flag is off.
// This is typically used to exlude solvent particles from display.
// If there are no restrictions on the range of bead coordinates to be
// displayed, we use a simple form of the loop; if any axis has a specified
// range then we use a longer form that checks each coordinate.
//
// NOTE. Bead coordinates must lie within the range, not equal to one of the
// endpoints: is this sufficient or are some beads missed?

	if(m_bRestrictCoords)
	{
		for(cAbstractBeadVectorIterator iterBead=m_vAllBeads.begin(); iterBead!=m_vAllBeads.end(); iterBead++)
		{
			// Use each bead's display id to set its colour, but keep its type
			// to determine if the bead type should be displayed at all

			const long      type = (*iterBead)->GetType();
			const zString   name = m_BeadNames.at(type);
			const long displayId = CCurrentState::GetBeadDisplayId((*iterBead)->GetId());
			const double  radius = (*iterBead)->GetRadius();
			const double       x = (*iterBead)->GetXPos();
			const double       y = (*iterBead)->GetYPos();
			const double       z = (*iterBead)->GetZPos();

			if((*iterBead)->GetVisible() &&
			   m_XMin < x && x < m_XMax &&
			   m_YMin < y && y < m_YMax &&
			   m_ZMin < z && z < m_ZMax)
			{
				m_pFormat->SerializeBead(m_outStream, name, displayId, radius, x, y, z);

				if(!m_outStream.good())
					return IOError("Error writing CurrentState data to file");
			}
		}
	}
	else
	{
		for(cAbstractBeadVectorIterator iterBead=m_vAllBeads.begin(); iterBead!=m_vAllBeads.end(); iterBead++)
		{
			if((*iterBead)->GetVisible())
			{
			    const long      type = (*iterBead)->GetType();
			    const zString   name = m_BeadNames.at(type);
				const long   displayId = CCurrentState::GetBeadDisplayId((*iterBead)->GetId());
				const double    radius = (*iterBead)->GetRadius();
				const double         x = (*iterBead)->GetXPos();
				const double         y = (*iterBead)->GetYPos();
				const double         z = (*iterBead)->GetZPos();

				m_pFormat->SerializeBead(m_outStream, name, displayId, radius, x, y, z);

				if(!m_outStream.good())
					return IOError("Error writing CurrentState data to file");
			}
		}
	}

	if(m_pFormat->UsesBonds()){
		for(const CPolymer *polymer : m_vAllPolymers){
			for(const CBond *bond : polymer->GetBonds()){
				const auto *head=bond->GetHead();
				const auto *tail=bond->GetTail();
				if(!(head->GetVisible()&&tail->GetVisible())){
					continue;
				}
				
				m_pFormat->SerializeBond(m_outStream, *polymer, *bond);
			}
		}
	}

    // Write out footer information if the format class requires it.
    // Otherwise, the base class' do-nothing function is invoked.

	m_pFormat->SerializeFooter(m_outStream, CountBeadsDisplayed());

	// Close the file so that we can view it before the simulation has ended

	m_outStream << zFlush;

	return Close();
}

// Private helper function to count the number of beads to be drawn in the
// output file. We perform the output loop but only count those beads not excluded 
// by type or coordinate range.

long CCurrentState::CountBeadsDisplayed() const
{
	long displayTotal = 0;

	if(m_bRestrictCoords)
	{
		for(cAbstractBeadVectorIterator iterBead=m_vAllBeads.begin(); iterBead!=m_vAllBeads.end(); iterBead++)
		{
			const double         x = (*iterBead)->GetXPos();
			const double         y = (*iterBead)->GetYPos();
			const double         z = (*iterBead)->GetZPos();

			if((*iterBead)->GetVisible() &&
			   m_XMin < x && x < m_XMax &&
			   m_YMin < y && y < m_YMax &&
			   m_ZMin < z && z < m_ZMax)
			{
				displayTotal++;
			}
		}
	}
	else
	{
		for(cAbstractBeadVectorIterator iterBead=m_vAllBeads.begin(); iterBead!=m_vAllBeads.end(); iterBead++)
		{
			if((*iterBead)->GetVisible())
			{
				displayTotal++;
			}
		}
	}

	return displayTotal;
}

// Function to write out the coordinates of beads on P0 during a parallel simulation.
// This function starts the snapshot serialization, and each PN must call
// the SerializePN function to add its bead coordinate data to the snapshot file,
// and finally the SerializeEnd() function must be called to close the file.

bool CCurrentState::SerializeP0()
{
#if EnableParallelMonitor == SimMPSEnabled

	// Write out header information including the number of beads to draw

	m_pFormat->SerializeHeader(m_outStream, CountBeadsDisplayed());

// Bead types are excluded from the output files if their visibility flag is off.
// This is typically used to exlude solvent particles from display.
// If there are no restrictions on the range of bead coordinates to be
// displayed, we use a simple form of the loop; if any axis has a specified
// range then we use a longer form that checks each coordinate.
//
// NOTE. Bead coordinates must lie within the range, not equal to one of the
// endpoints: is this sufficient or are some beads missed?

	if(m_bRestrictCoords)
	{
		for(cAbstractBeadVectorIterator iterBead=m_vAllBeads.begin(); iterBead!=m_vAllBeads.end(); iterBead++)
		{
			// Use each bead's display id to set its colour, but keep its type
			// to determine if the bead type should be displayed at all

			const long      type = (*iterBead)->GetType();
			const zString   name = m_BeadNames.at(type);
			const long displayId = CCurrentState::GetBeadDisplayId((*iterBead)->GetId());
			const double  radius = (*iterBead)->GetRadius();
			const double       x = (*iterBead)->GetXPos();
			const double       y = (*iterBead)->GetYPos();
			const double       z = (*iterBead)->GetZPos();

			if((*iterBead)->GetVisible() &&
			   m_XMin < x && x < m_XMax &&
			   m_YMin < y && y < m_YMax &&
			   m_ZMin < z && z < m_ZMax)
			{
				m_pFormat->SerializeBead(m_outStream, name, displayId, radius, x, y, z);

				if(!m_outStream.good())
					return IOError("Error writing CurrentState data to file");
			}
		}
	}
	else
	{
		for(cAbstractBeadVectorIterator iterBead=m_vAllBeads.begin(); iterBead!=m_vAllBeads.end(); iterBead++)
		{
			if((*iterBead)->GetVisible())
			{
			    const long      type = (*iterBead)->GetType();
			    const zString   name = m_BeadNames.at(type);
				const long   displayId = CCurrentState::GetBeadDisplayId((*iterBead)->GetId());
				const double    radius = (*iterBead)->GetRadius();
				const double         x = (*iterBead)->GetXPos();
				const double         y = (*iterBead)->GetYPos();
				const double         z = (*iterBead)->GetZPos();

				m_pFormat->SerializeBead(m_outStream, name, displayId, radius, x, y, z);

				if(!m_outStream.good())
					return IOError("Error writing CurrentState data to file");
			}
		}
	}

    return true;
#else
    return false;
#endif
}

// We add the beads from the PN processors to the snapshot file here.
// Unlike the P0 data, we don't have actual bead instances here only
// the bead's id, type and position coordinates. Note that we don't 
// check if the bead is visible as the calling routine is expected to
// have already done this, but we do impose the bead coordinate restrictions.
// If there are no restrictions on the range of bead coordinates to be
// displayed, we use a simple form of the loop; if any axis has a specified
// range then we use a longer form that checks each coordinate.

bool CCurrentState::SerializePN(long beadTotal, long idArray[], long beadTypeArray[], double RadiusArray[], double XArray[], double YArray[], double ZArray[])
{
#if EnableParallelMonitor == SimMPSEnabled

	if(m_bRestrictCoords)
	{
        for(long i = 0; i < beadTotal; i++)
        {
		    const long        type = beadTypeArray[i];
		    const zString     name = m_BeadNames.at(type);
		    const long   displayId = idArray[i];
		    const double         x = XArray[i];
		    const double         y = YArray[i];
		    const double         z = ZArray[i];

#if !defined(UseDPDBeadRadii)
		    const double    radius = 0.5;   // Hardwired unless user sets the compilation flag
#else
		    const double    radius = RadiusArray[i];
#endif
		    if(m_XMin < x && x < m_XMax &&
		       m_YMin < y && y < m_YMax &&
		       m_ZMin < z && z < m_ZMax)
		    {
                m_pFormat->SerializeBead(m_outStream, name, displayId, radius, x, y, z);

		        if(!m_outStream.good())
			        return IOError("Error writing CurrentState data to file");
		    }
        }
	}
	else
	{
        for(long i = 0; i < beadTotal; i++)
        {
		    const long        type = beadTypeArray[i];
		    const zString     name = m_BeadNames.at(type);
		    const long   displayId = idArray[i];
		    const double         x = XArray[i];
		    const double         y = YArray[i];
		    const double         z = ZArray[i];

#if !defined(UseDPDBeadRadii)
		    const double    radius = 0.5;   // Hardwired unless user sets the compilation flag
#else
		    const double    radius = RadiusArray[i];
#endif
			m_pFormat->SerializeBead(m_outStream, name, displayId, radius, x, y, z);

			if(!m_outStream.good())
				return IOError("Error writing CurrentState data to file");
        }
	}

    return true;
#else
    return false;
#endif
}

// Function to close the snapshot file in a parallel simulation.
// This function is only compiled in if the parallel monitor flag is set.

bool CCurrentState::SerializeEnd()
{
#if EnableParallelMonitor == SimMPSEnabled
	// Close the file so that we can view it before the simulation has ended

	m_outStream << zFlush;

	return Close();
#else
    return false;
#endif
}

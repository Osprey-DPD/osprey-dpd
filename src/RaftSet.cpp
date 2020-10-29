/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// RaftSet.cpp: implementation of the CRaftSet class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "RaftSet.h"

#include "SimMathFlags.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRaftSet::CRaftSet() : m_MeanRaftCellSize(0.0), 
					   m_MeanRaftPolymerSize(0.0),
					   m_MeanRaftArea(0.0),     
					   m_MeanRaftPerimeter(0.0),
					   m_SDevRaftCellSize(0.0),
					   m_SDevRaftPolymerSize(0.0),
					   m_SDevRaftArea(0.0),     
					   m_SDevRaftPerimeter(0.0)
{
	m_Rafts.clear();
}

// Destroy the CRafts objects if any have been created.

CRaftSet::~CRaftSet()
{
	if(!m_Rafts.empty())
	{
		for(RaftIterator iterRaft=m_Rafts.begin(); iterRaft!=m_Rafts.end(); iterRaft++)
		{
			delete *iterRaft;
		}
		m_Rafts.clear();
	}
}

// Function to delete all the contained CRaft objects.

void CRaftSet::DeleteAll()
{
	if(!m_Rafts.empty())
	{
		for(RaftIterator iterRaft=m_Rafts.begin(); iterRaft!=m_Rafts.end(); iterRaft++)
		{
			delete *iterRaft;
		}
		m_Rafts.clear();
	}
}

// Function to calculate the mean number of CNT cells per raft.

double CRaftSet::CalculateMeanRaftCellSize()
{
	m_MeanRaftCellSize = 0.0;
	m_SDevRaftCellSize = 0.0;

	if(m_Rafts.size() > 0)	// avoid divide by zero errors
	{ 
		for(cRaftIterator cIterRaft=m_Rafts.begin(); cIterRaft!=m_Rafts.end(); cIterRaft++)
		{
			const double total = (*cIterRaft)->GetCellTotal();
 			m_MeanRaftCellSize += total;
 			m_SDevRaftCellSize += total*total;
		}

		m_MeanRaftCellSize /= static_cast<double>(m_Rafts.size());
		m_SDevRaftCellSize /= static_cast<double>(m_Rafts.size());
 
		m_SDevRaftCellSize = sqrt(m_SDevRaftCellSize - m_MeanRaftCellSize*m_MeanRaftCellSize);
	}

 	return m_MeanRaftCellSize;
}

// Function to calculate the mean number of raft polymers (of all types) per raft.

double CRaftSet::CalculateMeanRaftPolymerSize()
{
	m_MeanRaftPolymerSize = 0.0;
	m_SDevRaftPolymerSize = 0.0;

	if(m_Rafts.size() > 0)	// avoid divide by zero errors
	{
		for(cRaftIterator cIterRaft=m_Rafts.begin(); cIterRaft!=m_Rafts.end(); cIterRaft++)
		{
			const double total = (*cIterRaft)->GetPolymerTotal();
			m_MeanRaftPolymerSize += total;
			m_SDevRaftPolymerSize += total*total;
		}

		m_MeanRaftPolymerSize /= static_cast<double>(m_Rafts.size());
		m_SDevRaftPolymerSize /= static_cast<double>(m_Rafts.size());
 
		m_SDevRaftPolymerSize = sqrt(m_SDevRaftPolymerSize - m_MeanRaftPolymerSize*m_MeanRaftPolymerSize);
	}

	return m_MeanRaftPolymerSize;
}

double CRaftSet::CalculateMeanRaftArea()
{
	m_MeanRaftArea = 0.0;
	m_SDevRaftArea = 0.0;

	if(m_Rafts.size() > 0)
	{
	}

	return m_MeanRaftArea;
}

double CRaftSet::CalculateMeanRaftPerimeter()
{
	m_MeanRaftPerimeter = 0.0;
	m_SDevRaftPerimeter = 0.0;

	if(m_Rafts.size() > 0)
	{
	}

	return m_MeanRaftPerimeter;
}

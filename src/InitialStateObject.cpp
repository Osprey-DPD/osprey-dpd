/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// InitialStateObject.cpp: implementation of the CInitialStateObject class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "InitialStateObject.h"
#include "InitialStateData.h"
#include "acfInitialStateFactory.h"

//////////////////////////////////////////////////////////////////////
// Global functions
//////////////////////////////////////////////////////////////////////

// Function to write out the initial state data to file. We pass the
// stream output operator to the contained CInitialStateData-derived object
// if the IsDataValid function shows that we have an object ready to
// hold the data.

zOutStream& operator<<(zOutStream& os, const CInitialStateObject& ISDOb)
{
	if(ISDOb.IsDataValid())
		os << *(ISDOb.m_pISD);

	return os;
}

// Function to read the initial state data from file. We pass the
// stream input operator to its contained CInitialStateData-derived object
// if the internal pointer shows that we have an object. We also check that
// the data is read correctly and set the local flag appropriately.
//
// The CInitialStateObject::m_bISDValid flag performs two functions: on creation,
// it shows whether a valid CAnalysisData-derived object has been created, that is,
// whether the type identifier was recognised, and after having read its data, 
// it shows whether the data is valid.

zInStream& operator>>(zInStream& is, CInitialStateObject& ISDOb)
{
	if(ISDOb.IsDataValid())
	{
		is >> *(ISDOb.m_pISD);

		ISDOb.SetDataValid(ISDOb.m_pISD->IsDataValid());
	}

	return is;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// The CInitialStateObject uses the "type" argument to determine which 
// CInitialStateData object to create. The CInitialStateData-derived classes 
// each have a static member variable that corresponds to the entry in the 
// control data file used to identify the initial state. This entry is accessed 
// using the static function GetType(). The acfInitialStateFactory object uses 
// this type identifier to call a function implemented by each 
// CInitialStateData-derived class to create an instance of the class.
//
// If the type is not recognised a flag is set showing that data cannot be read.

CInitialStateObject::CInitialStateObject(const zString type) : m_bISDValid(true)
{
	m_pISD = acfInitialStateFactory::Instance()->Create(type);

	if(!m_pISD)
	{
		m_bISDValid = false;
	}	
}

CInitialStateObject::~CInitialStateObject()
{
	// Only delete the contained CInitialStateData object if it exists

	if(m_pISD)
	{
		delete m_pISD;
		m_pISD = NULL;
	}
}

// Function to return a flag showing if the contained CInitialStateData object 
// is valid.

bool CInitialStateObject::IsDataValid() const
{
	return m_bISDValid;
}

// Function to set a flag showing whether the CInitialStateData object is valid.

void CInitialStateObject::SetDataValid(bool bValid)
{
	m_bISDValid = bValid;
}


// Function to return a pointer to the enclosed CInitialStateData object.
// Note that the object cannot be altered via the returned pointer.

const CInitialStateData* CInitialStateObject::GetISD() const
{
	return m_pISD;
}

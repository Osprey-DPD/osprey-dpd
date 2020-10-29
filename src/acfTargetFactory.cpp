/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// acfTargetFactory.cpp: implementation of the acfTargetFactory class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "acfTargetFactory.h"
//#include "CommandTargetNode.h"  do I need this header file?

// **********************************************************************
// Global Functions.
//////////////////////////////////////////////////////////////////////

// Static member variable holding a pointer to the single instance of
// the factory. This instance is created at startup.

acfTargetFactory* acfTargetFactory::m_pInstance = Instance();

acfTargetFactory* acfTargetFactory::Instance()
{
	if(!m_pInstance)
	{
		m_pInstance = new acfTargetFactory();
	}

	return m_pInstance;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Ensure the map is empty before using the factory.

acfTargetFactory::acfTargetFactory()
{
	m_mCallbacks.clear();
}

acfTargetFactory::~acfTargetFactory()
{

}

// Function that must be called by all classes that are to be creatable by this
// factory. The first argument is a unique string identifier used as a label 
// for the class of object to create. The second argument is a pointer to a callback
// function that returns an object of the class.
//
// The function returns true if registration is successful.

bool acfTargetFactory::Register(const zString id, CreateTargetCallback createFun)
{
	return m_mCallbacks.insert(StringTargetCallbackMap::value_type(id, createFun)).second;
}

// Function to remove a class from the factory's map.
//
// The function returns true if unregistration is successful.

bool acfTargetFactory::UnRegister(const zString id)
{
	return m_mCallbacks.erase(id) == 1;
}

// Function to create an object of the type specified by the
// identifying string passed in as the first argument. The second argument,
// "label" is a unique identifying label for the target.
//
// If the object's class has not been registered, we return a NULL pointer.

CCommandTargetNode* acfTargetFactory::Create(const zString id, const zString label)
{
	cStringTargetCallbackIterator citer = m_mCallbacks.find(id);

	if(citer != m_mCallbacks.end())
	{
		return (citer->second)(label);
	}
	else
		return 0;
}

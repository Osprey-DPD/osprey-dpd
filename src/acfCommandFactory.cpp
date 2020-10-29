/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// acfCommandFactory.cpp: implementation of the acfCommandFactory class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "acfCommandFactory.h"
#include "xxCommand.h"

// **********************************************************************
// Global Functions.
//////////////////////////////////////////////////////////////////////

// Static member variable holding a pointer to the single instance of
// the factory. This instance is created at startup.

acfCommandFactory* acfCommandFactory::m_pInstance = Instance();

acfCommandFactory* acfCommandFactory::Instance()
{
	if( !m_pInstance)
	{
		m_pInstance = new acfCommandFactory();
	}

	return m_pInstance;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Ensure the maps are empty before using the factory.

acfCommandFactory::acfCommandFactory()
{
	m_mCallbacks.clear();
    m_mArguments.clear();
}

acfCommandFactory::~acfCommandFactory()
{

}

// Function that must be called by all classes that are to be creatable by this
// factory. The first argument is a unique string identifier used as a label 
// for the class of object to create. The second argument is a pointer to a callback
// function that returns an object of the class.
//
// The function returns true if registration is successful.

bool acfCommandFactory::Register(const zString id, CreateCommandCallback createFun)
{
	return m_mCallbacks.insert(StringCommandMap::value_type(id, createFun)).second;
}

// Overloaded registration function for command classes that want to record
// the number of arguments they take so that it is accessible given just the
// class name.

bool acfCommandFactory::Register(const zString id, long argTotal, CreateCommandCallback createFun)
{
    m_mArguments.insert(zPairSL(id, argTotal));

	return m_mCallbacks.insert(StringCommandMap::value_type(id, createFun)).second;
}

// Function to remove a class from the factory's map. If the command class
// registered its argument total, we remove that entry as well.
//
// The function returns true if unregistration is successful.

bool acfCommandFactory::UnRegister(const zString id)
{
    if(m_mArguments.find(id) != m_mArguments.end())
    {
        m_mArguments.erase(id);
    }

	return m_mCallbacks.erase(id) == 1;
}

// Function to create an object of the type specified by the
// identifying string passed in as an argument.
//
// If the object's class has not been registered, we return a NULL pointer.

xxCommand* acfCommandFactory::Create(const zString id, long executionTime)
{
	cStringCommandIterator citer = m_mCallbacks.find(id);

	if(citer != m_mCallbacks.end())
	{
		return (citer->second)(executionTime);
	}
	else
		return 0;
}

// Function to return the number of arguments required by a command
// class if it has registered it. Otherwise it returns -1 to indicate
// that the command class did not register the information. The calling 
// routine must check that the returned value is acceptable.

long acfCommandFactory::GetArgumentTotal(const zString id)
{
	cStringLongIterator citer = m_mArguments.find(id);

	if(citer != m_mArguments.end())
	{
		return citer->second;
	}
	else
		return -1;
}

/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// LogctDynamicPolymerisePolymersInTarget.cpp: implementation of the CLogctDynamicPolymerisePolymersInTarget class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "LogctDynamicPolymerisePolymersInTarget.h"

//////////////////////////////////////////////////////////////////////
// Global function for serialization
//////////////////////////////////////////////////////////////////////

zOutStream& operator<<(zOutStream& os, const CLogctDynamicPolymerisePolymersInTarget& rMsg)
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	os << "<Body>" << zEndl;
	os << "<Name>DynamicPolymerisePolymersInTarget</Name>" << zEndl;
	os << "<Text>"  << zEndl;
	os << "Dynamic polymerisation decorator " << rMsg.m_DecLabel << " applied to command target " << rMsg.m_TargetLabel;
    os << " with parameters " << rMsg.m_PolyBondName << " " << rMsg.m_PolymerType;
        
    for(czLongVectorIterator citer=rMsg.m_vBeadTypes.begin(); citer!=rMsg.m_vBeadTypes.end(); ++citer)
    {
        os << " " << *citer;
    }
    
    os << " " << rMsg.m_MaxBonds<< "  " << rMsg.m_MaxDistance << " " << rMsg.m_sprConst << " " << rMsg.m_unStrLength
       << " " << rMsg.m_BondPrefactor << " " <<   rMsg.m_BondBarrier;
    os << "</Text>" << zEndl;
    os << "</Body>" << zEndl;

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	os << "Dynamic polymerisation decorator " << rMsg.m_DecLabel << " applied to command target " << rMsg.m_TargetLabel;
    os << " with parameters " << rMsg.m_PolyBondName << " " << rMsg.m_PolymerType << " " << rMsg.m_vBeadTypes.size();
    
    for(czLongVectorIterator citer=rMsg.m_vBeadTypes.begin(); citer!=rMsg.m_vBeadTypes.end(); ++citer)
    {
        os << " " << *citer;
    }
    
    os << " " << rMsg.m_MaxBonds<< "  " << rMsg.m_MaxDistance << " " << rMsg.m_sprConst << " " << rMsg.m_unStrLength
       << " " << rMsg.m_BondPrefactor << " " <<   rMsg.m_BondBarrier;

#endif

	return os;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogctDynamicPolymerisePolymersInTarget::CLogctDynamicPolymerisePolymersInTarget(long time,
                                    const zString targetlabel, const zString decLabel,
                                    const zString polyBondName, long polyType, zLongVector vBeadTypes,
                                    long maxBonds, double maxDist, double sprConst, double unStrLength,
                                    double bondPrefactor, double bondBarrier) :
                                    CLogConstraintMessage(time), m_TargetLabel(targetlabel),
                                    m_DecLabel(decLabel),
                                    m_PolyBondName(polyBondName),
                                    m_PolymerType(polyType), m_vBeadTypes(vBeadTypes),
                                    m_MaxBonds(maxBonds), m_MaxDistance(maxDist),
                                    m_sprConst(sprConst), m_unStrLength(unStrLength),
                                    m_BondPrefactor(bondPrefactor), m_BondBarrier(bondBarrier)
{
}

CLogctDynamicPolymerisePolymersInTarget::~CLogctDynamicPolymerisePolymersInTarget()
{

}

// Pure virtual function to allow the xxMessage-derived object to 
// write its data to file when invoked through an xxMessage pointer. 

void CLogctDynamicPolymerisePolymersInTarget::Serialize(zOutStream& os) const
{
	CLogConstraintMessage::Serialize(os);

	os << (*this);
}


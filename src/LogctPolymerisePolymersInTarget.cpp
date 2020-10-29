/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// LogctPolymerisePolymersInTarget.cpp: implementation of the CLogctPolymerisePolymersInTarget class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "LogctPolymerisePolymersInTarget.h"

//////////////////////////////////////////////////////////////////////
// Global function for serialization
//////////////////////////////////////////////////////////////////////

zOutStream& operator<<(zOutStream& os, const CLogctPolymerisePolymersInTarget& rMsg)
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	os << "<Body>" << zEndl;
	os << "<Name>PolymerisePolymersInTarget</Name>" << zEndl;
	os << "    <TargetName>"		<< rMsg.m_CommandTarget		<< "</TargetName>" << zEndl;
	os << "    <BondName>"		    << rMsg.m_BondName          << "</BondName>" << zEndl;
	os << "    <BondType>"		    << rMsg.m_BondType          << "</BondType>" << zEndl;
	os << "    <PolymerTotal>"		<< rMsg.m_PolymerTotal		<< "</PolymerTotal>" << zEndl;
	os << "    <BondTotal>"			<< rMsg.m_BondTotal			<< "</BondTotal>" << zEndl;
	os << "    <MaxBonds>"			<< rMsg.m_MaxBonds		    << "</MaxBonds>" << zEndl;
	os << "    <Range>"			    << rMsg.m_Range 		    << "</Range>"    << zEndl;
	os << "    <Fraction>"		    << rMsg.m_Fraction			<< "</Fraction>" << zEndl;
	os << "    <SpringConstant>"    << rMsg.m_SpringConstant	<< "</SpringConstant>" << zEndl;
	os << "    <UnStretchedLength>" << rMsg.m_UnstretchedLength	<< "</UnStretchedLength>" << zEndl;
	os << "</Body>" << zEndl;

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	os << "Command target "       << rMsg.m_CommandTarget
	   << " containing "          << rMsg.m_PolymerTotal   << " polymers "
	   << " polymerised using "   << rMsg.m_BondTotal      << " Hookean bonds with name " << rMsg.m_BondName
       << " (type " << rMsg.m_BondType << ") with max bonds/polymer " << rMsg.m_MaxBonds
	   << " max sepn of "         << rMsg.m_Range          << " and fraction " << rMsg.m_Fraction
	   << " and bond parameters " << rMsg.m_SpringConstant << " " << rMsg.m_UnstretchedLength;

#endif

	return os;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogctPolymerisePolymersInTarget::CLogctPolymerisePolymersInTarget(long time, const zString cmdTarget, 
									long polymerTotal, long bondTotal, long maxBonds, double range, double fraction, 
									double sprConst, double unStrLength) : CLogConstraintMessage(time), 
									m_CommandTarget(cmdTarget), 
                                    m_BondName("Unnamed"), m_BondType(-1),
									m_PolymerTotal(polymerTotal), m_BondTotal(bondTotal),
									m_MaxBonds(maxBonds), m_Range(range), m_Fraction(fraction), 
									m_SpringConstant(sprConst), m_UnstretchedLength(unStrLength)
{

}

// Overloaded constructor that writes out the bond's name and numeric type.

CLogctPolymerisePolymersInTarget::CLogctPolymerisePolymersInTarget(long time, const zString cmdTarget, 
                                    const zString bondName, long bondType,
									long polymerTotal, long bondTotal, long maxBonds, double range, double fraction, 
									double sprConst, double unStrLength) : CLogConstraintMessage(time), 
									m_CommandTarget(cmdTarget), 
									m_BondName(bondName), m_BondType(bondType),
									m_PolymerTotal(polymerTotal), m_BondTotal(bondTotal),
									m_MaxBonds(maxBonds), m_Range(range), m_Fraction(fraction), 
									m_SpringConstant(sprConst), m_UnstretchedLength(unStrLength)
{

}





CLogctPolymerisePolymersInTarget::~CLogctPolymerisePolymersInTarget()
{

}

// Pure virtual function to allow the xxMessage-derived object to 
// write its data to file when invoked through an xxMessage pointer. 

void CLogctPolymerisePolymersInTarget::Serialize(zOutStream& os) const
{
	CLogConstraintMessage::Serialize(os);

	os << (*this);
}


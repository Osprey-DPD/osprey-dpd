/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// aaRegionToType.cpp: implementation of the aaRegionToType class.
//
// This file contains the member functions for the function objects
// that are defined in the header file.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "aaRegionToType.h"

aaDouble::aaDouble()
{
}

aaDouble::~aaDouble()
{
}


aaGetAggregateId::aaGetAggregateId(long id) : m_id(id)
{
}

aaGetAggregateId::~aaGetAggregateId()
{
}

aaGetCommandId::aaGetCommandId(long id) : m_id(id)
{
}

aaGetCommandId::~aaGetCommandId()
{
}

aaGetEventId::aaGetEventId(long id) : m_id(id)
{
}

aaGetEventId::~aaGetEventId()
{
}

aaGetProcessId::aaGetProcessId(long id) : m_id(id)
{
}

aaGetProcessId::~aaGetProcessId()
{
}

aaGetAggregateName::aaGetAggregateName(const zString name) : m_Name(name)
{
}

aaGetAggregateName::~aaGetAggregateName()
{
}

aaGetCommandName::aaGetCommandName(const zString name) : m_Name(name)
{
}

aaGetCommandName::~aaGetCommandName()
{
}

aaGetCommandExecutionTime::aaGetCommandExecutionTime(long time) : m_ExecutionTime(time)
{
}

aaGetCommandExecutionTime::~aaGetCommandExecutionTime()
{
}

aaCommandExecutionTimeLess::aaCommandExecutionTimeLess(long time) : m_ExecutionTime(time)
{
}

aaCommandExecutionTimeLess::~aaCommandExecutionTimeLess()
{
}

aaCommandExecutionTimeGreater::aaCommandExecutionTimeGreater(long time) : m_ExecutionTime(time)
{
}

aaCommandExecutionTimeGreater::~aaCommandExecutionTimeGreater()
{
}

aaGetEventName::aaGetEventName(const zString name) : m_Name(name)
{
}

aaGetEventName::~aaGetEventName()
{
}

aaGetProcessName::aaGetProcessName(const zString name) : m_Name(name)
{
}

aaGetProcessName::~aaGetProcessName()
{
}

aaBeadId::aaBeadId(long beadId) : m_Id(beadId)
{
}


aaBeadType::aaBeadType(long type) : m_Type(type)
{
}

aaBeadType::~aaBeadType()
{
}

aaBeadType2::aaBeadType2(long type1, long type2) : m_Type1(type1), m_Type2(type2)
{
}

aaBeadType2::~aaBeadType2()
{
}

aaBeadTypeLess::aaBeadTypeLess(long type) : m_Type(type)
{
}

aaBeadTypeLess::~aaBeadTypeLess()
{
}

aaBeadTypeGreater::aaBeadTypeGreater(long type) : m_Type(type)
{
}

aaBeadTypeGreater::~aaBeadTypeGreater()
{
}

aaPolymerType::aaPolymerType(long type) : m_Type(type)
{
}

aaPolymerType::~aaPolymerType()
{
}

aaPolymerTypeLess::aaPolymerTypeLess(long type) : m_Type(type)
{
}

aaPolymerTypeLess::~aaPolymerTypeLess()
{
}

aaPolymerTypeGreater::aaPolymerTypeGreater(long type) : m_Type(type)
{
}

aaPolymerTypeGreater::~aaPolymerTypeGreater()
{
}

aaCNTCellNumber::aaCNTCellNumber(long beadType) : m_BeadType(beadType)
{
}

aaCNTCellNumber::~aaCNTCellNumber()
{
}


aaCellNumber::aaCellNumber(long beadType) : m_BeadType(beadType)
{
}

aaCellNumber::~aaCellNumber()
{
}

aaCellXCM::aaCellXCM(long beadType) : m_BeadType(beadType)
{
}

aaCellXCM::~aaCellXCM()
{
}

aaCellYCM::aaCellYCM(long beadType) : m_BeadType(beadType)
{
}

aaCellYCM::~aaCellYCM()
{
}

aaCellZCM::aaCellZCM(long beadType) : m_BeadType(beadType)
{
}

aaCellZCM::~aaCellZCM()
{
}

aaCellCM::aaCellCM(long beadType) : aaRegionToType<CCell,aaVector>(beadType)
{
}

aaCellCM::~aaCellCM()
{
}

aaDensity::aaDensity(long beadType) : aaRegionToType<CSlice,aaScalar>(beadType)
{
}

aaDensity::~aaDensity()
{
}

aaMomentum::aaMomentum(long beadType) : aaRegionToType<CSlice,aaVector>(beadType)
{
}

aaMomentum::~aaMomentum()
{
}

aaBeadStressXProfile::aaBeadStressXProfile(long index) : m_Index(index)
{
}

aaBeadStressXProfile::~aaBeadStressXProfile()
{
}

aaBeadStressYProfile::aaBeadStressYProfile(long index) : m_Index(index)
{
}

aaBeadStressYProfile::~aaBeadStressYProfile()
{
}

aaBeadStressZProfile::aaBeadStressZProfile(long index) : m_Index(index)
{
}

aaBeadStressZProfile::~aaBeadStressZProfile()
{
}

aaBondStressXProfile::aaBondStressXProfile(long index) : m_Index(index)
{
}

aaBondStressXProfile::~aaBondStressXProfile()
{
}

aaBondStressYProfile::aaBondStressYProfile(long index) : m_Index(index)
{
}

aaBondStressYProfile::~aaBondStressYProfile()
{
}

aaBondStressZProfile::aaBondStressZProfile(long index) : m_Index(index)
{
}

aaBondStressZProfile::~aaBondStressZProfile()
{
}

aaBondPairStressXProfile::aaBondPairStressXProfile(long index) : m_Index(index)
{
}

aaBondPairStressXProfile::~aaBondPairStressXProfile()
{
}

aaBondPairStressYProfile::aaBondPairStressYProfile(long index) : m_Index(index)
{
}

aaBondPairStressYProfile::~aaBondPairStressYProfile()
{
}

aaBondPairStressZProfile::aaBondPairStressZProfile(long index) : m_Index(index)
{
}

aaBondPairStressZProfile::~aaBondPairStressZProfile()
{
}

aaStressZProfile::aaStressZProfile()
{
}

aaStressZProfile::~aaStressZProfile()
{
}

aaStressComponentProfile::aaStressComponentProfile(long index) : m_Index(index)
{
}

aaStressComponentProfile::~aaStressComponentProfile()
{
}

aaBeadXPos::aaBeadXPos()
{
}

aaBeadXPos::~aaBeadXPos()
{
}

aaBeadYPos::aaBeadYPos()
{
}

aaBeadYPos::~aaBeadYPos()
{
}

aaBeadZPos::aaBeadZPos()
{
}

aaBeadZPos::~aaBeadZPos()
{
}

aaBeadXMom::aaBeadXMom()
{
}

aaBeadXMom::~aaBeadXMom()
{
}

aaBeadYMom::aaBeadYMom()
{
}

aaBeadYMom::~aaBeadYMom()
{
}

aaBeadZMom::aaBeadZMom()
{
}

aaBeadZMom::~aaBeadZMom()
{
}

aaBeadXForce::aaBeadXForce()
{
}

aaBeadXForce::~aaBeadXForce()
{
}

aaBeadYForce::aaBeadYForce()
{
}

aaBeadYForce::~aaBeadYForce()
{
}

aaBeadZForce::aaBeadZForce()
{
}

aaBeadZForce::~aaBeadZForce()
{
}

#if EnableShadowSimBox == SimACNEnabled
aaActiveBondXPos::aaActiveBondXPos()
{
}

aaActiveBondXPos::~aaActiveBondXPos()
{
}

aaActiveBondYPos::aaActiveBondYPos()
{
}

aaActiveBondYPos::~aaActiveBondYPos()
{
}

aaActiveBondZPos::aaActiveBondZPos()
{
}

aaActiveBondZPos::~aaActiveBondZPos()
{
}
#endif

aaBeadXPosLess::aaBeadXPosLess(double limit) : m_Limit(limit)
{
}

aaBeadXPosLess::~aaBeadXPosLess()
{
}

aaBeadYPosLess::aaBeadYPosLess(double limit) : m_Limit(limit)
{
}

aaBeadYPosLess::~aaBeadYPosLess()
{
}

aaBeadZPosLess::aaBeadZPosLess(double limit) : m_Limit(limit)
{
}

aaBeadZPosLess::~aaBeadZPosLess()
{
}

aaBeadXPosGreater::aaBeadXPosGreater(double limit) : m_Limit(limit)
{
}

aaBeadXPosGreater::~aaBeadXPosGreater()
{
}

aaBeadYPosGreater::aaBeadYPosGreater(double limit) : m_Limit(limit)
{
}

aaBeadYPosGreater::~aaBeadYPosGreater()
{
}

aaBeadZPosGreater::aaBeadZPosGreater(double limit) : m_Limit(limit)
{
}

aaBeadZPosGreater::~aaBeadZPosGreater()
{
}

aaBeadXPosGreaterEqual::aaBeadXPosGreaterEqual(double limit) : m_Limit(limit)
{
}

aaBeadXPosGreaterEqual::~aaBeadXPosGreaterEqual()
{
}

aaBeadYPosGreaterEqual::aaBeadYPosGreaterEqual(double limit) : m_Limit(limit)
{
}

aaBeadYPosGreaterEqual::~aaBeadYPosGreaterEqual()
{
}

aaBeadZPosGreaterEqual::aaBeadZPosGreaterEqual(double limit) : m_Limit(limit)
{
}

aaBeadZPosGreaterEqual::~aaBeadZPosGreaterEqual()
{
}

// Note that we allow a negative value for the translation distance.
aaTranslateCoordinate::aaTranslateCoordinate(double l) : m_L(l)
{
}

aaTranslateCoordinate::~aaTranslateCoordinate()
{
}



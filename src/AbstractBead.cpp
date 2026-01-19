/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// AbstractBead.cpp: implementation of the CAbstractBead class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "AbstractBead.h"
#include "Polymer.h"
#include "xxParallelBase.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Default constructor

CAbstractBead::CAbstractBead() : m_id(0), m_Type(0), m_bIsVisible(true), m_bIsMovable(true), m_bIsFrozen(false), m_ForceCounter(0), 
#if SimIdentifier == BD
                                 m_Radius(0.0),
                                 m_TransDiff(0.0),
                                 m_RotDiff(0.0)
#else
                                 m_Radius(0.0)
#endif
{
#if EnableDPDLG == ExperimentEnabled
    m_LGRadius  = 0.0;
    m_LGDensity = 0.0;
#endif

#if EnableParallelSimBox == SimMPSEnabled
    m_pPolymer = 0;
#endif

	for(short int i=0; i<3; i++)
	{
		m_Pos[i]		= 0.0;
		m_Mom[i]		= 0.0;
		m_Force[i]		= 0.0;
		m_AngMom[i]		= 0.0;
		m_oldPos[i]		= 0.0;
		m_oldMom[i]		= 0.0;
		m_oldForce[i]	= 0.0;
		m_unPBCPos[i]	= 0.0;
		m_InitialPos[i]	= 0.0;
		m_dPos[i]		= 0.0;
		m_Stress[3*i]	= 0.0;
		m_Stress[3*i+1]	= 0.0;
		m_Stress[3*i+2]	= 0.0;
	}
}

// Constructor used in CInitialData to set up a template for each type of
// bead in the simulation. These template beads are used for creating the
// actual beads later. The radius of the beads is set to 0.5 corresponding
// to the default values used in simulating DPD beads and for drawing
// MD beads. This radius is not used during the MD simulation itself.
//
// Note that id = -1 that this is a template bead not used in the simulation.

CAbstractBead::CAbstractBead(long type) : m_id(-1), m_Type(type), 
										  m_bIsVisible(true), 
										  m_bIsMovable(true), 
										  m_bIsFrozen(false), 
										  m_ForceCounter(0),
#if SimIdentifier == BD
                                 m_Radius(0.0),
                                 m_TransDiff(0.0),
                                 m_RotDiff(0.0)
#else
                                 m_Radius(0.0)
#endif
{
#if EnableDPDLG == ExperimentEnabled
    m_LGRadius  = 0.0;
    m_LGDensity = 0.0;
#endif

#if EnableParallelSimBox == SimMPSEnabled
    m_pPolymer = 0;
#endif

    for(short int i=0; i<3; i++)
	{
		m_Pos[i]		= 0.0;
		m_Mom[i]		= 0.0;
		m_Force[i]		= 0.0;
		m_AngMom[i]		= 0.0;
		m_oldPos[i]		= 0.0;
		m_oldMom[i]		= 0.0;
		m_oldForce[i]	= 0.0;
		m_unPBCPos[i]	= 0.0;
		m_InitialPos[i]	= 0.0;
		m_dPos[i]		= 0.0;
		m_Stress[3*i]	= 0.0;
		m_Stress[3*i+1]	= 0.0;
		m_Stress[3*i+2]	= 0.0;
	}
}

// Constructor that sets the bead interaction distance to unity for all 
// bead types. This is provided so that the original code will still be 
// available. Note that given the interaction distance is 1 the bead radius 
// must be 0.5.

CAbstractBead::CAbstractBead(long id, long type, bool movable, 
							 double x0[3], double v0[3] ) : m_id(id), m_Type(type), 
															m_bIsVisible(true),
															m_bIsMovable(movable),
															m_bIsFrozen(false),
															m_ForceCounter(0),
#if SimIdentifier == BD
                                                            m_Radius(0.5),
                                                            m_TransDiff(0.0),
                                                            m_RotDiff(0.0)
#else
                                                            m_Radius(0.5)
#endif
{
#if EnableDPDLG == ExperimentEnabled
    m_LGRadius  = 0.0;
    m_LGDensity = 0.0;
#endif

#if EnableParallelSimBox == SimMPSEnabled
    m_pPolymer = 0;
#endif

	for(short int i=0; i<3; i++)
	{
		m_Pos[i]		= x0[i];	// Note non-zero initial values!
		m_Mom[i]		= v0[i];
		m_Force[i]		= 0.0;
		m_AngMom[i]		= 0.0;
		m_oldPos[i]		= 0.0;
		m_oldMom[i]		= 0.0;
		m_oldForce[i]	= 0.0;
		m_unPBCPos[i]	= 0.0;
		m_InitialPos[i]	= 0.0;
		m_dPos[i]		= 0.0;
		m_Stress[3*i]	= 0.0;
		m_Stress[3*i+1]	= 0.0;
		m_Stress[3*i+2]	= 0.0;
	}
}

// Constructor that sets the bead interaction distance according to
// the bead's type.

CAbstractBead::CAbstractBead(long id, long type, bool movable, double radius,
							 double x0[3], double v0[3] ) : m_id(id), m_Type(type), 
															m_bIsVisible(true),
															m_bIsMovable(movable),
															m_bIsFrozen(false),
															m_ForceCounter(0),
#if SimIdentifier == BD
                                                            m_Radius(radius),
                                                            m_TransDiff(0.0),
                                                            m_RotDiff(0.0)
#else
                                                            m_Radius(radius)
#endif
{
#if EnableDPDLG == ExperimentEnabled
    m_LGRadius  = 0.0;
    m_LGDensity = 0.0;
#endif

#if EnableParallelSimBox == SimMPSEnabled
    m_pPolymer = 0;
#endif

    for(short int i=0; i<3; i++)
	{
		m_Pos[i]		= x0[i];	// Note non-zero initial values!
		m_Mom[i]		= v0[i];
		m_Force[i]		= 0.0;
		m_AngMom[i]		= 0.0;
		m_oldPos[i]		= 0.0;
		m_oldMom[i]		= 0.0;
		m_oldForce[i]	= 0.0;
		m_unPBCPos[i]	= 0.0;
		m_InitialPos[i]	= 0.0;
		m_dPos[i]		= 0.0;
		m_Stress[3*i]	= 0.0;
		m_Stress[3*i+1]	= 0.0;
		m_Stress[3*i+2]	= 0.0;
	}
}

// Constructor that sets the force parameter for a liquid-gas DPD simulation
// using a density-dependent potential.

#if EnableDPDLG == ExperimentEnabled
CAbstractBead::CAbstractBead(long id, long type, bool movable, double radius, double lgRadius,
							 double x0[3], double v0[3] ) : m_id(id), m_Type(type), 
															m_bIsVisible(true),
															m_bIsMovable(movable),
															m_bIsFrozen(false),
															m_ForceCounter(0),
                                                            m_Radius(radius),
                                                            m_LGRadius(lgRadius),
                                                            m_LGDensity(0.0)
{
#if EnableParallelSimBox == SimMPSEnabled
    m_pPolymer = 0;
#endif

	for(short int i=0; i<3; i++)
	{
		m_Pos[i]		= x0[i];	// Note non-zero initial values!
		m_Mom[i]		= v0[i];
		m_Force[i]		= 0.0;
		m_AngMom[i]		= 0.0;
		m_oldPos[i]		= 0.0;
		m_oldMom[i]		= 0.0;
		m_oldForce[i]	= 0.0;
		m_unPBCPos[i]	= 0.0;
		m_InitialPos[i]	= 0.0;
		m_dPos[i]		= 0.0;
		m_Stress[3*i]	= 0.0;
		m_Stress[3*i+1]	= 0.0;
		m_Stress[3*i+2]	= 0.0;
	}
}
#endif

// Constructor for use in the parallel code when beads need access to their 
// parent polymer.

#if EnableParallelSimBox == SimMPSEnabled
CAbstractBead::CAbstractBead(CPolymer* const pPolymer, long id, long type, bool movable, double radius,
                             double x0[3], double v0[3] ) : m_id(id), m_Type(type), 
															m_bIsVisible(true),
															m_bIsMovable(movable),
															m_bIsFrozen(false),
															m_ForceCounter(0),
                                                            m_Radius(radius),
                                                            m_pPolymer(pPolymer)
{
	for(short int i=0; i<3; i++)
	{
		m_Pos[i]		= x0[i];	// Note non-zero initial values!
		m_Mom[i]		= v0[i];
		m_Force[i]		= 0.0;
		m_AngMom[i]		= 0.0;
		m_oldPos[i]		= 0.0;
		m_oldMom[i]		= 0.0;
		m_oldForce[i]	= 0.0;
		m_unPBCPos[i]	= 0.0;
		m_InitialPos[i]	= 0.0;
		m_dPos[i]		= 0.0;
		m_Stress[3*i]	= 0.0;
		m_Stress[3*i+1]	= 0.0;
		m_Stress[3*i+2]	= 0.0;
	}
}
#endif

// Copy constructor for the CAbstractBead class.

CAbstractBead::CAbstractBead(const CAbstractBead& oldBead) : m_id(oldBead.m_id),
															 m_Type(oldBead.m_Type),
															 m_bIsVisible(oldBead.m_bIsVisible),
															 m_bIsMovable(oldBead.m_bIsMovable),
															 m_bIsFrozen(oldBead.m_bIsFrozen),
															 m_ForceCounter(oldBead.m_ForceCounter),
#if SimIdentifier == BD
															 m_Radius(oldBead.m_Radius),
															 m_TransDiff(oldBead.m_TransDiff),
															 m_RotDiff(oldBead.m_RotDiff)
#else
#if EnableDPDLG == ExperimentDisabled
															 m_Radius(oldBead.m_Radius)
#elif EnableDPDLG == ExperimentEnabled
															 m_Radius(oldBead.m_Radius),
															 m_LGRadius(oldBead.m_LGRadius),
															 m_LGDensity(oldBead.m_LGDensity)
#endif
#endif
#if EnableParallelSimBox == SimMPSEnabled
                                                             , m_pPolymer(oldBead.m_pPolymer)
#endif                                                          

{
	for(short int i=0; i<3; i++)
	{
		m_Pos[i]		= oldBead.m_Pos[i];
		m_Mom[i]		= oldBead.m_Mom[i];
		m_Force[i]		= oldBead.m_Force[i];
		m_AngMom[i]		= oldBead.m_AngMom[i];

		m_oldPos[i]		= oldBead.m_oldPos[i];
		m_oldMom[i]		= oldBead.m_oldMom[i];
		m_oldForce[i]	= oldBead.m_oldForce[i];

		m_InitialPos[i]	= oldBead.m_InitialPos[i];
		m_unPBCPos[i]	= oldBead.m_unPBCPos[i];
		m_dPos[i]		= oldBead.m_dPos[i];
		m_Stress[3*i]	= oldBead.m_Stress[3*i];
		m_Stress[3*i+1]	= oldBead.m_Stress[3*i+1];
		m_Stress[3*i+2]	= oldBead.m_Stress[3*i+2];
	}
}

CAbstractBead::~CAbstractBead()
{
}

// BIOENG 455 Function to add the total Morse force on this bead.

void CAbstractBead::AddHelixForces(const double fx, const double fy, const double fz)
{
    m_Force[0] += fx;
    m_Force[1] += fy;
    m_Force[2] += fz;
}

// Function used in the parallel code to set the bead's owning polymer. This allows
// beads to navigate upwards via their polymers. We only compile this in for a
// parallel executable as the member variable is not defined in the serial code.
// Note that we make no check that the passed-in pointer is valid: the calling
// routine must do this.

#if EnableParallelSimBox == SimMPSEnabled
void CAbstractBead::SetOwningPolymer(CPolymer* pPolymer)
{
	m_pPolymer = pPolymer;
}
#endif

// Function to return the kinetic energy of the bead. Note that the mass is not
// explicitly included here, so we are really returning p**2/2*m.

double CAbstractBead::GetKE() const
{
    return 0.5*(m_Mom[0]*m_Mom[0] + m_Mom[1]*m_Mom[1] + m_Mom[2]*m_Mom[2]);
}

// Functions to return the bead coordinates shifted into the whole simulation Space.
// We take the P0-based coordinates and shift them according to the owning
// processor's rank and the assignment of processor's in Space.

double CAbstractBead::GetShiftedXPos() const
{
#if EnableParallelSimBox == SimMPSEnabled
    return (m_Pos[0] + xxParallelBase::GlobalGetSimBoxXOrigin());
#else
    return m_Pos[0];
#endif
}

double CAbstractBead::GetShiftedYPos() const
{
#if EnableParallelSimBox == SimMPSEnabled
    return (m_Pos[1] + xxParallelBase::GlobalGetSimBoxYOrigin());
#else
    return m_Pos[1];
#endif
}

double CAbstractBead::GetShiftedZPos() const
{
#if EnableParallelSimBox == SimMPSEnabled
    return (m_Pos[2] + xxParallelBase::GlobalGetSimBoxZOrigin());
#else
    return m_Pos[2];
#endif
}



/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// RestartState.cpp: implementation of the CRestartState class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "RestartState.h"
#include "InitialState.h"
#include "Polymer.h"
#include "Bead.h"

#include "LogRestartStateBuilderError.h"  
#include "LogRestartStateBuilderWarning.h" 


// **********************************************************************
// Global Functions.
//

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// Constructor to write out a restart state to file. We pass the IOFlag
// to xxFile hardwired to true.
// Note tha hardwired parameter m_CoordErrorLimit. This is used when writing
// a restart state to ensure that bead coordinates that are very close to a
// boundary of the SimBox are translated to the origin to avoid them being
// written to the file as equal to the SimBox size. A previous case wrote
// a value of 16 when the coordinate was 16 - 0.000011734. Hence, we have 
// to use an epsilon large enough to catch this.

CRestartState::CRestartState(long currentTime, zString runId, 
                             const CInitialState& riState) : xxState(xxBase::GetRSPrefix() +  runId + ".con." + ToString(currentTime) + ".dat" , true, currentTime, runId),
							 m_riState(riState),
                             m_bLogWarningMessages(false),
							 m_CoordErrorLimit(0.001),
							 m_SimBoxXLength(riState.GetSimBoxXLength()),
							 m_SimBoxYLength(riState.GetSimBoxYLength()),
							 m_SimBoxZLength(riState.GetSimBoxZLength()),
                             m_SimBoxXMinusEpsilon(m_SimBoxXLength - m_CoordErrorLimit),
                             m_SimBoxYMinusEpsilon(m_SimBoxYLength - m_CoordErrorLimit),
                             m_SimBoxZMinusEpsilon(m_SimBoxZLength - m_CoordErrorLimit),
                             m_bUnexpectedBeadTypeFound(false),
                             m_ExpectedLargestBeadType(riState.GetBeadTypeTotal()-1),
                             m_LargestBeadTypeFound(0)
{
}

// Constructor to allow a restart state to be read or written using the IOFlag:
//
// IOFlag = true	means write data out to file
//          false	means read data from file.

CRestartState::CRestartState(long currentTime, zString runId, bool IOFlag, bool bLogWarnings, 
                             const CInitialState& riState) : xxState(xxBase::GetRSPrefix() + runId + ".con." + ToString(currentTime) + ".dat", IOFlag, currentTime, runId),
							 m_riState(riState),
                             m_bLogWarningMessages(bLogWarnings),
							 m_CoordErrorLimit(0.001),
							 m_SimBoxXLength(riState.GetSimBoxXLength()),
							 m_SimBoxYLength(riState.GetSimBoxYLength()),
							 m_SimBoxZLength(riState.GetSimBoxZLength()),
                             m_SimBoxXMinusEpsilon(m_SimBoxXLength - m_CoordErrorLimit),
                             m_SimBoxYMinusEpsilon(m_SimBoxYLength - m_CoordErrorLimit),
                             m_SimBoxZMinusEpsilon(m_SimBoxZLength - m_CoordErrorLimit),
                             m_bUnexpectedBeadTypeFound(false),
                             m_ExpectedLargestBeadType(riState.GetBeadTypeTotal()-1),
                             m_LargestBeadTypeFound(0)

{
}

CRestartState::~CRestartState()
{

}

// Function to return the state of warning message logging.

bool CRestartState::AreWarningMessagesLogged() const
{
	return m_bLogWarningMessages;
}

// Function to read/write the bead coordinate data and other information from a 
// previously-saved CRestartState. 

bool CRestartState::Serialize()
{
	// Write data to a restart file

	if(m_IOFlag)	
	{
		// First write out the constraints on the SimBox. These are boolean flags
		// output as 0 or 1. Note that if a constraint exists then it is written to
		// the restart file as if it is active. The user is responsible for setting
		// a command to turn the constraint off at t = 1 in the restarted run.
		// This is to avoid having to work out if the constraint was active at the
		// time the restart state was saved.

		m_outStream << m_riState.IsWallPresent()    << " ";
		m_outStream << m_riState.IsGravityPresent() << " ";
		m_outStream << m_riState.IsShearPresent()   << zEndl;

		// Next write out the bulk polymers' bead coordinates.

		// To avoid beads that are very close to the SimBox boundaries having
		// their coordinates read in as if they were on the boundary we check
		// all beads and translate them to the origin if one of their coordinates
		// is within a certain epsilon of a boundary. We don't have to do this
		// for wall head beads as their coordinates do not change during a simulation
		// and they are assigned safe values in CBuilder::AssignWallBeadCoords().
		// Note that we cannot move the unPBC coordinates because beads joined
		// into polymers use these to calculate the bond lengths and forces and
		// this calculation is screwed up unless all beads in a polymer have
		// unPBC coordinates that are in the same box image.

//		m_outStream.precision(8);

		double xp[3];

		for(cPolymerVectorIterator iterPoly=m_riState.GetPolymers().begin(); iterPoly!=m_riState.GetPolymers().end(); iterPoly++ )
		{
			for(cBeadVectorIterator iterBead=(*iterPoly)->GetBeads().begin(); iterBead!=(*iterPoly)->GetBeads().end(); iterBead++)
			{
				xp[0] = (*iterBead)->GetXPos();
				xp[1] = (*iterBead)->GetYPos();
				xp[2] = (*iterBead)->GetZPos();

				// Check for beads with coordinates outside the SimBox, and log an
                // error message and stop the run if any are found. Also check for 
                // beads that have coordinates that are closer than m_CoordErrorLimit 
                // to the boundaries and log a warning message and shift them to the
                // them to the origin. The run continues in this case.

                if(CheckBeadWithinBox((*iterBead)->GetId(), (*iterBead)->GetType(), xp[0], xp[1], xp[2]))
                {
				    m_outStream << (*iterPoly)->GetId()			<< " " << (*iterPoly)->GetType()		<< " ";
				    m_outStream << (*iterBead)->GetId()			<< " " << (*iterBead)->GetType()		<< " ";
				    m_outStream << (*iterBead)->GetRadius()		<< " ";
				    m_outStream << xp[0]						<< " " << xp[1]							<< " " << xp[2]   << " ";
				    m_outStream << (*iterBead)->GetunPBCXPos()  << " " << (*iterBead)->GetunPBCYPos()   << " " << (*iterBead)->GetunPBCZPos()   << " ";
				    m_outStream << (*iterBead)->GetXMom()		<< " " << (*iterBead)->GetYMom()		<< " " << (*iterBead)->GetZMom()		<< " ";
				    m_outStream << (*iterBead)->GetXForce()		<< " " << (*iterBead)->GetYForce()		<< " " << (*iterBead)->GetZForce()		<< zEndl;
                }
                else
                {
		            return ErrorTrace("Bead coordinate error writing restart state");
                }
			}
		}

//		m_outStream.precision(6);

		// Now the wall polymer bead coordinates. We write the head bead data first and
		// then the remainder of the polymer's beads. This is because the head bead of a 
		// wall polymer is not kept in its bead vector, but we need to write it in a 
		// well-defined order so that we can read it in later. It does not matter what
		// order we use as long it is written and read consistently.
		//
		// We write the data this way rather than use the vAllBeads, vAllBonds vectors 
		// because we want to identify beads and bonds from their parent polymers,  

		if(m_riState.IsWallPresent())
		{
			for(cPolymerVectorIterator iterWallPoly=m_riState.GetWallPolymers().begin(); iterWallPoly!=m_riState.GetWallPolymers().end(); iterWallPoly++ )
			{
				// Head bead of wall polymers is fixed in the wall

				CAbstractBead* pHead = (*iterWallPoly)->GetHead();

				m_outStream << (*iterWallPoly)->GetId()	<< " " << (*iterWallPoly)->GetType() << " ";
				m_outStream << pHead->GetId()			<< " " << pHead->GetType()			 << " ";
				m_outStream << pHead->GetRadius()		<< " ";
				m_outStream << pHead->GetXPos()			<< " " << pHead->GetYPos()			 << " " << pHead->GetZPos()   << " ";
				m_outStream << pHead->GetunPBCXPos()    << " " << pHead->GetunPBCYPos()      << " " << pHead->GetunPBCZPos()   << " ";
				m_outStream << pHead->GetXMom()			<< " " << pHead->GetYMom()			 << " " << pHead->GetZMom()   << " ";
				m_outStream << pHead->GetXForce()		<< " " << pHead->GetYForce()		 << " " << pHead->GetZForce() << zEndl;
				
				// The next loop is skipped if the wall polymers consist of a single bead

				for(cBeadVectorIterator iterBead=(*iterWallPoly)->GetBeads().begin(); iterBead!=(*iterWallPoly)->GetBeads().end(); iterBead++)
				{
					xp[0] = (*iterBead)->GetXPos();
					xp[1] = (*iterBead)->GetYPos();
					xp[2] = (*iterBead)->GetZPos();

	                if(CheckBeadWithinBox((*iterBead)->GetId(), (*iterBead)->GetType(), xp[0], xp[1], xp[2]))
                    {
					    m_outStream << (*iterWallPoly)->GetId()		<< " " << (*iterWallPoly)->GetType()	<< " ";
				        m_outStream << (*iterBead)->GetId()			<< " " << (*iterBead)->GetType()		<< " ";
				        m_outStream << (*iterBead)->GetRadius()		<< " ";
				        m_outStream << xp[0]						<< " " << xp[1]							<< " " << xp[2]   << " ";
				        m_outStream << (*iterBead)->GetunPBCXPos()  << " " << (*iterBead)->GetunPBCYPos()   << " " << (*iterBead)->GetunPBCZPos()   << " ";
				        m_outStream << (*iterBead)->GetXMom()		<< " " << (*iterBead)->GetYMom()		<< " " << (*iterBead)->GetZMom()		<< " ";
				        m_outStream << (*iterBead)->GetXForce()		<< " " << (*iterBead)->GetYForce()		<< " " << (*iterBead)->GetZForce()		<< zEndl;
                    }
                    else
                    {
		                return ErrorTrace("Wall bead coordinate error writing restart state");
                    }
				}
			}
		}

        // Store the stream position so that other classes can append data if needed

        m_outPos = m_outStream.tellp();
	}
	else
	{
	// **********************************************************************
		// Read data from a restart file.

		// First, the constraints on the SimBox. Even if there is no wall or other
		// constraints we still expect the flags to be read in. We check that 
		// the data in the restart state is consistent with that in the new control
		// data file. This means that if the restart state had a wall on, the new
		// control data file must also have the wall turned on. It can be turned 
		// off, if required, by a command issued in the new control data file.

		bool bConstraint;

		m_inStream >> bConstraint;
		if(!m_inStream.good() || bConstraint != m_riState.IsWallPresent())
		{
			return ErrorTrace("Error reading Wall constraint");
		}

		m_inStream >> bConstraint;
		if(!m_inStream.good() || bConstraint != m_riState.IsGravityPresent())
		{
			return ErrorTrace("Error reading Gravity constraint");
		}

		m_inStream >> bConstraint;
		if(!m_inStream.good() || bConstraint != m_riState.IsShearPresent())
		{
			return ErrorTrace("Error reading Shear constraint");
		}

		// Read the polymer id and type and bead id and check that they 
		// agree with the new polymers created in the CInitialState object. Note
        // that the polymer and bead id's cannot be changed between runs, nor can
        // the polymer types. But bead types may be different to allow the user to
        // create command targets and new bead types. Hence, we have to use the bead
        // type specified in the restart file and not the value that is generated
        // by default from the CDF. If the data are consistent, 
		// read the old bead coordinates and copy them into the new initial state, 
		// thus recreating the old configuration. If an error is found return the
		// id and type of the bead and exit. We allow beads to have a type larger
        // than that expected from the original CDF, as this may be an inclusive
        // restart state being read in, but we store the largest type found so that 
        // the data can be checked for consistency.
		//
		// If the flag UseDPDBeadRadii is set then read in the bead radii but allow
		// them to be different from the ones in the previous run.
	
		long polyId, polyType;
		long beadId, beadType;
		double radius;
		double unPBCxPos, unPBCyPos, unPBCzPos;
		double xMom, yMom, zMom;
		double xForce, yForce, zForce;

        double xp[3];
//		m_inStream.precision(8);

		for(cPolymerVectorIterator iterPoly=m_riState.GetPolymers().begin(); iterPoly!=m_riState.GetPolymers().end(); iterPoly++ )
		{
			for(cBeadVectorIterator iterBead=(*iterPoly)->GetBeads().begin(); iterBead!=(*iterPoly)->GetBeads().end(); iterBead++)
			{
				m_inStream >> polyId >> polyType >> beadId >> beadType >> radius;

				if( m_inStream.good() &&
					polyId		== (*iterPoly)->GetId()		&& 
					polyType	== (*iterPoly)->GetType()	&&
				    beadId		== (*iterBead)->GetId() )
				{
					m_inStream >> xp[0]   >> xp[1]   >> xp[2];
					m_inStream >> unPBCxPos   >> unPBCyPos   >> unPBCzPos;
					m_inStream >> xMom   >> yMom   >> zMom;
					m_inStream >> xForce >> yForce >> zForce;

                    // Check that the coordinates of all beads lie within the SimBox.
                    // If the user has modified the restart state by hand they may
                    // have introduced errors.

					if(m_inStream.good() && CheckBeadWithinBox(beadId, beadType, xp[0], xp[1], xp[2]))
					{
                        if(beadType > m_ExpectedLargestBeadType)
                        {
                            m_bUnexpectedBeadTypeFound = true;
                            m_LargestBeadTypeFound = beadType;
                        }

						(*iterBead)->SetType(beadType);
						(*iterBead)->SetRadius(radius);

						(*iterBead)->SetXPos(xp[0]);
						(*iterBead)->SetYPos(xp[1]);
						(*iterBead)->SetZPos(xp[2]);

						(*iterBead)->SetInitialXPos(unPBCxPos);
						(*iterBead)->SetInitialYPos(unPBCyPos);
						(*iterBead)->SetInitialZPos(unPBCzPos);

						(*iterBead)->SetunPBCXPos(unPBCxPos);
						(*iterBead)->SetunPBCYPos(unPBCyPos);
						(*iterBead)->SetunPBCZPos(unPBCzPos);

						(*iterBead)->SetXMom(xMom);
						(*iterBead)->SetYMom(yMom);
						(*iterBead)->SetZMom(zMom);

						(*iterBead)->SetXForce(xForce);
						(*iterBead)->SetYForce(yForce);
						(*iterBead)->SetZForce(zForce);
					}
					else
					{
						TraceInt2("Polymer coords", polyId, beadId);
						return ErrorTrace("Error reading bead coordinates");
					}
				}
				else
				{
					TraceInt2("Polymer id", polyId, beadId);
					return ErrorTrace("Error reading restart state polymer/bead ids");
				}
			}
		}

//		m_inStream.precision(6);

		// Now read the wall polymers if there are any. 

		if(m_riState.IsWallPresent())
		{
			for(cPolymerVectorIterator iterWallPoly=m_riState.GetWallPolymers().begin(); iterWallPoly!=m_riState.GetWallPolymers().end(); iterWallPoly++ )
			{
				// Head bead first

				CAbstractBead* pHead = (*iterWallPoly)->GetHead();

				m_inStream >> polyId >> polyType >> beadId >> beadType >> radius;

				if( m_inStream.good() &&
					polyId		== (*iterWallPoly)->GetId()		&& 
					polyType	== (*iterWallPoly)->GetType()	&&
					beadId		== pHead->GetId() )
				{
					m_inStream >> xp[0]   >> xp[1]   >> xp[2];
					m_inStream >> unPBCxPos   >> unPBCyPos   >> unPBCzPos;
					m_inStream >> xMom   >> yMom   >> zMom;
					m_inStream >> xForce >> yForce >> zForce;

					if(m_inStream.good() && CheckBeadWithinBox(beadId, beadType, xp[0], xp[1], xp[2]))
					{
                        if(beadType > m_ExpectedLargestBeadType)
                        {
                            m_bUnexpectedBeadTypeFound = true;
                            m_LargestBeadTypeFound = beadType;
                        }

                        pHead->SetType(beadType);
						pHead->SetRadius(radius);

						pHead->SetXPos(xp[0]);
						pHead->SetYPos(xp[1]);
						pHead->SetZPos(xp[2]);

						pHead->SetInitialXPos(unPBCxPos);
						pHead->SetInitialYPos(unPBCyPos);
						pHead->SetInitialZPos(unPBCzPos);

						pHead->SetunPBCXPos(unPBCxPos);
						pHead->SetunPBCYPos(unPBCyPos);
						pHead->SetunPBCZPos(unPBCzPos);

						pHead->SetXMom(xMom);
						pHead->SetYMom(yMom);
						pHead->SetZMom(zMom);

						pHead->SetXForce(xForce);
						pHead->SetYForce(yForce);
						pHead->SetZForce(zForce);
					}
					else
					{
						TraceInt2("Wall Polymer head coords", polyId, beadId);
						return ErrorTrace("Error reading wall polymer head coordinates");
					}
				}
				else
				{
					TraceInt2("Wall Polymer head id", polyId, beadId);
					return ErrorTrace("Error reading restart state wall polymer head id");
				}

				// Now the rest of the wall polymer's beads: this will be skipped if
				// the wall polymer is a single bead.

				for(cBeadVectorIterator iterBead=(*iterWallPoly)->GetBeads().begin(); iterBead!=(*iterWallPoly)->GetBeads().end(); iterBead++)
				{
					m_inStream >> polyId >> polyType >> beadId >> beadType >> radius;

					if( m_inStream.good() &&
						polyId		== (*iterWallPoly)->GetId()		&& 
						polyType	== (*iterWallPoly)->GetType()	&&
						beadId		== (*iterBead)->GetId() )
					{
						m_inStream >> xp[0]   >> xp[1]   >> xp[2];
						m_inStream >> unPBCxPos   >> unPBCyPos   >> unPBCzPos;
						m_inStream >> xMom   >> yMom   >> zMom;
						m_inStream >> xForce >> yForce >> zForce;

					    if(m_inStream.good() && CheckBeadWithinBox(beadId, beadType, xp[0], xp[1], xp[2]))
					    {
                            if(beadType > m_ExpectedLargestBeadType)
                            {
                                m_bUnexpectedBeadTypeFound = true;
                                m_LargestBeadTypeFound = beadType;
                            }

						    (*iterBead)->SetType(beadType);
						    (*iterBead)->SetRadius(radius);

						    (*iterBead)->SetXPos(xp[0]);
						    (*iterBead)->SetYPos(xp[1]);
						    (*iterBead)->SetZPos(xp[2]);

						    (*iterBead)->SetInitialXPos(xp[0]);
						    (*iterBead)->SetInitialYPos(xp[1]);
						    (*iterBead)->SetInitialZPos(xp[2]);

						    (*iterBead)->SetunPBCXPos(unPBCxPos);
						    (*iterBead)->SetunPBCYPos(unPBCyPos);
						    (*iterBead)->SetunPBCZPos(unPBCzPos);

						    (*iterBead)->SetXMom(xMom);
						    (*iterBead)->SetYMom(yMom);
						    (*iterBead)->SetZMom(zMom);

						    (*iterBead)->SetXForce(xForce);
						    (*iterBead)->SetYForce(yForce);
						    (*iterBead)->SetZForce(zForce);
					    }
					    else
					    {
						    TraceInt2("Wall Polymer coords", polyId, beadId);
						    return ErrorTrace("Error reading wall polymer coordinates");
					    }
					}
					else
					{
						TraceInt2("Wall Polymer id", polyId, beadId);
						return ErrorTrace("Error reading restart state wall polymer id");
					}
				}
			}
		}

        // Store the stream position so that other classes can read more data if needed

        m_inPos = m_inStream.tellg();
	}

	return true;
}

// Private helper function to check if a bead's coordinates are outside the
// SimBox or within a fixed, small distance of the boundaries, or well within 
// the SimBox. In the first case, the function returns false. The second and third
// cases return true, but the bead coordinates are shifted to the origin if they
// lie within m_CoordErrorLimit of any boundary. The first two cases issue messages
// to the log file to identify the error and to record the fact that the coordinates
// have been shifted.

bool CRestartState::CheckBeadWithinBox(long id, long type, double& xp, double& yp, double& zp)
{
    bool bWithinBox = true;

    if(xp > m_SimBoxXLength) // Bead outside box
	{
        bWithinBox = false;
		CLogRestartStateBuilderError* pMsg = new CLogRestartStateBuilderError(m_CurrentTime, "Bead "+ ToString(id) + " (type " + ToString(type) + ") X coord " + ToString(xp) + " outside box");
    }
	else if(xp > m_SimBoxXMinusEpsilon || xp < m_CoordErrorLimit)	// Bead close to boundary
    {
        if(AreWarningMessagesLogged())
 		    CLogRestartStateBuilderWarning* pMsg = new CLogRestartStateBuilderWarning(m_CurrentTime, "Bead "+ ToString(id) + " (type " + ToString(type) + ") X coord " + ToString(xp) + " shifted to origin");

        xp = m_CoordErrorLimit;
   }

	if(yp > m_SimBoxYLength)
	{
        bWithinBox = false;
		CLogRestartStateBuilderError* pMsg = new CLogRestartStateBuilderError(m_CurrentTime, "Bead "+ ToString(id) + " (type " + ToString(type) + ") Y coord " + ToString(yp) + " outside box");
	}
	else if(yp > m_SimBoxYMinusEpsilon || yp < m_CoordErrorLimit)	// Bead close to boundary
    {
        if(AreWarningMessagesLogged())
            CLogRestartStateBuilderWarning* pMsg = new CLogRestartStateBuilderWarning(m_CurrentTime, "Bead "+ ToString(id) + " (type " + ToString(type) + ") Y coord " + ToString(yp) + " shifted to origin");

        yp = m_CoordErrorLimit;
    }

	if(zp > m_SimBoxZLength)
	{
        bWithinBox = false;
		CLogRestartStateBuilderError* pMsg = new CLogRestartStateBuilderError(m_CurrentTime, "Bead "+ ToString(id) + " (type " + ToString(type) + ") Z coord " + ToString(zp) + " outside box");
	}
	else if(zp > m_SimBoxZMinusEpsilon || zp < m_CoordErrorLimit)	// Bead close to boundary
    {
        if(AreWarningMessagesLogged())
 		    CLogRestartStateBuilderWarning* pMsg = new CLogRestartStateBuilderWarning(m_CurrentTime, "Bead "+ ToString(id) + " (type " + ToString(type) + ") Z coord " + ToString(zp) + " shifted to origin");

        zp = m_CoordErrorLimit;
    }

    return bWithinBox;
}

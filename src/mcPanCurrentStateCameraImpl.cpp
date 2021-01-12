/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mcPanCurrentStateCameraImpl.cpp: implementation of the mcPanCurrentStateCameraImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimMPSFlags.h"
#include "mcPanCurrentStateCameraImpl.h"
#include "mcPanCurrentStateCamera.h"
#include "mcSetDisplayPeriod.h"
#include "Monitor.h"
#include "ISimBox.h"
#include "mcSetCurrentStateCamera.h"
#include "mcSaveCurrentState.h"
#include "LogPanCurrentStateCamera.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

mcPanCurrentStateCameraImpl::mcPanCurrentStateCameraImpl() : m_Theta(0.0), m_Phi(0.0)
{
	// Zero the rotation matrices and set the initial pan matrix to the unit matrix

	for(short int i=0; i<3; i++)
	{
		m_OriginalCamera[i] = 0.0;
		m_OriginalTarget[i] = 0.0;
		m_Normal[i]			= 0.0;

		for(short int j=0; j<3; j++)
		{
			m_RNormal[i][j] = 0.0;
			m_RPan[i][j]    = 0.0;

			if(i == j)
				m_RPan[i][i] = 1.0;
		}
	}
}

mcPanCurrentStateCameraImpl::~mcPanCurrentStateCameraImpl()
{

}

// Command handler to set the coordinates of the camera and target in the
// CCurrentState snapshot file so that the camera pans across from an initial
// position to a final position. The angle of pan, number of snapshots created 
// during the pan, and the time steps between snapshots are all specified by 
// the user, as are options to start and finish at the original camera location 
// or to jump to a new position and stay at the final position.
// The snapshots are issued using the current output format: this allows
// different animation options using PovRay or Amira.
//
// The coordinates are specified as multiples of the SimBox size and converted to 
// actual coordinates before passing them to the CCurrentState.

void mcPanCurrentStateCameraImpl::PanCurrentStateCamera(const xxCommand* const pCommand)
{
#if EnableMonitorCommand == SimCommandEnabled
	const mcPanCurrentStateCamera* const pCmd = dynamic_cast<const mcPanCurrentStateCamera*>(pCommand);

	CMonitor* const pMon = dynamic_cast<CMonitor*>(this);

	const bool bJumpToInitial	= pCmd->GetJumpToInitialFlag();
	const bool bStayAtFinal		= pCmd->GetStayAtFinalFlag();
	const double panAngle       = pCmd->GetPanAngle();			// radians
	const long duration			= pCmd->GetDuration();			// sec
	const long frameRate		= pCmd->GetFrameRate();			// per sec
	const long stepsPerFrame	= pCmd->GetStepsPerFrame();

	const long snapshotTotal	= duration*frameRate;
	const double dPsi			= panAngle/snapshotTotal;		// pan angle may be < 0

	// Store the normal vector, and the original look-at and camera locations 
	// in absolute units. If the user changes the initial camera location from 
	// the current one, it can be restored later.

	short int i, j;	// Counters used many times below

	for(i=0; i<3; i++)
	{
		m_Normal[i]	= static_cast<double>(pCmd->GetNormal(i)); // User input has integer components

		m_OriginalCamera[i] = pMon->m_Camera[i];	// Absolute units
		m_OriginalTarget[i] = pMon->m_Target[i];
	}	

	double initialCamera[3];
	double initialTarget[3];

	if(bJumpToInitial)
	{
		initialCamera[0] = pCmd->GetCamera(0);	// Relative to SimBox size
		initialCamera[1] = pCmd->GetCamera(1);
		initialCamera[2] = pCmd->GetCamera(2);

		initialTarget[0] = pCmd->GetTarget(0);
		initialTarget[1] = pCmd->GetTarget(1);
		initialTarget[2] = pCmd->GetTarget(2);
	}
	else
	{
	    // We use the size of the simulation Space to get the absolute positions of the camera/target as
		// this reduces to the SimBox size for the single-processor case.
		
		initialCamera[0] = m_OriginalCamera[0]/pMon->GetSimSpaceXLength();
		initialCamera[1] = m_OriginalCamera[1]/pMon->GetSimSpaceYLength();
		initialCamera[2] = m_OriginalCamera[2]/pMon->GetSimSpaceZLength();

		initialTarget[0] = m_OriginalTarget[0]/pMon->GetSimSpaceXLength();
		initialTarget[1] = m_OriginalTarget[1]/pMon->GetSimSpaceYLength();
		initialTarget[2] = m_OriginalTarget[2]/pMon->GetSimSpaceZLength();
	}

	// Translate the camera so that its look-at point is at the origin of 
	// its coordinate system; then rotate the camera in the new system and
	// translate it back to the original one for display.

	double ct[3];
	double target[3];

	for(i=0; i<3; i++)
	{
		ct[i]		= initialCamera[i] - initialTarget[i];
		target[i]	= initialTarget[i];
	}

	// Now rotate the camera into the frame defined by the user-specified normal vector

	CalculateNRotationMatrix();

	double ctr[3];

	for(i=0; i<3; i++)
	{
		ctr[i] = 0.0;

		for(j=0; j<3; j++)
		{
			ctr[i] += m_RNormal[i][j]*ct[j];
		}
	}

	// Issue commands to pan the camera around the specified axis and save a 
	// snapshot at each location. We use a command rather than the internal
	// CMonitor::SaveCurrentState() function so that we can schedule the 
	// pan to execute as simulation time actually passes. Note that the command
	// expects the camera and target coordinates to be defined as multiples
	// of the SimBox size, but the CMonitor stores the coordinates in absolute
	// units. So we have to convert between them when using the original values.
	//
	// Note that m_RPan[] is initialised to the unit matrix in the constructor

	long   time	 = pMon->GetCurrentTime();
	double psi = 0.0;

	double ctrp[3];
	double camera[3];

	// ****************************************
	// Loop over the camera angles in the pan 
	
	// Because we cannot send the SaveCurrentState command synchronously between processors in the parallel code(it hangs), 
	// we change the displayPeriod using a command at the beginning and end of the pan. We do this for both the serial and parallel
	// codes. Note that we have to put the command into the future otherwise it does not execute properly in the parallel code.
	
	const long oldDisplayPeriod = pMon->m_pSimState->GetDisplayPeriod();
	
	mcSetDisplayPeriod* pDisplayCmd1 = new mcSetDisplayPeriod(time+stepsPerFrame, false, stepsPerFrame);
	pMon->GetISimBox()->AddCommand(pDisplayCmd1);
	
	for(long sn=0; sn<=snapshotTotal; sn++)
	{
		for(i=0; i<3; i++)
		{
			ctrp[i] = 0.0;

			for(j=0; j<3; j++)
			{
				ctrp[i] += m_RPan[i][j]*ctr[j];
			}
		}

		// Now rotate the camera back into the original frame and translate
		// back to look at the target

		for(i=0; i<3; i++)
		{
			camera[i] = 0.0;

			for(j=0; j<3; j++)
			{
				camera[i] += m_RNormal[j][i]*ctrp[j];
			}

			camera[i] += target[i];
		}

		// Use the internal version of the appropriate command to avoid
		// writing hundreds of messages to the log state file.
		
		mcSetCurrentStateCamera* pCameraCmd = new mcSetCurrentStateCamera(time, false, camera, target);

		pMon->GetISimBox()->AddCommand(pCameraCmd);

		time += stepsPerFrame;
		psi  += dPsi;

		m_RPan[0][0] = cos(psi);
		m_RPan[0][1] = sin(psi);
//		m_RPan[0][2] = 0.0;

		m_RPan[1][0] = -sin(psi);
		m_RPan[1][1] = cos(psi);
//		m_RPan[1][2] = 0.0;

//		m_RPan[2][0] = 0.0;
//		m_RPan[2][1] = 0.0;
//		m_RPan[2][2] = 1.0;
	}

    // Reset the display period after the pan to the original value
	
	mcSetDisplayPeriod* pDisplayCmd2 = new mcSetDisplayPeriod(time, false, oldDisplayPeriod);
	pMon->GetISimBox()->AddCommand(pDisplayCmd2);
	
	// ****************************************
	// Restore the original look-at and camera locations if the user has specified that the camera should not stay at its final coordinates.
	// We have to issue a command to reset the coordinates after the pan has finished. Note that the coordinates must be converted back into relative coordinates
	// before we do this. Also note that the command must be executed at a later time than the final pan command otherwise it will overwrite it.
	
	if(!bStayAtFinal)
	{
		camera[0] = m_OriginalCamera[0]/pMon->GetSimSpaceXLength();
		camera[1] = m_OriginalCamera[1]/pMon->GetSimSpaceYLength();
		camera[2] = m_OriginalCamera[2]/pMon->GetSimSpaceZLength();
		
		pMon->GetISimBox()->AddCommand(new mcSetCurrentStateCamera(time, false, camera, target));
	}

#if EnableParallelCommands == SimMPSEnabled
        if(xxParallelBase::GlobalGetRank() == 0)
        {
	        new CLogPanCurrentStateCamera(pMon->GetCurrentTime(), 
														bJumpToInitial, bStayAtFinal,
														duration, frameRate, stepsPerFrame,
														panAngle*180.0/xxBase::m_globalPI,
														m_Normal, initialCamera, initialTarget);
        }
        else
        {
            // send a pmParallelCommandFailed error message back to P0 so that it can
            // log a CLogParallelCommandFailed message to the log file.
        }
#else
	        new CLogPanCurrentStateCamera(pMon->GetCurrentTime(), 
														bJumpToInitial, bStayAtFinal,
														duration, frameRate, stepsPerFrame,
														panAngle*180.0/xxBase::m_globalPI,
														m_Normal, initialCamera, initialTarget);
#endif

#endif  // EnableMonitorCommand block
}

// Private helper function to calculate the rotation matrix that takes the
// frame containing the camera and target into one whose Z axis lies along the
// user-sepcified normal direction. If the normal vector, m_Normal, is null it is
// replaced by the Z unit vector, otherwise it is normalized and the polar and 
// azimuthal angles are stored in the member variables m_Normal, m_Theta, m_Phi.
// The rotation matrix is then stored in m_RNormal.

void mcPanCurrentStateCameraImpl::CalculateNRotationMatrix()
{
	short int i;

	double vmag = 0.0;

	for(i=0; i<3; i++)
	{
		vmag += m_Normal[i]*m_Normal[i];
	}

	vmag = sqrt(vmag);

	// Guard against a zero normal vector

	if(vmag > 1.0e-08)
	{
		for(i=0; i<3; i++)
		{
			m_Normal[i] = m_Normal[i]/vmag;
		}

		m_Theta = 0.0;
		m_Phi   = 0.0;

		double ctheta = m_Normal[2]/vmag;

		if(fabs(ctheta) > 1.0e-06)
		{
			m_Theta = acos(ctheta);
		}
		else
			m_Theta = 0.5*xxBase::m_globalPI;


		if(m_Normal[0] > 1.0e-06)	// X component > 0
		{
			if(m_Normal[1] > 1.0e-06)			// Y component > 0
			{
				m_Phi = atan(m_Normal[1]/m_Normal[0]);
			}
			else if(m_Normal[1] < -1.0e-06)		// Y component < 0
			{
				m_Phi = atan(m_Normal[1]/m_Normal[0]);
				m_Phi  = xxBase::m_globalTwoPI - m_Phi;
			}
			else
			{
				m_Phi = 0.0;
			}
		}
		else if(m_Normal[0] < -1.0e-06)	// X component < 0
		{
			if(m_Normal[1] > 1.0e-06)			// Y component > 0
			{
				m_Phi = atan(m_Normal[1]/m_Normal[0]);
				m_Phi  = xxBase::m_globalPI - m_Phi;
			}
			else if(m_Normal[1] < -1.0e-06)		// Y component < 0
			{
				m_Phi = atan(m_Normal[1]/m_Normal[0]);
				m_Phi  = xxBase::m_globalPI + m_Phi;
			}
			else
			{
				m_Phi = xxBase::m_globalPI;
			}
		}
		else	// X component = 0
		{
			if(m_Normal[1] > 1.0e-06)
			{
				m_Phi = 0.5*xxBase::m_globalPI;
			}
			else if(m_Normal[1] < -1.0e-06)		// Y component < 0
			{
				m_Phi = 1.5*xxBase::m_globalPI;
			}
			else
			{
				m_Phi = 0.0;
			}
		}

		// Specify the rotation matrix

		m_RNormal[0][0] = cos(m_Theta)*cos(m_Phi);
		m_RNormal[0][1] = cos(m_Theta)*sin(m_Phi);
		m_RNormal[0][2] = -sin(m_Theta);

		m_RNormal[1][0] = -sin(m_Phi);
		m_RNormal[1][1] = cos(m_Phi);
		m_RNormal[1][2] = 0.0;

		m_RNormal[2][0] = sin(m_Theta)*cos(m_Phi);
		m_RNormal[2][1] = sin(m_Theta)*sin(m_Phi);
		m_RNormal[2][2] = cos(m_Theta);
	}
	else
	{
		// Set the normal to the Z axis

		m_RNormal[0][0] = 1.0;
		m_RNormal[0][1] = 0.0;
		m_RNormal[0][2] = 0.0;

		m_RNormal[1][0] = 0.0;
		m_RNormal[1][1] = 1.0;
		m_RNormal[1][2] = 0.0;

		m_RNormal[2][0] = 0.0;
		m_RNormal[2][1] = 0.0;
		m_RNormal[2][2] = 1.0;

		m_Normal[0] = 0.0;
		m_Normal[1] = 0.0;
		m_Normal[2] = 1.0;

		m_Theta = 0.0;
		m_Phi   = 0.0;
	}
}

/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mcZoomCurrentStateCameraImpl.cpp: implementation of the mcZoomCurrentStateCameraImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "mcZoomCurrentStateCameraImpl.h"
#include "mcZoomCurrentStateCamera.h"
#include "Monitor.h"
#include "ISimBox.h"
#include "mcSetCurrentStateCamera.h"
#include "mcSaveCurrentState.h"
#include "LogZoomCurrentStateCamera.h"
#include "LogCommandFailed.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

mcZoomCurrentStateCameraImpl::mcZoomCurrentStateCameraImpl() : m_OriginalSeparation(0.0),
															   m_FinalSeparation(0.0)
{
	// Zero the original camera/target coordinates

	for(short int i=0; i<3; i++)
	{
		m_OriginalCamera[i] = 0.0;
		m_OriginalTarget[i] = 0.0;
		m_FinalCamera[i]    = 0.0;
	}
}

mcZoomCurrentStateCameraImpl::~mcZoomCurrentStateCameraImpl()
{

}

// Command handler to set the coordinates of the camera and target in the
// CCurrentState snapshot file so that the camera zooms from an initial
// position to a final position. The scale factor of the zoom, number of 
// snapshots created during the zoom, and the time steps between snapshots 
// are all specified by the user.
// The snapshots are issued using the current output format: this allows
// different animation options using PovRay or Amira.
//
// The coordinates are specified as multiples of the SimBox size and converted to 
// actual coordinates before passing them to the CCurrentState.

void mcZoomCurrentStateCameraImpl::ZoomCurrentStateCamera(const xxCommand* const pCommand)
{
#if EnableMonitorCommand == SimCommandEnabled

	const mcZoomCurrentStateCamera* const pCmd = dynamic_cast<const mcZoomCurrentStateCamera*>(pCommand);

	CMonitor* const pMon = dynamic_cast<CMonitor*>(this);

	const double scaleFactor    = pCmd->GetScaleFactor();		// per snapshot
	const long duration			= pCmd->GetDuration();			// sec
	const long frameRate		= pCmd->GetFrameRate();			// per sec
	const long stepsPerFrame	= pCmd->GetStepsPerFrame();

	const long snapshotTotal	= duration*frameRate;

	// Store the original look-at and camera locations in absolute units as obtained
	// directly from the CMonitor.

	short int i;	// Counters used many times below

	for(i=0; i<3; i++)
	{
		m_OriginalCamera[i] = pMon->m_Camera[i];
		m_OriginalTarget[i] = pMon->m_Target[i];
	}

	// Translate the camera so that its look-at point is at the origin of 
	// its coordinate system; then zoom the camera in the new system and
	// translate it back to the original one for display.

	double ctvec[3];		// vector from target to camera's initial position
	double dctvec = 0.0;	// magnitude of vector from target to camera
	double ect[3];			// unit vector from target to camera

	for(i=0; i<3; i++)
	{
		ctvec[i] = m_OriginalCamera[i] - m_OriginalTarget[i];
		dctvec   += ctvec[i]*ctvec[i];
	}

	m_OriginalSeparation = sqrt(dctvec);
	m_FinalSeparation    = scaleFactor*m_OriginalSeparation;

	// If the camera and target are coincident, the normal is null, the scale factor
	// is negative, or the number of snapshots is zero or less the command fails.
	// Define the unit vector along the zoom axis as the vector from the initial
	// camera position to its final position.

	if(m_OriginalSeparation > 0.001 && m_FinalSeparation > 0.0 && snapshotTotal > 0)
	{
		double ezoom[3];	// unit vector from initial camera to final camera position
		double dzoom = 0.0;

		for(i=0; i<3; i++)
		{
			ect[i]           = ctvec[i]/m_OriginalSeparation;
			m_FinalCamera[i] = m_OriginalTarget[i] + m_FinalSeparation*ect[i];
			ezoom[i]         = m_FinalCamera[i] - m_OriginalCamera[i];
			dzoom			+= ezoom[i]*ezoom[i];
		}

		dzoom = sqrt(dzoom);

		// We allow a scale factor of 1 so that the zoom has no effect

		if(dzoom > 0.001)
		{
			for(i=0; i<3; i++)
			{
				ezoom[i] /= dzoom;
			}
		}
		else
		{
			for(i=0; i<3; i++)
			{
				ezoom[i] = 0.0;
				dzoom    = 0.0;
			}
		}

		// Issue commands to zoom the camera by a given amount over the specified 
		// time interval saving a snapshot at each location. We use a command rather 
		// than the internal CMonitor::SaveCurrentState() function so that we can 
		// schedule the zoom to execute as simulation time actually passes. 
		// Note that the command expects the camera and target coordinates to be 
		// defined as multiples of the SimBox size, but the CMonitor stores the 
		// coordinates in absolute units. So we have to convert between them when 
		// using the original values.

		// ****************************************
		// Loop over the snapshots in the zoom and convert the coordinates back
		// to values relative to the SimBox side lengths

		long time = pMon->GetCurrentTime();
		const double zoomStep = dzoom/static_cast<double>(snapshotTotal);

		double camera[3];
		double target[3];

		for(long sn=0; sn<=snapshotTotal; sn++)
		{
			for(i=0; i<3; i++)
			{
				camera[i] = m_OriginalCamera[i] + static_cast<double>(sn)*zoomStep*ezoom[i];
				target[i] = m_OriginalTarget[i];
			}

			camera[0] /= pMon->m_SimBoxXLength;
			camera[1] /= pMon->m_SimBoxYLength;
			camera[2] /= pMon->m_SimBoxZLength;

			target[0] /= pMon->m_SimBoxXLength;
			target[1] /= pMon->m_SimBoxYLength;
			target[2] /= pMon->m_SimBoxZLength;

			// Use the internal version of the appropriate commands to avoid
			// writing hundreds of messages to the log state file. Note that the
			// commands expect the coordinates to be specified relative to the
			// SimBox side lengths.

			mcSetCurrentStateCamera* pCameraCmd = new mcSetCurrentStateCamera(time, false, camera, target);
			mcSaveCurrentState* pSaveCmd		= new mcSaveCurrentState(time, false);

			pMon->GetISimBox()->AddCommand(pCameraCmd);
			pMon->GetISimBox()->AddCommand(pSaveCmd);

			time += stepsPerFrame;
		}

		// ****************************************

		new CLogZoomCurrentStateCamera(pMon->GetCurrentTime(), duration, frameRate, stepsPerFrame, scaleFactor, m_OriginalCamera, m_OriginalTarget);
	}
	else
	{
		 new CLogCommandFailed(pMon->GetCurrentTime(), pCmd);
	}
#endif
}



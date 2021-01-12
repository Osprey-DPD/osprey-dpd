/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctSelectBeadsInSphericalTargetRegionImpl.cpp: implementation of the ctSelectBeadsInSphericalTargetRegionImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "ctSelectBeadsInSphericalTargetRegionImpl.h"
#include "ctSelectBeadsInSphericalTargetRegion.h"
#include "SimBox.h"
#include "IModifySimStateBondTypes.h"
#include "Bead.h"
#include "Bond.h"
#include "Polymer.h"
#include "CommandTarget.h"
#include "CommandTargetComposite.h"
#include "LogctSelectBeadsInSphericalTargetRegion.h"
#include "LogTextMessage.h"

#include "CNTCell.h"	// Needed for the RNG

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctSelectBeadsInSphericalTargetRegionImpl::ctSelectBeadsInSphericalTargetRegionImpl()
{
}

ctSelectBeadsInSphericalTargetRegionImpl::~ctSelectBeadsInSphericalTargetRegionImpl()
{

}

// Command handler to create a simple bead target out of all beads of a specified type
// that belong to a named target and whose centres lie within a region defined 
// by specified spherical polar coordinates. The source target can be any kind of
// target that has a GetBeads() function. 
//
// NB. This command currently assumes that the original target lies completely within
// the simulation box and does NOT cross the PBCs. If we relax this assumption, the
// implementation must be modified to calculate the CM correctly when some parts of the
// target are in different periodic images of the box.
//
// Because we use spherical polar coordinates, we have to take care of the special cases
// when phi is centred on x axis (phi1 < 0) and if particles lie on the x,y or z axes.
// Particularly, if phi1 < 0, we calculate the phi angle so that it is negative and 
// less than 90 degrees instead of being between 270 and 360.
//
// We first collect all the appropriate beads, then select those that lie within the
// region of the target defined by the given spherical polar coordinates. This means
// that the distance of their centres from the target's CM must be within the radial
// range and the angular range.

void ctSelectBeadsInSphericalTargetRegionImpl::SelectBeadsInSphericalTargetRegion(const xxCommand* const pCommand)
{
	const ctSelectBeadsInSphericalTargetRegion* const pCmd = dynamic_cast<const ctSelectBeadsInSphericalTargetRegion*>(pCommand);

	const zString sourceLabel = pCmd->GetSourceLabel();
	const zString beadLabel   = pCmd->GetNewTargetLabel();
    const zString beadName    = pCmd->GetBeadName();
    const double  radius1     = pCmd->GetInnerRadius();
    const double  radius2     = pCmd->GetOuterRadius();
    const double  theta1      = pCmd->GetLowerPolarAngle();  // Angles assumed to be in radians
    const double  theta2      = pCmd->GetUpperPolarAngle();
    const double  phi1        = pCmd->GetLowerAzimuthalAngle(); // phi1 is allowed to be negative
    const double  phi2        = pCmd->GetUpperAzimuthalAngle(); // phi2 cannot be negative

    // Derived data

	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);

    const long beadType    = pSimBox->GetBeadTypeFromName(beadName);
    const double  r1sq     = radius1*radius1;
    const double  r2sq     = radius2*radius2;
    const double  ctheta1  = cos(theta1);
    const double  ctheta2  = cos(theta2);


	// Get the command target from the target list collect all of the beads of
    // the specified type in it

	CCommandTargetNode*   pCmdTarget = pSimBox->GetCommandTarget(sourceLabel);

    BeadVector vSourceBeads = pCmdTarget->GetBeads();

    // Now select the beads of the specified type, and calculate the target's CM
    // coordinates. Note that the CM depends on whether the target crosses the 
    // PBCs or not. For now, we assume that it does not.

    double xcm, ycm, zcm;
    xcm = 0.0;
    ycm = 0.0;
    zcm = 0.0;

    BeadVector vNewBeads;
    vNewBeads.clear();

    for(BeadVectorIterator iterBead = vSourceBeads.begin(); iterBead != vSourceBeads.end(); iterBead++)
	{
        xcm += (*iterBead)->GetXPos();
        ycm += (*iterBead)->GetYPos();
        zcm += (*iterBead)->GetZPos();

        if((*iterBead)->GetType() == beadType)
        {
            vNewBeads.push_back(*iterBead);
        }
	}

	// Now we have the set of beads, we find those that lie in the specified region;
    // if the target does not contain any beads of the specified type, the command fails.
    // In this case, we do not calculate the CM coordinates at all as there are none.

	if(pCmdTarget && vNewBeads.size() > 0)
	{
        xcm /= static_cast<double>(vSourceBeads.size());
        ycm /= static_cast<double>(vSourceBeads.size());
        zcm /= static_cast<double>(vSourceBeads.size());

        BeadVector vNewTargetBeads;
        vNewTargetBeads.clear();

        double xi, yi, zi;
        double dxi, dyi, dzi;
        double dr, dr2, ctheta, phi;

        for(BeadVectorIterator iterBead1 = vNewBeads.begin(); iterBead1 != vNewBeads.end(); iterBead1++)
	    {
            xi = (*iterBead1)->GetXPos();
            yi = (*iterBead1)->GetYPos();
            zi = (*iterBead1)->GetZPos();

            dxi = xi - xcm;
            dyi = yi - ycm;
            dzi = zi - zcm;

            dr2 = dxi*dxi + dyi*dyi + dzi*dzi;

            // First the radial test then the angular tests (in radians!)

            if(dr2 > r1sq && dr2 < r2sq)
            {
                dr = sqrt(dr2);
                ctheta = dzi/dr;  // this is negative for theta > 90 deg

                if(fabs(dyi) < 0.000001)
                {
                    // Special case of zero y coordinate, so phi is 0 or 180 degrees
                    // depending on the sign of x coordinate. If x coordinate is
                    // also zero, we set phi = 0.

                    if(fabs(dxi) < 0.000001)
                    {
                        phi = 0.0;
                    }
                    else if (dxi > 0.0)
                    {
                        phi = 0.0;
                    }
                    else
                    {
                        phi = xxBase::m_globalPI;
                    }
                }
                else if(dyi > 0.0) // 0 < phi < 180
                {
                    if(fabs(dxi) < 0.000001)
                    {
                        // Special case of zero x coordinate, so phi is 90 degrees

                        phi = xxBase::m_globalPIByTwo;
                    }
                    else if(dxi < 0.0) // second quadrant
                    {
                        phi = xxBase::m_globalPI + atan(dyi/dxi);
                    }
                    else        // first quadrant
                    {
                        phi = atan(dyi/dxi);
                    }
                }
                else  // 180 < phi < 360
                {
                    if(fabs(dxi) < 0.000001)
                    {
                        // Special case of zero x coordinate, so phi is 270 degrees

                        phi = 1.5*xxBase::m_globalPI;
                    }
                    else if(dxi < 0.0)   // third quadrant
                    {
                        phi = xxBase::m_globalPI + atan(dyi/dxi);
                    }
                    else  // fourth quadrant: sign of phi depends on limits
                    {
                        if(phi1*phi2 > 0.0)
                        {
                            phi = xxBase::m_globalTwoPI + atan(dyi/dxi);
                        }
                        else
                        {
                            phi = -atan(dyi/dxi);
                        }
                    }
                }

                if(ctheta < ctheta1 && ctheta > ctheta2 &&
                   phi > phi1 && phi < phi2)
                {
                    vNewTargetBeads.push_back(*iterBead1);
                }
            }
	    }

		// Create and store a CCommandTarget to hold the beads and receive commands.
		// We pass in the bead type in case the vNewTargetBeads container is empty as
		// we still want to know what type of bead was specified. If the target is
        // empty, we still create it but it contains no beads.

        const long beadTotal = vNewTargetBeads.size();

		if(pSimBox->CreateCommandTarget(beadLabel, beadType, vNewTargetBeads))
		{
                    new CLogctSelectBeadsInSphericalTargetRegion(pSimBox->GetCurrentTime(), sourceLabel, beadLabel, beadName,
		beadType, beadTotal, radius1, radius2, theta1, theta2, phi1, phi2);
        }
		else
		{
			 new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);
		}

	}
	else
	{
		 new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);
	}
}

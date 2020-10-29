/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// StressGridCell.cpp: implementation of the CStressGridCell class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "StressGridCell.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// Default constructor

CStressGridCell::CStressGridCell()
{
	for(long k=0; k<9; k++)
	{
	    m_aStressSphere[k] = 0.0;
	}	

}

// Constructor used to create the grid of cells that hold the local stress tensor.
// The widths of the cell in units of the CNT cell widths (typically 1)  are set by (dx,dy,dz) and the integer coordinates of its
// bottom-left corner are in coords[3].

CStressGridCell::CStressGridCell(long index, long coords[3], double dx, double dy, double dz) : CAbstractCell()
{
	// Initialise the member variables of CAbstractCell 

	m_id			= index;

	m_BLIndex[0]	= coords[0];
	m_BLIndex[1]	= coords[1];
	m_BLIndex[2]	= coords[2];

	m_BLCoord[0]	= static_cast<double>(coords[0])*dx;
	m_BLCoord[1]	= static_cast<double>(coords[1])*dy;
	m_BLCoord[2]	= static_cast<double>(coords[2])*dz;

	m_TRCoord[0]	= m_BLCoord[0] + dx;
	m_TRCoord[1]	= m_BLCoord[1] + dy;
	m_TRCoord[2]	= m_BLCoord[2] + dz;

	for(long k=0; k<9; k++)
	{
	    m_aStressSphere[k] = 0.0;
	}	

}


// Copy constructor

CStressGridCell::CStressGridCell(const CStressGridCell& oldCell) : CAbstractCell(oldCell)
{
	for(long k=0; k<9; k++)
	{
	    m_aStressSphere[k] = oldCell.m_aStressSphere[k];
	}	

}

CStressGridCell::~CStressGridCell()
{

}

// Debug helper function to write out the components of the cell's stress tensor in curvilinear coordinates.

void CStressGridCell::WriteStressTensor() const
{
#if EnableStressTensorSphere == SimMiscEnabled
    std::cout << " Stress tensor for cell " << GetBLXIndex() << " " << GetBLYIndex() << " " << GetBLZIndex() << zEndl;
	std::cout << GetStressComponent(0) << " " << GetStressComponent(1) << " " << GetStressComponent(2) << zEndl;
	std::cout << GetStressComponent(3) << " " << GetStressComponent(4) << " " << GetStressComponent(5) << zEndl;
	std::cout << GetStressComponent(6) << " " << GetStressComponent(7) << " " << GetStressComponent(8) << zEndl;
	std::cout << "**********" << zEndl;
#endif
}

// Function to zero the stress tensor stored in the cell.

void CStressGridCell::Clear()
{
	for(long k=0; k<9; k++)
	{
	    m_aStressSphere[k] = 0.0;
	}	
}

// Function to add a stress tensor to that stored in the cell.

void CStressGridCell::AddStress(double stress[9])
{
	for(long k=0; k<9; k++)
	{
	    m_aStressSphere[k] += stress[k];
	}	
}

// Function to transform the stress tensor from Cartesian coordinates into spherical polar coordinates. The stress tensor must have been
// calculated in Cartesian coordinates and the origin of the spherical coordinate system in the simulation box defined before this function 
// is called. The new tensor is stored in the same array.
//
// We transform the stress tensor from Cartesian coordinates into spherical polar coordinates at the point P in the centre of the cell with
// respect to the origin that is passed as the argument.
// Note that the actual 3x3 matrix is stored in a linear array of 9 elements in row order.

void CStressGridCell::TransformStressToSphCoords(double origin[3])
{
#if EnableStressTensorSphere == SimMiscEnabled

    double p[3], rp2;
	
	p[0] = 0.5*(m_BLCoord[0] + m_TRCoord[0]) - origin[0];
	p[1] = 0.5*(m_BLCoord[1] + m_TRCoord[1]) - origin[1];
	p[2] = 0.5*(m_BLCoord[2] + m_TRCoord[2]) - origin[2];
	
	rp2  = p[0]*p[0] + p[1]*p[1] + p[2]*p[2];
	
	if(rp2 > 0.0)
	{
		const double rp      = sqrt(rp2);
		const double root    = sqrt(p[0]*p[0] + p[1]*p[1]);
		const double invrp2  = 1.0/rp2;
		const double invrp   = 1.0/rp;
		const double invroot = 1.0/root;
		
		double rot[9];
	    rot[0] = p[0]/rp;
	    rot[1] = p[1]/rp;
	    rot[2] = p[2]/rp;
	    rot[3] = p[0]*p[2]/(rp2*root);
	    rot[4] = p[1]*p[2]/(rp2*root);
	    rot[5] = -root/rp2;
		rot[6] = -p[1]/(root*root);
		rot[7] = p[0]/(root*root);
		rot[8] = 0.0;
		
		// Now we have the transformation matrix, we apply it to the stress tensor to transform its components into spherical polar coordinates.
		// After the transformation, we get the physical components of the tensor by projecting it along the coordinate axes. This involves 
		// multiplying each component by the appropriate products of the metric tensor's components.
		//
		// Note the correspondence between the i,j indices of the matrix and i=0...8 of the array:
		//
		//  1,1 -> 0  1,2 -> 1  1,3 -> 2
		//  2,1 -> 3  2,2 -> 4  2,3 -> 5
		//  3,1 -> 6  3,2 -> 7  3,3 -> 8
		//
		//
				
		double aRotatedTensor[9];
		
		// T 1,1
         aRotatedTensor[0] = rot[0]*rot[0]*m_aStressSphere[0] + rot[0]*rot[1]*m_aStressSphere[1] +  rot[0]*rot[2]*m_aStressSphere[2] + 
							 rot[1]*rot[0]*m_aStressSphere[3] + rot[1]*rot[1]*m_aStressSphere[4] +  rot[1]*rot[2]*m_aStressSphere[5] + 
							 rot[2]*rot[0]*m_aStressSphere[6] + rot[2]*rot[1]*m_aStressSphere[7] +  rot[2]*rot[2]*m_aStressSphere[8];
							 
        // This component is unchanged.
		
		// T 1,2
         aRotatedTensor[1] = rot[0]*rot[3]*m_aStressSphere[0] + rot[0]*rot[4]*m_aStressSphere[1] +  rot[0]*rot[5]*m_aStressSphere[2] + 
							 rot[1]*rot[3]*m_aStressSphere[3] + rot[1]*rot[4]*m_aStressSphere[4] +  rot[1]*rot[5]*m_aStressSphere[5] + 
							 rot[2]*rot[3]*m_aStressSphere[6] + rot[2]*rot[4]*m_aStressSphere[7] +  rot[2]*rot[5]*m_aStressSphere[8];
							 	
		aRotatedTensor[1] *= rp;
	
		// T 1,3
         aRotatedTensor[2] = rot[0]*rot[6]*m_aStressSphere[0] + rot[0]*rot[7]*m_aStressSphere[1] +  rot[0]*rot[8]*m_aStressSphere[2] + 
							 rot[1]*rot[6]*m_aStressSphere[3] + rot[1]*rot[7]*m_aStressSphere[4] +  rot[1]*rot[8]*m_aStressSphere[5] + 
							 rot[2]*rot[6]*m_aStressSphere[6] + rot[2]*rot[7]*m_aStressSphere[7] +  rot[2]*rot[8]*m_aStressSphere[8];
	
		aRotatedTensor[2] *= root;
	
		// T 2,1
         aRotatedTensor[3] = rot[3]*rot[0]*m_aStressSphere[0] + rot[3]*rot[1]*m_aStressSphere[1] +  rot[3]*rot[2]*m_aStressSphere[2] + 
							 rot[4]*rot[0]*m_aStressSphere[3] + rot[4]*rot[1]*m_aStressSphere[4] +  rot[4]*rot[2]*m_aStressSphere[5] + 
							 rot[5]*rot[0]*m_aStressSphere[6] + rot[5]*rot[1]*m_aStressSphere[7] +  rot[5]*rot[2]*m_aStressSphere[8];
	
		aRotatedTensor[3] *= rp;
	
		// T 2,2
         aRotatedTensor[4] = rot[3]*rot[3]*m_aStressSphere[0] + rot[3]*rot[4]*m_aStressSphere[1] +  rot[3]*rot[5]*m_aStressSphere[2] + 
							 rot[4]*rot[3]*m_aStressSphere[3] + rot[4]*rot[4]*m_aStressSphere[4] +  rot[4]*rot[5]*m_aStressSphere[5] + 
							 rot[5]*rot[3]*m_aStressSphere[6] + rot[5]*rot[4]*m_aStressSphere[7] +  rot[5]*rot[5]*m_aStressSphere[8];
	
		aRotatedTensor[4] *= rp2;

		// T 2,3
         aRotatedTensor[5] = rot[3]*rot[6]*m_aStressSphere[0] + rot[3]*rot[7]*m_aStressSphere[1] +  rot[3]*rot[8]*m_aStressSphere[2] + 
							 rot[4]*rot[6]*m_aStressSphere[3] + rot[4]*rot[7]*m_aStressSphere[4] +  rot[4]*rot[8]*m_aStressSphere[5] + 
							 rot[5]*rot[6]*m_aStressSphere[6] + rot[5]*rot[7]*m_aStressSphere[7] +  rot[5]*rot[8]*m_aStressSphere[8];
	
		aRotatedTensor[5] *= (root*rp);
		
		// T 3,1
         aRotatedTensor[6] = rot[6]*rot[0]*m_aStressSphere[0] + rot[6]*rot[1]*m_aStressSphere[1] +  rot[6]*rot[2]*m_aStressSphere[2] + 
							 rot[7]*rot[0]*m_aStressSphere[3] + rot[7]*rot[1]*m_aStressSphere[4] +  rot[7]*rot[2]*m_aStressSphere[5] + 
							 rot[8]*rot[0]*m_aStressSphere[6] + rot[8]*rot[1]*m_aStressSphere[7] +  rot[8]*rot[2]*m_aStressSphere[8];		
	
		aRotatedTensor[6] *= root;
		
		// T 3,2
         aRotatedTensor[7] = rot[6]*rot[3]*m_aStressSphere[0] + rot[6]*rot[4]*m_aStressSphere[1] +  rot[6]*rot[5]*m_aStressSphere[2] + 
							 rot[7]*rot[3]*m_aStressSphere[3] + rot[7]*rot[4]*m_aStressSphere[4] +  rot[7]*rot[5]*m_aStressSphere[5] + 
							 rot[8]*rot[3]*m_aStressSphere[6] + rot[8]*rot[4]*m_aStressSphere[7] +  rot[8]*rot[5]*m_aStressSphere[8];				
	
		aRotatedTensor[7] *= (root*rp);
		
		// T 3,3
         aRotatedTensor[8] = rot[6]*rot[6]*m_aStressSphere[0] + rot[6]*rot[7]*m_aStressSphere[1] +  rot[6]*rot[8]*m_aStressSphere[2] + 
							 rot[7]*rot[6]*m_aStressSphere[3] + rot[7]*rot[7]*m_aStressSphere[4] +  rot[7]*rot[8]*m_aStressSphere[5] + 
							 rot[8]*rot[6]*m_aStressSphere[6] + rot[8]*rot[7]*m_aStressSphere[7] +  rot[8]*rot[8]*m_aStressSphere[8];		
	
		aRotatedTensor[8] *= (root*root);
		
		// Now replace the stored tensor with its new components
		
	    for(long i=0; i<9; i++)
		{
		    m_aStressSphere[i] = aRotatedTensor[i];
		}
	}
	else
	{
	    std::cout << " Error in StressGridCell - cell origin is zero" << zEndl;
	    // Centre of cell is exactly at the origin, so the transformation matrix is undefined. We leave the tensor unchanged
	}

#endif
}

// Function to transform the stress tensor from Cartesian coordinates into 
// cylindrical polar coordinates.

void CStressGridCell::TransformStressToCylCoords(double origin[3])
{
#if EnableStressTensorSphere == SimMiscEnabled

#endif
}

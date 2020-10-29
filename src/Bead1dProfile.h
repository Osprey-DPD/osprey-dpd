// Bead1dProfile.h: interface for the CBead1dProfile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BEAD1DPROFILE_H__E6DCE754_07BE_4E34_8BE8_3E9A0A0E2A59__INCLUDED_)
#define AFX_BEAD1DPROFILE_H__E6DCE754_07BE_4E34_8BE8_3E9A0A0E2A59__INCLUDED_


#include "DensityField1d.h"

class CBead1dProfile : public CDensityField1d
{
	// ****************************************
	// Construction/Destruction
public:

	CBead1dProfile(const zString runId, long currentTime, const zString acnType,
					BeadList lBeads, 
					long start, long end,long sample,
					long nx, long ny, long nz, long sliceTotal,
					double lx, double ly, double lz, 
					double lowerCoord, double upperCoord);

	virtual ~CBead1dProfile();

	// ****************************************
	// Global functions, static member functions and variables
public:

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	virtual void Sample();
	virtual void Normalize();

	// ****************************************
	// Public access functions
public:


	// ****************************************
	// Protected local functions
protected:


	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

	BeadList	m_Beads;	// Set of monomers to analysis

};

#endif // !defined(AFX_BEAD1DPROFILE_H__E6DCE754_07BE_4E34_8BE8_3E9A0A0E2A59__INCLUDED_)

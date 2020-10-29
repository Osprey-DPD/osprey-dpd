// aeActiveBond1dProfile.h: interface for the aeActiveBond1dProfile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AEACTIVEBOND1DPROFILE_H__FFC86208_552A_49F7_92D7_83FD82C15C97__INCLUDED_)
#define AFX_AEACTIVEBOND1DPROFILE_H__FFC86208_552A_49F7_92D7_83FD82C15C97__INCLUDED_


#include "DensityField1d.h"

class aeActiveBond1dProfile : public CDensityField1d
{
	// ****************************************
	// Construction/Destruction
public:

	aeActiveBond1dProfile(const zString runId, long currentTime, const zString acnType,
					ActiveBondList lBonds, 
					long start, long end,long sample,
					long nx, long ny, long nz, long sliceTotal,
					double lx, double ly, double lz, 
					double lowerCoord, double upperCoord);

	virtual ~aeActiveBond1dProfile();

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

	ActiveBondList	m_FreeBonds;	// Set of monomers to analysis

};

#endif // !defined(AFX_AEACTIVEBOND1DPROFILE_H__FFC86208_552A_49F7_92D7_83FD82C15C97__INCLUDED_)

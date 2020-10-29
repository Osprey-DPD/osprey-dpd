// LogFreezeBeadsInSlice.h: interface for the CLogFreezeBeadsInSlice class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGFREEZEBEADSINSLICE_H__404C7D69_09EC_4E36_B3A7_1259283A9791__INCLUDED_)
#define AFX_LOGFREEZEBEADSINSLICE_H__404C7D69_09EC_4E36_B3A7_1259283A9791__INCLUDED_


#include "LogConstraintMessage.h"

class CLogFreezeBeadsInSlice : public CLogConstraintMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogFreezeBeadsInSlice(long time, long total, long xn, long yn, long zn, double lower, double thickness);

	virtual ~CLogFreezeBeadsInSlice();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogFreezeBeadsInSlice& rMsg);

	// ****************************************
	// Public access functions
public:

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	virtual	void Serialize(zOutStream& os) const;

	// ****************************************
	// Protected local functions
protected:

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:
	
	// Explicitly disallow the copy constructor and assignment operators
	// by declaring them private and providing NO definitions.

	CLogFreezeBeadsInSlice(const CLogFreezeBeadsInSlice& oldMessage);
	CLogFreezeBeadsInSlice& operator=(const CLogFreezeBeadsInSlice& rhs);

	// ****************************************
	// Data members
private:

	const long	 m_BeadTotal;		// Number of beads frozen in shell
	const long	 m_XN;
	const long	 m_YN;				// Slice normal
	const long	 m_ZN;
	const double m_LowerBound;		// Lower edge of slice (in units of bead diameter)
	const double m_Thickness;		// Thickness of slice			"

};

#endif // !defined(AFX_LOGFREEZEBEADSINSLICE_H__404C7D69_09EC_4E36_B3A7_1259283A9791__INCLUDED_)

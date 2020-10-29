// LogFreezeBeadsInSphericalShell.h: interface for the CLogFreezeBeadsInSphericalShell class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGFREEZEBEADSINSPHERICALSHELL_H__D0E57C86_C9BB_42D2_8AE6_00DECC8E703C__INCLUDED_)
#define AFX_LOGFREEZEBEADSINSPHERICALSHELL_H__D0E57C86_C9BB_42D2_8AE6_00DECC8E703C__INCLUDED_


#include "LogConstraintMessage.h"

class CLogFreezeBeadsInSphericalShell : public CLogConstraintMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogFreezeBeadsInSphericalShell(long time, long total, double innerRadius, double thickness);

	virtual ~CLogFreezeBeadsInSphericalShell();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogFreezeBeadsInSphericalShell& rMsg);

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

	CLogFreezeBeadsInSphericalShell(const CLogFreezeBeadsInSphericalShell& oldMessage);
	CLogFreezeBeadsInSphericalShell& operator=(const CLogFreezeBeadsInSphericalShell& rhs);

	// ****************************************
	// Data members
private:

	const long	 m_BeadTotal;		// Number of beads frozen in shell
	const double m_InnerRadius;		// Inner radius of shell (in units of bead diameter)
	const double m_Thickness;		// Thickness of shell			"

};

#endif // !defined(AFX_LOGFREEZEBEADSINSPHERICALSHELL_H__D0E57C86_C9BB_42D2_8AE6_00DECC8E703C__INCLUDED_)


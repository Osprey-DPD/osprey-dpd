// LogctSelectBeadsInSphericalTargetRegion.h: interface for the CLogctSelectBeadsInSphericalTargetRegion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTSELECTBEADSINSPHERICALTARGETREGION_H__66036889_C6AF_4E3F_9AE3_99FFDB649157__INCLUDED_)
#define AFX_LOGCTSELECTBEADSINSPHERICALTARGETREGION_H__66036889_C6AF_4E3F_9AE3_99FFDB649157__INCLUDED_


#include "LogConstraintMessage.h"

class CLogctSelectBeadsInSphericalTargetRegion : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctSelectBeadsInSphericalTargetRegion(long time, const zString cmdTarget, const zString beadTarget, 
							   const zString beadName, long beadType, long beadTotal,
                               double radius1, double radius2, double theta1, double theta2,
                               double phi1, double phi2);

	virtual ~CLogctSelectBeadsInSphericalTargetRegion();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctSelectBeadsInSphericalTargetRegion& rMsg);

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

	CLogctSelectBeadsInSphericalTargetRegion(const CLogctSelectBeadsInSphericalTargetRegion& oldMessage);
	CLogctSelectBeadsInSphericalTargetRegion& operator=(const CLogctSelectBeadsInSphericalTargetRegion& rhs);


	// ****************************************
	// Data members
private:

	const zString m_CmdTarget;		// Source command target name
	const zString m_BeadTarget;		// New bead target name
	const zString m_BeadName;		// Bead name held in new target
	const long	  m_BeadType;		// Bead type held in new target
	const long	  m_BeadTotal;		// Number of beads in new target
    const double  m_Radius1;        // Radii of region (units of bead diameters)
    const double  m_Radius2;
    const double  m_Theta1;         // Polar angles of region (assumed in radians)
    const double  m_Theta2;
    const double  m_Phi1;           // Asimuthal angles of region (assumed in radians)
    const double  m_Phi2;

};

#endif // !defined(AFX_LOGCTSELECTBEADSINSPHERICALTARGETREGION_H__66036889_C6AF_4E3F_9AE3_99FFDB649157__INCLUDED_)

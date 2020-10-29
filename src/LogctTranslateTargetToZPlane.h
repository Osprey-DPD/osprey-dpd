// LogctTranslateTargetToZPlane.h: interface for the CLogctTranslateTargetToZPlane class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTTRANSLATETARGETTOZPLANE_H__d80889d3_0a86_42ed_9570_8affe2ce2d74__INCLUDED_)
#define AFX_LOGCTTRANSLATETARGETTOZPLANE_H__d80889d3_0a86_42ed_9570_8affe2ce2d74__INCLUDED_


#include "LogConstraintMessage.h"

class CLogctTranslateTargetToZPlane : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctTranslateTargetToZPlane(long time, const zString targetlabel, double zc);

	virtual ~CLogctTranslateTargetToZPlane();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctTranslateTargetToZPlane& rMsg);

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

	CLogctTranslateTargetToZPlane(const CLogctTranslateTargetToZPlane& oldMessage);
	CLogctTranslateTargetToZPlane& operator=(const CLogctTranslateTargetToZPlane& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_TargetLabel;	// Command target label
	const double	m_ZC;			// Fractional distance along the Z dimension of the plane

};

#endif // !defined(AFX_LOGCTTRANSLATETARGETTOZPLANE_H__d80889d3_0a86_42ed_9570_8affe2ce2d74__INCLUDED_)

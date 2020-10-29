// LogctExtractBeadTypesIntoTargets.h: interface for the CLogctExtractBeadTypesIntoTargets class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTEXTRACTBEADTYPESINTOTARGETS_H__06681A7D_CCA6_4F19_A518_5EDC6F72C4C8__INCLUDED_)
#define AFX_LOGCTEXTRACTBEADTYPESINTOTARGETS_H__06681A7D_CCA6_4F19_A518_5EDC6F72C4C8__INCLUDED_


#include "LogConstraintMessage.h"

class CLogctExtractBeadTypesIntoTargets : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctExtractBeadTypesIntoTargets(long time, const zString targetlabel, const zString destlabel,
		                              LongTargetMap mTargets);

	virtual ~CLogctExtractBeadTypesIntoTargets();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctExtractBeadTypesIntoTargets& rMsg);

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

	CLogctExtractBeadTypesIntoTargets(const CLogctExtractBeadTypesIntoTargets& oldMessage);
	CLogctExtractBeadTypesIntoTargets& operator=(const CLogctExtractBeadTypesIntoTargets& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_TargetLabel;	// Source command target label
	const zString	m_DestLabel;	// Destination target root label

	LongTargetMap   m_mTargets;		// Map of (bead type, CCommandTarget) pairs
};

#endif // !defined(AFX_LOGCTEXTRACTBEADTYPESINTOTARGETS_H__06681A7D_CCA6_4F19_A518_5EDC6F72C4C8__INCLUDED_)

// LogctExtractBeadTypesIntoCompositeTarget.h: interface for the CLogctExtractBeadTypesIntoCompositeTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTEXTRACTBEADTYPESINTOCOMPOSITETARGET_H__EACD1B56_E04C_4F30_81D8_0F36B6066A0D__INCLUDED_)
#define AFX_LOGCTEXTRACTBEADTYPESINTOCOMPOSITETARGET_H__EACD1B56_E04C_4F30_81D8_0F36B6066A0D__INCLUDED_


#include "LogConstraintMessage.h"

class CLogctExtractBeadTypesIntoCompositeTarget : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctExtractBeadTypesIntoCompositeTarget(long time, const zString targetLabel, const zString destLabel,
		                              const zString compLabel, long compBeadTotal, LongTargetMap mTargets);

	virtual ~CLogctExtractBeadTypesIntoCompositeTarget();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctExtractBeadTypesIntoCompositeTarget& rMsg);

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

	CLogctExtractBeadTypesIntoCompositeTarget(const CLogctExtractBeadTypesIntoCompositeTarget& oldMessage);
	CLogctExtractBeadTypesIntoCompositeTarget& operator=(const CLogctExtractBeadTypesIntoCompositeTarget& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_TargetLabel;		// Source target label
	const zString	m_DestLabel;		// Destination target root label
	const zString	m_CompLabel;		// Composite target label

	const long      m_CompBeadTotal;	// Number of beads in composite target

	LongTargetMap   m_mTargets;			// Map of (bead type, CCommandTarget) pairs
};

#endif // !defined(AFX_LOGCTEXTRACTBEADTYPESINTOCOMPOSITETARGET_H__EACD1B56_E04C_4F30_81D8_0F36B6066A0D__INCLUDED_)

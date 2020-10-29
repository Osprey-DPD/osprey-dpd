// LogAllBeadsVisible.h: interface for the CLogAllBeadsVisible class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGALLBEADSVISIBLE_H__6AD370B9_9C0B_4B0F_8A08_27D17AD04075__INCLUDED_)
#define AFX_LOGALLBEADSVISIBLE_H__6AD370B9_9C0B_4B0F_8A08_27D17AD04075__INCLUDED_


#include "LogInfoMessage.h" 

class CLogAllBeadsVisible : public CLogInfoMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogAllBeadsVisible(long time, bool bDisplay);

	virtual ~CLogAllBeadsVisible();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogAllBeadsVisible& rMsg);

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

	CLogAllBeadsVisible(const CLogAllBeadsVisible& oldMessage);
	CLogAllBeadsVisible& operator=(const CLogAllBeadsVisible& rhs);

	// ****************************************
	// Data members
private:

	const bool    m_bDisplay;	// Bead display status
};

#endif // !defined(AFX_LOGALLBEADSVISIBLE_H__6AD370B9_9C0B_4B0F_8A08_27D17AD04075__INCLUDED_)

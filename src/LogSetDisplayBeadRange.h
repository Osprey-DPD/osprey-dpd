// LogSetDisplayBeadRange.h: interface for the CLogSetDisplayBeadRange class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGSETDISPLAYBEADRANGE_H__03DE1954_3799_4672_95C6_5E3384846641__INCLUDED_)
#define AFX_LOGSETDISPLAYBEADRANGE_H__03DE1954_3799_4672_95C6_5E3384846641__INCLUDED_


#include "LogInfoMessage.h"

class CLogSetDisplayBeadRange : public CLogInfoMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogSetDisplayBeadRange(long time, const zString axis, double min, double max);

	virtual ~CLogSetDisplayBeadRange();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogSetDisplayBeadRange& rMsg);

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

	CLogSetDisplayBeadRange(const CLogSetDisplayBeadRange& oldMessage);
	CLogSetDisplayBeadRange& operator=(const CLogSetDisplayBeadRange& rhs);


	// ****************************************
	// Data members
private:

	const zString m_Axis;			// Coordinate axis
	const double  m_MinFraction;	// Mimumum bead coordinate displayed
	const double  m_MaxFraction;	// Maximum          "          "
};

#endif // !defined(AFX_LOGSETDISPLAYBEADRANGE_H__03DE1954_3799_4672_95C6_5E3384846641__INCLUDED_)

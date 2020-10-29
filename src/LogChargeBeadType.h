// LogChargeBeadType.h: interface for the CLogChargeBeadType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCHARGEBEADTYPE_H__84666B6F_E69A_479B_A295_ADAE91940F43__INCLUDED_)
#define AFX_LOGCHARGEBEADTYPE_H__84666B6F_E69A_479B_A295_ADAE91940F43__INCLUDED_


#include "LogConstraintMessage.h"

class CLogChargeBeadType : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogChargeBeadType(long time, const zString name, 
					   long type, double strength, double range, bool bCharged);

	virtual ~CLogChargeBeadType();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogChargeBeadType& rMsg);

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

	CLogChargeBeadType(const CLogChargeBeadType& oldMessage);
	CLogChargeBeadType& operator=(const CLogChargeBeadType& rhs);

	// ****************************************
	// Data members
private:

	const zString m_Name;				// Bead name, e.g., H
	const long    m_Type;				// Bead type, e.g., 0
	const double  m_Strength;			// Screened charge force strength
	const double  m_Range;				//    "        "     "   range
	bool		  m_bCharged;			// Flag showing whether charge force is on or off
};

#endif // !defined(AFX_LOGCHARGEBEADTYPE_H__84666B6F_E69A_479B_A295_ADAE91940F43__INCLUDED_)

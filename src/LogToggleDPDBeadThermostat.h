// LogToggleDPDBeadThermostat.h: interface for the CLogToggleDPDBeadThermostat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGTOGGLEDPDBEADTHERMOSTAT_H__000228BC_B167_4C97_A27E_D8009CF2C3F8__INCLUDED_)
#define AFX_LOGTOGGLEDPDBEADTHERMOSTAT_H__000228BC_B167_4C97_A27E_D8009CF2C3F8__INCLUDED_


#include "LogConstraintMessage.h"

class CLogToggleDPDBeadThermostat : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogToggleDPDBeadThermostat(long time, bool bZeroed);

	virtual ~CLogToggleDPDBeadThermostat();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogToggleDPDBeadThermostat& rMsg);

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

	CLogToggleDPDBeadThermostat(const CLogToggleDPDBeadThermostat& oldMessage);
	CLogToggleDPDBeadThermostat& operator=(const CLogToggleDPDBeadThermostat& rhs);


	// ****************************************
	// Data members
private:

	const bool m_bZeroed;	// Flag showing whether DPD forces are zeroed or not

};
#endif // !defined(AFX_LOGTOGGLEDPDBEADTHERMOSTAT_H__000228BC_B167_4C97_A27E_D8009CF2C3F8__INCLUDED_)

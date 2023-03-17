// LogBeadDisplayId.h: interface for the CLogSetSAXSProcessBeadElectronNo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGSETSAXSPROCESSBEADELECTRONNO_H__4c250ab8_48a7_4782_9ffb_a0c4f9371167__INCLUDED_)
#define AFX_LOGSETSAXSPROCESSBEADELECTRONNO_H__4c250ab8_48a7_4782_9ffb_a0c4f9371167__INCLUDED_


#include "LogInfoMessage.h" 

class CLogSetSAXSProcessBeadElectronNo : public CLogInfoMessage
{
	// ****************************************
	// Construction/Destruction
public:

	CLogSetSAXSProcessBeadElectronNo(long time, const zString procName, const zString beadName, long type, long total, double eno);

	virtual ~CLogSetSAXSProcessBeadElectronNo();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogSetSAXSProcessBeadElectronNo& rMsg);

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

	CLogSetSAXSProcessBeadElectronNo(const CLogSetSAXSProcessBeadElectronNo& oldMessage);
	CLogSetSAXSProcessBeadElectronNo& operator=(const CLogSetSAXSProcessBeadElectronNo& rhs);

	// ****************************************
	// Data members
private:

    const zString m_ProcName;   // Name of the SAXS processbeing modifed
    const zString m_BeadName;   // Name of bead whose electon number is set
    const long    m_Type;       // Numeric identifier for bead type
    const long    m_Total;      // Number of beads of the given type
	const double  m_Eno;	    // Number of electrons in the bead
};

#endif // !defined(AFX_LOGSETSAXSPROCESSBEADELECTRONNO_H__4c250ab8_48a7_4782_9ffb_a0c4f9371167__INCLUDED_)

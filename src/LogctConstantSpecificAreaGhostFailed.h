// LogctConstantSpecificAreaGhostFailed.h: interface for the CLogctConstantSpecificAreaGhostFailed class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTCONSTANTSPECIFICAREAGHOSTFAILED_H__a71c9191_3fa8_40ca_91a5_5ac94019d06d__INCLUDED_)
#define AFX_LOGCTCONSTANTSPECIFICAREAGHOSTFAILED_H__a71c9191_3fa8_40ca_91a5_5ac94019d06d__INCLUDED_
  

#include "LogErrorMessage.h"

class CLogctConstantSpecificAreaGhostFailed : public CLogErrorMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctConstantSpecificAreaGhostFailed(long time, const zString decLabel, const zString bulkTargetName, const zString membraneTargetName,
                                   const long bulkTotal, const long membraneTotal);
								   
	virtual ~CLogctConstantSpecificAreaGhostFailed();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctConstantSpecificAreaGhostFailed& rMsg);

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

	CLogctConstantSpecificAreaGhostFailed(const CLogctConstantSpecificAreaGhostFailed& oldMessage);
	CLogctConstantSpecificAreaGhostFailed& operator=(const CLogctConstantSpecificAreaGhostFailed& rhs);


	// ****************************************
	// Data members
private:

    const zString  m_DecLabel;                // Decorator label for the constant membrane area algorithm
    const zString  m_BulkTargetName;          // Bulk ghost reservoir target label
    const zString  m_MembraneTargetName;      // Membrane target label
    
    const long     m_BulkReservoirTotal;      // Number of ghosts currently in the bulk reservoir
    const long     m_MembraneReservoirTotal;  // Number of ghosts currently in the membrane reservoir
};

#endif // !defined(AFX_LOGCTCONSTANTSPECIFICAREAGHOSTFAILED_H__a71c9191_3fa8_40ca_91a5_5ac94019d06d__INCLUDED_)

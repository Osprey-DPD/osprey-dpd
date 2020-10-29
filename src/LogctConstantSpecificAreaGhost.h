// LogctConstantSpecificAreaGhost.h: interface for the CLogctConstantSpecificAreaGhost class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTCONSTANTSPECIFICAREAGHOST_H__ffd9fdb5_dc9d_49b2_9da5_3babd58f41b8__INCLUDED_)
#define AFX_LOGCTCONSTANTSPECIFICAREAGHOST_H__ffd9fdb5_dc9d_49b2_9da5_3babd58f41b8__INCLUDED_
  

#include "LogConstraintMessage.h"

class CLogctConstantSpecificAreaGhost : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctConstantSpecificAreaGhost(long time, const zString membraneTarget, const zString reservoirTarget, const zString decLabel,
                                    const zString beadName, const long sampleRate, const double targetAN);

	virtual ~CLogctConstantSpecificAreaGhost();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctConstantSpecificAreaGhost& rMsg);

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

	CLogctConstantSpecificAreaGhost(const CLogctConstantSpecificAreaGhost& oldMessage);
	CLogctConstantSpecificAreaGhost& operator=(const CLogctConstantSpecificAreaGhost& rhs);


	// ****************************************
	// Data members
private:

    const zString  m_MembraneTargetName;   // Membrane amphiphile target label
    const zString  m_ReservoirTargetName;  // Bulk ghost reservoir target label
    const zString  m_DecLabel;             // Decorator label for the constant membrane area algorithm
    const zString  m_BeadName;             // Bead type to use for triangulating the membrane surface area
    
    const long     m_SampleRate;           // Number of time steps between invocations of the decorator
    const double   m_TargetAN;             // Area per molecule to maintain for the membrane
    
};

#endif // !defined(AFX_LOGCTCONSTANTSPECIFICAREAGHOST_H__ffd9fdb5_dc9d_49b2_9da5_3babd58f41b8__INCLUDED_)

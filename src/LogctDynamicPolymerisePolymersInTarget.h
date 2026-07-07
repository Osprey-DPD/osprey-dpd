// LogctDynamicPolymerisePolymersInTarget.h: interface for the CLogctDynamicPolymerisePolymers class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTDYNAMICPOLYMERISEPOLYMERSINTARGET_H__ce8e3803_0fda_4049_a92b_823afe64e1e8__INCLUDED_)
#define AFX_LOGCTDYNAMICPOLYMERISEPOLYMERSINTARGET_H__ce8e3803_0fda_4049_a92b_823afe64e1e8__INCLUDED_


#include "LogConstraintMessage.h"

class CLogctDynamicPolymerisePolymersInTarget : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctDynamicPolymerisePolymersInTarget(long time, const zString targetlabel, const zString decLabel, 
                                            const zString polyBondName, long polyType, zLongVector vBeadTypes,
                                            long maxBonds, double maxDist, double sprConst, double unStrLength,
                                            double bondPrefactor, double bondBarrier);

	virtual ~CLogctDynamicPolymerisePolymersInTarget();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctDynamicPolymerisePolymersInTarget& rMsg);

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

	CLogctDynamicPolymerisePolymersInTarget(const CLogctDynamicPolymerisePolymersInTarget& oldMessage);
	CLogctDynamicPolymerisePolymersInTarget& operator=(const CLogctDynamicPolymerisePolymersInTarget& rhs);


	// ****************************************
	// Data members
private:

    const zString   m_TargetLabel;	  // Command target label
    const zString   m_DecLabel;	  // Command wrapper label 
    const zString   m_PolyBondName;     // String identifier for the dynamically-created bond type
    long            m_PolymerType;      // Numeric identifier for polymers to dynamically bond together
    zLongVector     m_vBeadTypes;       // Numeric identifiers of polymerisable bead types
    long            m_MaxBonds;         // Max number of bonds per polymer: 1 = dimer, 2 = trimer, etc.
    double          m_MaxDistance;      // Range within which polymers will be tied together
    double          m_sprConst;         // Spring constant of harmonic potential tying polymers together
    double          m_unStrLength;      // Unstretched length of harmonic potential
    double          m_BondPrefactor;    // Prefactor for the bond formation Arrhenius function
    double          m_BondBarrier;      // Energy barrier for the bond formation Arrhenius function
};

#endif // !defined(AFX_LOGCTDYNAMICPOLYMERISEPOLYMERSINTARGET_H__ce8e3803_0fda_4049_a92b_823afe64e1e8__INCLUDED_)

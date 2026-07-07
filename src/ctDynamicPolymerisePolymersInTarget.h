// ctDynamicPolymerisePolymersInTarget.h: interface for the ctDynamicPolymerisePolymersInTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTDYNAMICPOLYMERISEPOLYMERSINTARGET_H__c7af9312_24e2_4a64_b2aa_aedcba3a64d6__INCLUDED_)
#define AFX_CTDYNAMICPOLYMERISEPOLYMERSINTARGET_H__c7af9312_24e2_4a64_b2aa_aedcba3a64d6__INCLUDED_


#include "ctApplyCommand.h"

class ctDynamicPolymerisePolymersInTarget : public ctApplyCommand
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

    ctDynamicPolymerisePolymersInTarget(long executionTime);
    ctDynamicPolymerisePolymersInTarget(const ctDynamicPolymerisePolymersInTarget& oldCommand);

    ctDynamicPolymerisePolymersInTarget(long executionTime, const zString targetLabel, const zString decLabel,
                                long polyType, long beadTypeTotal, zLongVector vBeadTypes, long maxBonds, 
                                double maxDist, double sprConst, double unStrLength, double bondPrefactor, double bondBarrier);

	virtual ~ctDynamicPolymerisePolymersInTarget();
	
	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	 // Return the type of command
        static long  GetArgumentTotal(); // Return the number of arguments 

private:

	static const zString m_Type;	// Identifier used in control data file for command
        static long  m_ArgumentTotal;   // Number of arguments required not including the execution time

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	zOutStream& put(zOutStream& os) const;
	zInStream&  get(zInStream& is);

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an xxCommand pointer,
	// respond to the SimBox's request to execute and return the name of the command.

	virtual bool Execute(long simTime, ISimCmd* const pISimCmd) const;

	virtual const xxCommand* GetCommand() const;

	virtual bool IsDataValid(const CInputData& riData) const;

        virtual long GetCommandArgumentTotal() const {return m_ArgumentTotal;}


	// ****************************************
	// Public access functions
public:

        inline const zString   GetDecLabel()      const {return m_DecLabel;}
	inline long        GetPolymerType()	    const {return m_PolymerType;}
	inline zLongVector GetBeadTypes()	    const {return m_vBeadTypes;}
	inline long        GetMaxBonds()	    const {return m_MaxBonds;}
	inline double      GetMaxDistance()        const {return m_MaxDistance;}
	inline double      GetSpringConstant()     const {return m_sprConst;}
	inline double      GetUnStretchedLength()  const {return m_unStrLength;}
	inline double      GetBondPrefactor()      const {return m_BondPrefactor;}
	inline double      GetBondBarrier()        const {return m_BondBarrier;}
    
	// ****************************************
	// Protected local functions
protected:

	virtual const zString GetCommandType() const;

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

        zString     m_DecLabel;         // Label for the decorator object
        long        m_PolymerType;      // Numeric identifier for polymers to dynamically bond together
        long        m_BeadTypeTotal;    // Number of polymerisable bead types in the polymer
        zLongVector m_vBeadTypes;	 // Container of polymerisable bead types in polymers (not dynamically created)
        long        m_MaxBonds;         // Max number of polymers per cluster: 2 = dimer, 3 = trimer, etc.
        double      m_MaxDistance;      // Range within which polymers will be tied together
        double      m_sprConst;         // Spring constant of harmonic potential tying polymers together
        double      m_unStrLength;      // Unstretched length of harmonic potential
        double      m_BondPrefactor;    // Bare probability of bond formation
        double      m_BondBarrier;      // Energy barrier in the Arrhenius function for bond formation
};

#endif // !defined(AFX_CTDYNAMICPOLYMERISEPOLYMERSINTARGET_H__c7af9312_24e2_4a64_b2aa_aedcba3a64d6__INCLUDED_)

// ctSetBondStiffnessInTarget.h: interface for the ctSetBondStiffnessInTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTSETBONDSTIFFNESSINTARGET_H__F157D819_4F14_4FFA_B6F2_9ED3BCD5164F__INCLUDED_)
#define AFX_CTSETBONDSTIFFNESSINTARGET_H__F157D819_4F14_4FFA_B6F2_9ED3BCD5164F__INCLUDED_


#include "ctApplyCommand.h"

class ctSetBondStiffnessInTarget : public ctApplyCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ctSetBondStiffnessInTarget(long executionTime);
	ctSetBondStiffnessInTarget(const ctSetBondStiffnessInTarget& oldCommand);

	ctSetBondStiffnessInTarget(long executionTime, const zString target, const zString name,
                               double modulus, double phi0);

	virtual ~ctSetBondStiffnessInTarget();
	
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

    virtual bool Pack(const tguArgumentSequence& vArguments);

	// ****************************************
	// Public access functions
public:

    inline const zString  GetName()           const {return m_Name;};
    inline         double GetBendingModulus() const {return m_BendingModulus;}
    inline         double GetPhi0()           const {return m_Phi0;}

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

    zString m_Name;                 // Name of corresponding CBondPair object
    double m_BendingModulus;        // New value of bending modulus
    double m_Phi0;                  // New preferred angle for bond pair

};

#endif // !defined(AFX_CTSETBONDSTIFFNESSINTARGET_H__F157D819_4F14_4FFA_B6F2_9ED3BCD5164F__INCLUDED_)

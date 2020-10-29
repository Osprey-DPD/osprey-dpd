// ctSetBondStrengthInTarget.h: interface for the ctSetBondStrengthInTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTSETBONDSTRENGTHINTARGET_H__4F6602C1_046E_4C90_A446_06A6D8D8098A__INCLUDED_)
#define AFX_CTSETBONDSTRENGTHINTARGET_H__4F6602C1_046E_4C90_A446_06A6D8D8098A__INCLUDED_


#include "ctApplyCommand.h"

class ctSetBondStrengthInTarget : public ctApplyCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ctSetBondStrengthInTarget(long executionTime);
	ctSetBondStrengthInTarget(const ctSetBondStrengthInTarget& oldCommand);

	ctSetBondStrengthInTarget(long executionTime, const zString target, const zString name, 
					  double spring, double length);

	virtual ~ctSetBondStrengthInTarget();
	
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

	inline const zString GetName()				const {return m_Name;};
	inline		 double	 GetSpringConstant()	const {return m_SpringConstant;}
	inline		 double  GetUnstretchedLength()	const {return m_UnstretchedLength;}


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

	zString m_Name;					// Name of corresponding CBond object
	double  m_SpringConstant;		// New value of spring constant
	double  m_UnstretchedLength;	// New unstretched length

};

#endif // !defined(AFX_CTSETBONDSTRENGTHINTARGET_H__4F6602C1_046E_4C90_A446_06A6D8D8098A__INCLUDED_)

// ccSelectACNCurrentBondsAsTarget.h: interface for the ccSelectACNCurrentBondsAsTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSELECTACNCURRENTBONDSASTARGET_H__91F66F92_BB5E_4144_99A6_5C2142A41D2B__INCLUDED_)
#define AFX_CCSELECTACNCURRENTBONDSASTARGET_H__91F66F92_BB5E_4144_99A6_5C2142A41D2B__INCLUDED_


#include "xxCommand.h"

class ccSelectACNCurrentBondsAsTarget : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ccSelectACNCurrentBondsAsTarget(long executionTime);
	ccSelectACNCurrentBondsAsTarget(const ccSelectACNCurrentBondsAsTarget& oldCommand);

	virtual ~ccSelectACNCurrentBondsAsTarget();
	
	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	// Return the type of command

	
private:

	static const zString m_Type;	// Identifier used in control data file for command


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

	// ****************************************
	// Public access functions
public:

	inline const zString GetTargetLabel() const {return m_TargetLabel;}
	inline const zString GetACNType()     const {return m_ACNType;}

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

	zString	m_TargetLabel;	// Command target's name
	zString m_ACNType;		// Name of ACN to wrap in the target

};

#endif // !defined(AFX_CCSELECTACNCURRENTBONDSASTARGET_H__91F66F92_BB5E_4144_99A6_5C2142A41D2B__INCLUDED_)

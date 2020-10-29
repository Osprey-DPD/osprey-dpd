// ccSelectACNFreePolymersAsTarget.h: interface for the ccSelectACNFreePolymersAsTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSELECTACNFREEPOLYMERSASTARGET_H__35EA4C33_2E3D_461F_941B_69F263B78F62__INCLUDED_)
#define AFX_CCSELECTACNFREEPOLYMERSASTARGET_H__35EA4C33_2E3D_461F_941B_69F263B78F62__INCLUDED_


#include "xxCommand.h"

class ccSelectACNFreePolymersAsTarget : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ccSelectACNFreePolymersAsTarget(long executionTime);
	ccSelectACNFreePolymersAsTarget(const ccSelectACNFreePolymersAsTarget& oldCommand);

	virtual ~ccSelectACNFreePolymersAsTarget();
	
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

#endif // !defined(AFX_CCSELECTACNFREEPOLYMERSASTARGET_H__35EA4C33_2E3D_461F_941B_69F263B78F62__INCLUDED_)

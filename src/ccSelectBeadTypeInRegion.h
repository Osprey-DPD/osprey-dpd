// ccSelectBeadTypeInRegion.h: interface for the ccSelectBeadTypeInRegion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSELECTBEADTYPEINREGION_H__68C6CA0B_26AC_43E4_BB9E_D71447F24996__INCLUDED_)
#define AFX_CCSELECTBEADTYPEINREGION_H__68C6CA0B_26AC_43E4_BB9E_D71447F24996__INCLUDED_


#include "xxCommand.h"

class ccSelectBeadTypeInRegion : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: protected constructor declared below
public:

	virtual ~ccSelectBeadTypeInRegion();
	
	// ****************************************
	// Global functions, static member functions and variables
public:

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	zOutStream& put(zOutStream& os) const;
	zInStream&  get(zInStream& is);

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an xxCommand pointer,
	// respond to the SimBox's request to execute and return the name of the command.

	virtual bool Execute(long simTime, ISimCmd* const pISimCmd) const = 0;

	virtual const zString GetCommandType() const = 0;

	virtual const xxCommand* GetCommand()  const = 0;

	// The following function is not a PVF in this class as it is used by 
	// derived classes to check their common data stored here

	virtual bool IsDataValid(const CInputData& riData) const;

    virtual bool Pack(const tguArgumentSequence& vArguments);

	// ****************************************
	// Public access functions
public:

	inline const zString GetTargetLabel() const {return m_TargetLabel;}
	inline const zString GetBeadName()   const {return m_BeadName;}

	// ****************************************
	// Protected local functions
protected:

	// Protected constructor ensures that only subclasses can be instantiated

	ccSelectBeadTypeInRegion(long executionTime);
	ccSelectBeadTypeInRegion(const ccSelectBeadTypeInRegion& oldCommand);

	ccSelectBeadTypeInRegion(long executionTime, const zString target, const zString bead);

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

	zString		m_TargetLabel;	// Command target's name
	zString		m_BeadName; 	// Bead name

};

#endif // !defined(AFX_CCSELECTBEADTYPEINREGION_H__68C6CA0B_26AC_43E4_BB9E_D71447F24996__INCLUDED_)

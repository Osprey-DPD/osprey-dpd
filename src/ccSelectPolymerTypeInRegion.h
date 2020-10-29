// ccSelectPolymerTypeInRegion.h: interface for the ccSelectPolymerTypeInRegion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSELECTPOLYMERTYPEINREGION_H__1C7A64A5_2ABE_4FD5_BB22_177B078823CB__INCLUDED_)
#define AFX_CCSELECTPOLYMERTYPEINREGION_H__1C7A64A5_2ABE_4FD5_BB22_177B078823CB__INCLUDED_


#include "xxCommand.h"

class ccSelectPolymerTypeInRegion : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: protected constructor declared below
public:

	virtual ~ccSelectPolymerTypeInRegion();
	
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
	inline const zString GetPolymerName() const {return m_PolymerName;}

	// ****************************************
	// Protected local functions
protected:

	// Protected constructor ensures that only subclasses can be instantiated

	ccSelectPolymerTypeInRegion(long executionTime);
	ccSelectPolymerTypeInRegion(const ccSelectPolymerTypeInRegion& oldCommand);

	ccSelectPolymerTypeInRegion(long executionTime, const zString target, const zString poly);

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

	zString		m_TargetLabel;	// Command target's name
	zString		m_PolymerName; 	// Polymer name

};

#endif // !defined(AFX_CCSELECTPOLYMERTYPEINREGION_H__1C7A64A5_2ABE_4FD5_BB22_177B078823CB__INCLUDED_)

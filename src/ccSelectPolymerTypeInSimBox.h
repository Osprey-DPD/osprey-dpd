// ccSelectPolymerTypeInSimBox.h: interface for the ccSelectPolymerTypeInSimBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSELECTPOLYMERTYPEINSIMBOX_H__0B173395_00CA_486E_971D_4693D1B4FF16__INCLUDED_)
#define AFX_CCSELECTPOLYMERTYPEINSIMBOX_H__0B173395_00CA_486E_971D_4693D1B4FF16__INCLUDED_


#include "ccSelectPolymerTypeInRegion.h"

class ccSelectPolymerTypeInSimBox : public ccSelectPolymerTypeInRegion  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ccSelectPolymerTypeInSimBox(long executionTime);
	ccSelectPolymerTypeInSimBox(long executionTime, const zString target, const zString name);

    ccSelectPolymerTypeInSimBox(const ccSelectPolymerTypeInSimBox& oldCommand);

	virtual ~ccSelectPolymerTypeInSimBox();
	
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

	virtual bool Pack(const tguArgumentSequence& vArguments);

	// ****************************************
	// Public access functions
public:


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


};

#endif // !defined(AFX_CCSELECTPOLYMERTYPEINSIMBOX_H__0B173395_00CA_486E_971D_4693D1B4FF16__INCLUDED_)

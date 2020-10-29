// ccSelectBeadTypeInSimBox.h: interface for the ccSelectBeadTypeInSimBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSELECTBEADTYPEINSIMBOX_H__397BD6EB_5271_4421_BF2B_B27070AF1510__INCLUDED_)
#define AFX_CCSELECTBEADTYPEINSIMBOX_H__397BD6EB_5271_4421_BF2B_B27070AF1510__INCLUDED_


#include "ccSelectBeadTypeInRegion.h"

class ccSelectBeadTypeInSimBox : public ccSelectBeadTypeInRegion  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ccSelectBeadTypeInSimBox(long executionTime);
	ccSelectBeadTypeInSimBox(const ccSelectBeadTypeInSimBox& oldCommand);

	virtual ~ccSelectBeadTypeInSimBox();
	
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

	// This command has no specific arguments

};

#endif // !defined(AFX_CCSELECTBEADTYPEINSIMBOX_H__397BD6EB_5271_4421_BF2B_B27070AF1510__INCLUDED_)

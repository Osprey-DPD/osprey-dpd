// ctRestoreOriginalBeadType.h: interface for the ctRestoreOriginalBeadType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTRESTOREORIGINALBEADTYPE_H__8EF23569_05B6_446B_87E1_1874B9AAF6E2__INCLUDED_)
#define AFX_CTRESTOREORIGINALBEADTYPE_H__8EF23569_05B6_446B_87E1_1874B9AAF6E2__INCLUDED_


#include "ctApplyCommand.h"

class ctRestoreOriginalBeadType : public ctApplyCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ctRestoreOriginalBeadType(long executionTime);
	ctRestoreOriginalBeadType(const ctRestoreOriginalBeadType& oldCommand);

	virtual ~ctRestoreOriginalBeadType();
	
	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	// Return the type of command
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

#endif // !defined(AFX_CTRESTOREORIGINALBEADTYPE_H__8EF23569_05B6_446B_87E1_1874B9AAF6E2__INCLUDED_)

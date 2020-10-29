// ctChangeBeadType.h: interface for the ctChangeBeadType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTCHANGEBEADTYPE_H__1CB9FE51_D7A5_4991_AB2A_C5FFCD61D20C__INCLUDED_)
#define AFX_CTCHANGEBEADTYPE_H__1CB9FE51_D7A5_4991_AB2A_C5FFCD61D20C__INCLUDED_


#include "ctApplyCommand.h"

class ctChangeBeadType : public ctApplyCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ctChangeBeadType(long executionTime);
	ctChangeBeadType(const ctChangeBeadType& oldCommand);

	ctChangeBeadType(long executionTime, const zString target);

	virtual ~ctChangeBeadType();
	
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

#endif // !defined(AFX_CTCHANGEBEADTYPE_H__1CB9FE51_D7A5_4991_AB2A_C5FFCD61D20C__INCLUDED_)

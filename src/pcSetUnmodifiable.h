// pcSetUnmodifiable.h: interface for the pcSetUnmodifiable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PCSETUNMODIFIABLE_H__E94113B5_6093_4FEE_BFF9_0282CF5D3FB1__INCLUDED_)
#define AFX_PCSETUNMODIFIABLE_H__E94113B5_6093_4FEE_BFF9_0282CF5D3FB1__INCLUDED_


#include "xxCommand.h"

class pcSetUnmodifiable : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	pcSetUnmodifiable(long executionTime);
	pcSetUnmodifiable(const pcSetUnmodifiable& oldCommand);

	virtual ~pcSetUnmodifiable();
	
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

	inline const zString GetPid()   const {return m_pid;}

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

	zString	 m_pid;		// Process id that uniquely identifies each process

};

#endif // !defined(AFX_PCSETUNMODIFIABLE_H__E94113B5_6093_4FEE_BFF9_0282CF5D3FB1__INCLUDED_)

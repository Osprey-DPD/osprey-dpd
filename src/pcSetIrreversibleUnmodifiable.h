// pcSetIrreversibleUnmodifiable.h: interface for the pcSetIrreversibleUnmodifiable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PCSETIRREVERSIBLEUNMODIFIABLE_H__0E7CBF3A_2297_4E83_8B95_4CB3D26C890D__INCLUDED_)
#define AFX_PCSETIRREVERSIBLEUNMODIFIABLE_H__0E7CBF3A_2297_4E83_8B95_4CB3D26C890D__INCLUDED_


#include "xxCommand.h"

class pcSetIrreversibleUnmodifiable : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	pcSetIrreversibleUnmodifiable(long executionTime);
	pcSetIrreversibleUnmodifiable(const pcSetIrreversibleUnmodifiable& oldCommand);

	virtual ~pcSetIrreversibleUnmodifiable();
	
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

#endif // !defined(AFX_PCSETIRREVERSIBLEUNMODIFIABLE_H__0E7CBF3A_2297_4E83_8B95_4CB3D26C890D__INCLUDED_)

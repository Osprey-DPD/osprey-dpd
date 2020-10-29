// pcSetModifiable.h: interface for the pcSetModifiable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PCSETMODIFIABLE_H__DFD6A349_67A8_474A_9DED_B6FE6125E011__INCLUDED_)
#define AFX_PCSETMODIFIABLE_H__DFD6A349_67A8_474A_9DED_B6FE6125E011__INCLUDED_


#include "xxCommand.h"

class pcSetModifiable : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	pcSetModifiable(long executionTime);
	pcSetModifiable(const pcSetModifiable& oldCommand);

	virtual ~pcSetModifiable();
	
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


#endif // !defined(AFX_PCSETMODIFIABLE_H__DFD6A349_67A8_474A_9DED_B6FE6125E011__INCLUDED_)

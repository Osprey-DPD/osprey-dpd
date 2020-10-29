// ctWorkDoneOnBeadTypeInTarget.h: interface for the ctWorkDoneOnBeadTypeInTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTWORKDONEONBEADTYPEINTARGET_H__489F5EC1_2744_4ED1_91B9_EAB6DAFE6903__INCLUDED_)
#define AFX_CTWORKDONEONBEADTYPEINTARGET_H__489F5EC1_2744_4ED1_91B9_EAB6DAFE6903__INCLUDED_


#include "ctApplyCommand.h"

class ctWorkDoneOnBeadTypeInTarget : public ctApplyCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ctWorkDoneOnBeadTypeInTarget(long executionTime);
	ctWorkDoneOnBeadTypeInTarget(const ctWorkDoneOnBeadTypeInTarget& oldCommand);

	ctWorkDoneOnBeadTypeInTarget(long executionTime, const zString target);

	virtual ~ctWorkDoneOnBeadTypeInTarget();
	
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


	// ****************************************
	// Public access functions
public:

	inline long GetBeadType() const {return m_BeadType;}


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

	long m_BeadType; // Type of beads whose F.dx is to be calculated

};

#endif // !defined(AFX_CTWORKDONEONBEADTYPEINTARGET_H__489F5EC1_2744_4ED1_91B9_EAB6DAFE6903__INCLUDED_)

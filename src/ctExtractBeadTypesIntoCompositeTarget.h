// ctExtractBeadTypesIntoCompositeTarget.h: interface for the ctExtractBeadTypesIntoCompositeTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTEXTRACTBEADTYPESINTOCOMPOSITETARGET_H__714F6A4A_69A3_4828_9EB0_8E1DE186128D__INCLUDED_)
#define AFX_CTEXTRACTBEADTYPESINTOCOMPOSITETARGET_H__714F6A4A_69A3_4828_9EB0_8E1DE186128D__INCLUDED_


#include "ctApplyCommand.h"

class ctExtractBeadTypesIntoCompositeTarget : public ctApplyCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ctExtractBeadTypesIntoCompositeTarget(long executionTime);
	ctExtractBeadTypesIntoCompositeTarget(const ctExtractBeadTypesIntoCompositeTarget& oldCommand);

	ctExtractBeadTypesIntoCompositeTarget(long executionTime, const zString target, const zString destLabel);

	virtual ~ctExtractBeadTypesIntoCompositeTarget();
	
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

	const zString GetDestinationLabel() const {return m_DestLabel;}


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

	zString  m_DestLabel;     // Name of composite target and root name of destination targets

};

#endif // !defined(AFX_CTEXTRACTBEADTYPESINTOCOMPOSITETARGET_H__714F6A4A_69A3_4828_9EB0_8E1DE186128D__INCLUDED_)

// prCommandContainer.h: interface for the prCommandContainer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRCOMMANDCONTAINER_H__F80CAA9B_E14D_4C7C_80C0_B4D0CB0975D5__INCLUDED_)
#define AFX_PRCOMMANDCONTAINER_H__F80CAA9B_E14D_4C7C_80C0_B4D0CB0975D5__INCLUDED_


#include "xxProcess.h"

class prCommandContainer : public xxProcess
{
	// ****************************************
	// Construction/Destruction
public:

	prCommandContainer();
	virtual ~prCommandContainer();

	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	// Return the process name

private:

	static const zString m_Type;	// Identifier used in control data file

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an xxProcess pointer,
	// and respond to the SimBox's request to sample their data.

	virtual zOutStream& put(zOutStream& os) const;
	virtual zInStream&  get(zInStream& is);

	// Function to allow the process to monitor aggregates and check for events

	virtual void UpdateState(CSimState& rSimState, const ISimBox* const pISimBox);

	virtual xxProcess* GetProcess()  const;

	// Non-static member function to return the (static) process name that
	// must be provided by each derived class. A static GetType() function
	// must also be provided.

	virtual const zString GetProcessType() const;

	// Function to allow the CInputData object to check all processes' data

	virtual bool ValidateData(const CInputData &riData);

	// Function to allow process validation when it is created internally

	virtual bool InternalValidateData(const ISimState* const pISimState);

	// ****************************************
	// Public access functions
public:

	inline long GetCommandTotal() const {return m_Commands.size();}

	bool AddCommand(const xxCommand* const pCommand);


	// ****************************************
	// Protected local functions
protected:

	// Functions to allow the xxProcess to store pointers to its 
	// required CAnalysis and xxEvents-derived class(es)
	// rather than doing dynamic_cast every time we want to access them.
	// We cannot provide such a function in the xxProcess base class
	// because different processes may need different numbers of aggregates
	// and events.


	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:	

	// ****************************************
	// Data members

private:							// Data specific to the process

	CommandSequence  m_Commands;	// Set of command held in container

};

#endif // !defined(AFX_PRCOMMANDCONTAINER_H__F80CAA9B_E14D_4C7C_80C0_B4D0CB0975D5__INCLUDED_)

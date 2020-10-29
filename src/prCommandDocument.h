// prCommandDocument.h: interface for the prCommandDocument class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRCOMMANDDOCUMENT_H__16FD95C4_71E6_4469_B653_E2E36EAE1B26__INCLUDED_)
#define AFX_PRCOMMANDDOCUMENT_H__16FD95C4_71E6_4469_B653_E2E36EAE1B26__INCLUDED_


#include "IModifyCommandDocument.h"

class prCommandDocument : public IModifyCommandDocument
{
	// ****************************************
	// Construction/Destruction
public:

	prCommandDocument();
	virtual ~prCommandDocument();

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
	// Implementation of the IModifyProcess interface

	bool IsModifiable() const;
	void SetModifiable(const xxCommand* const pCommand);
	void SetUnmodifiable(const xxCommand* const pCommand);
	void SetIrreversibleUnmodifiable(const xxCommand* const pCommand);

// ****************************************
	// Implementation of the IModifyCommandDocument interface

	virtual void GetPendingDocumentTotal(const xxCommand* const pCommand) const;

	virtual void GetPendingDocuments(const xxCommand* const pCommand) const;

	virtual void AddDocument(const xxCommand* const pCommand);


	// ****************************************
	// Public access functions
public:


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

private:						// Data specific to the process

	// Data required to implement the IModifyProcess interface 

	bool m_bIsModifiable;		// Flag showing if the process is currently modifiable
	bool m_bNeverModifiable;	// Flag showing if the process is not modifiable

	// Local data for the process

	StringSequence	m_Documents;		// Names of pending command documents (each including the ".xml" suffix)

	zString			m_DocumentName;		// Local copy of the next command document's name 
};

#endif // !defined(AFX_PRCOMMANDDOCUMENT_H__16FD95C4_71E6_4469_B653_E2E36EAE1B26__INCLUDED_)

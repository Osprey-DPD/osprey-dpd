// ccStartCommandDocumentProcess.h: interface for the ccStartCommandDocumentProcess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSTARTCOMMANDDOCUMENTPROCESS_H__62BB011F_9902_4FA9_9287_E30F22A51C1A__INCLUDED_)
#define AFX_CCSTARTCOMMANDDOCUMENTPROCESS_H__62BB011F_9902_4FA9_9287_E30F22A51C1A__INCLUDED_


#include "xxCommand.h"

class ccStartCommandDocumentProcess : public xxCommand  
{
	// ****************************************
	// Construction/Destruction
public:

	ccStartCommandDocumentProcess(long executionTime);
	ccStartCommandDocumentProcess(const ccStartCommandDocumentProcess& oldCommand);

	virtual ~ccStartCommandDocumentProcess();

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

#endif // !defined(AFX_CCSTARTCOMMANDDOCUMENTPROCESS_H__62BB011F_9902_4FA9_9287_E30F22A51C1A__INCLUDED_)

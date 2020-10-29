// pcCommandDocumentAddDocument.h: interface for the pcCommandDocumentAddDocument class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PCCOMMANDDOCUMENTADDDOCUMENT_H__4D1B894F_8979_4FEA_841D_45C375A9664E__INCLUDED_)
#define AFX_PCCOMMANDDOCUMENTADDDOCUMENT_H__4D1B894F_8979_4FEA_841D_45C375A9664E__INCLUDED_


#include "xxCommand.h"

class pcCommandDocumentAddDocument : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	pcCommandDocumentAddDocument(long executionTime);
	pcCommandDocumentAddDocument(const pcCommandDocumentAddDocument& oldCommand);

	virtual ~pcCommandDocumentAddDocument();
	
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

	inline const zString GetDocumentName() const {return m_Name;}

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

	zString	 m_Name;	// Command document's name including ".xml" suffix

};

#endif // !defined(AFX_PCCOMMANDDOCUMENTADDDOCUMENT_H__4D1B894F_8979_4FEA_841D_45C375A9664E__INCLUDED_)

// ICommandDocumentImpl.h: interface for the ICommandDocumentImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICOMMANDDOCUMENTIMPL_H__21ED3609_0041_46FB_A2BC_B44125273186__INCLUDED_)
#define AFX_ICOMMANDDOCUMENTIMPL_H__21ED3609_0041_46FB_A2BC_B44125273186__INCLUDED_


// Forward declarations

class CExperiment;


#include "ICommandDocument.h"

class ICommandDocumentImpl : public ICommandDocument
{
	// ****************************************
	// Construction/Destruction
public:

	ICommandDocumentImpl(CExperiment& rExpt);

	virtual ~ICommandDocumentImpl();

	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	IExperiment* IIExperiment() const;

	bool ReadCommandDocument(const zString docName);

	// ****************************************
	// Public access functions
public:



	// ****************************************
	// Protected local functions
protected:


	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:



	// ****************************************
	// Data members
private:

	CExperiment& m_rExperiment;		// Reference to the containing class

	zString m_DocumentName;			// Name of command document's file (.xml)

	CommandSequence	m_Commands;		// Set of commands imported from the document

};

#endif // !defined(AFX_ICOMMANDDOCUMENTIMPL_H__21ED3609_0041_46FB_A2BC_B44125273186__INCLUDED_)

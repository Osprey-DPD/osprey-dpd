// IProtocolDocumentImpl.h: interface for the IProtocolDocumentImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPROTOCOLDOCUMENTIMPL_H__FF948F15_EE1B_4A90_9C03_2BAFBFDB9FA5__INCLUDED_)
#define AFX_IPROTOCOLDOCUMENTIMPL_H__FF948F15_EE1B_4A90_9C03_2BAFBFDB9FA5__INCLUDED_


// Forward declarations

class CExperiment;


#include "IProtocolDocument.h"

class IProtocolDocumentImpl : public IProtocolDocument
{
	// ****************************************
	// Construction/Destruction
public:

	IProtocolDocumentImpl(CExperiment& rExpt);

	virtual ~IProtocolDocumentImpl();

	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:


	// ****************************************
	// Public access functions
public:

	IExperiment* IIExperiment() const;

	bool ReadProtocolDocument(const zString docName);

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

	zString		 m_DocumentName;	// Name of protocol document (including ".xml" suffix)
};

#endif // !defined(AFX_IPROTOCOLDOCUMENTIMPL_H__FF948F15_EE1B_4A90_9C03_2BAFBFDB9FA5__INCLUDED_)

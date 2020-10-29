// IDocumentHandlerImpl.h: interface for the IDocumentHandlerImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IDOCUMENTHANDLERIMPL_H__430B5957_F5E4_46C8_9F7E_28FC2C034C4A__INCLUDED_)
#define AFX_IDOCUMENTHANDLERIMPL_H__430B5957_F5E4_46C8_9F7E_28FC2C034C4A__INCLUDED_


// Forward declarations

class CExperiment;
class ICommandDocument;
class IProtocolDocument;


#include "IDocumentHandler.h"

class IDocumentHandlerImpl : public IDocumentHandler
{
	// ****************************************
	// Construction/Destruction
public:

	IDocumentHandlerImpl(CExperiment& rExpt);

	virtual ~IDocumentHandlerImpl();

	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:


	// ****************************************
	// Public access functions
public:
	IProtocolDocument* GetIProtocolDocument();
	ICommandDocument* GetICommandDocument();

	IExperiment* IIExperiment() const;


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

};

#endif // !defined(AFX_IDOCUMENTHANDLERIMPL_H__430B5957_F5E4_46C8_9F7E_28FC2C034C4A__INCLUDED_)

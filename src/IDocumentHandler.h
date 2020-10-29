// IDocumentHandler.h: interface for the IDocumentHandler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IDOCUMENTHANDLER_H__1A89439A_824E_49E9_98BA_B16FCA672EE0__INCLUDED_)
#define AFX_IDOCUMENTHANDLER_H__1A89439A_824E_49E9_98BA_B16FCA672EE0__INCLUDED_


// Forward declarations

class IExperiment;
class ICommandDocument;
class IProtocolDocument;


class IDocumentHandler  
{
	// ****************************************
	// Construction/Destruction: no ctor as this is an interface class
public:


	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	virtual IExperiment* IIExperiment() const = 0;

	virtual ICommandDocument*  GetICommandDocument()  = 0;
	virtual IProtocolDocument* GetIProtocolDocument() = 0;

	// ****************************************
	// Public access functions
public:


	// ****************************************
	// Protected local functions
protected:

	virtual ~IDocumentHandler();

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

};

#endif // !defined(AFX_IDOCUMENTHANDLER_H__1A89439A_824E_49E9_98BA_B16FCA672EE0__INCLUDED_)

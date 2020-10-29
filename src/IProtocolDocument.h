// IProtocolDocument.h: interface for the IProtocolDocument class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPROTOCOLDOCUMENT_H__9F95F0BA_920A_4DBE_8650_319ED2AFCD7A__INCLUDED_)
#define AFX_IPROTOCOLDOCUMENT_H__9F95F0BA_920A_4DBE_8650_319ED2AFCD7A__INCLUDED_


// Forward declarations

class IExperiment;

// Include files

#include "xxBase.h"


class IProtocolDocument
{
	// ****************************************
	// Construction/Destruction: abstract base class has no ctor or dtor
public:


	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	virtual IExperiment* IIExperiment() const = 0;

	virtual bool ReadProtocolDocument(const zString docName) = 0;

	// ****************************************
	// Public access functions
public:


	// ****************************************
	// Protected local functions
protected:

	virtual ~IProtocolDocument();

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

};

#endif // !defined(AFX_IPROTOCOLDOCUMENT_H__9F95F0BA_920A_4DBE_8650_319ED2AFCD7A__INCLUDED_)

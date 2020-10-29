// ICommandDocument.h: interface for the ICommandDocument class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICOMMANDDOCUMENT_H__90CA4BCB_FC2E_4FE3_B2BC_78792D8F6AC8__INCLUDED_)
#define AFX_ICOMMANDDOCUMENT_H__90CA4BCB_FC2E_4FE3_B2BC_78792D8F6AC8__INCLUDED_


// Forward declarations

class IExperiment;

// Include file for access to types

#include "xxBase.h"


class ICommandDocument
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

	virtual bool ReadCommandDocument(const zString docName) = 0;

	// ****************************************
	// Public access functions
public:


	// ****************************************
	// Protected local functions
protected:

	virtual ~ICommandDocument();

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

};

#endif // !defined(AFX_ICOMMANDDOCUMENT_H__90CA4BCB_FC2E_4FE3_B2BC_78792D8F6AC8__INCLUDED_)

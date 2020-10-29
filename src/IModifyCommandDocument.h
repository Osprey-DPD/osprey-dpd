// IModifyCommandDocument.h: interface for the IModifyCommandDocument class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMODIFYCOMMANDDOCUMENT_H__CFB52A18_3802_41AE_8CF1_0A34BBD9B4C6__INCLUDED_)
#define AFX_IMODIFYCOMMANDDOCUMENT_H__CFB52A18_3802_41AE_8CF1_0A34BBD9B4C6__INCLUDED_


#include "IModifyProcess.h"

class IModifyCommandDocument : public IModifyProcess
{
	// ****************************************
	// Construction/Destruction: No ctor or dtor as this is an interface class
public:


	// ****************************************
	// Global functions, static member functions and variables
public:

	static IModifyCommandDocument* GetInterfacePointer(IModifyProcess* pIMP);

	// ****************************************
	// PVFs that must be overridden by the derived process class
public:

	virtual void GetPendingDocumentTotal(const xxCommand* const pCommand) const = 0;
	virtual void GetPendingDocuments(const xxCommand* const pCommand) const = 0;
	virtual void AddDocument(const xxCommand* const pCommand) = 0;

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
	// Data members: No member variables as this is an interface class
private:

};

#endif // !defined(AFX_IMODIFYCOMMANDDOCUMENT_H__CFB52A18_3802_41AE_8CF1_0A34BBD9B4C6__INCLUDED_)

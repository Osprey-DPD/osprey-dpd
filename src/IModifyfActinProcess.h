// IModifyfActinProcess.h: interface for the IModifyfActinProcess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMODIFYFACTINPROCESS_H__F62D75E5_2CEE_4A40_B1DB_15CC68CEF6AA__INCLUDED_)
#define AFX_IMODIFYFACTINPROCESS_H__F62D75E5_2CEE_4A40_B1DB_15CC68CEF6AA__INCLUDED_


#include "IModifyProcess.h"

class IModifyfActinProcess : public IModifyProcess
{
	// ****************************************
	// Construction/Destruction: ctor below is needed to pass 
	// data to the base class.
public:

	virtual ~IModifyfActinProcess();


	// ****************************************
	// Global functions, static member functions and variables
public:

	static IModifyfActinProcess* GetInterfacePointer(IModifyProcess* pIMP);

	// ****************************************
	// PVFs that must be overridden by the derived process class
public:

	virtual void SetCondenseDelay(const xxCommand* const pCommand) = 0;

	// ****************************************
	// Public access functions
public:

	// ****************************************
	// Protected local functions
protected:

	IModifyfActinProcess();

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members: No member variables as this is an interface class
private:

};

#endif // !defined(AFX_IMODIFYFACTINPROCESS_H__F62D75E5_2CEE_4A40_B1DB_15CC68CEF6AA__INCLUDED_)

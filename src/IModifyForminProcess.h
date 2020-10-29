// IModifyForminProcess.h: interface for the IModifyForminProcess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMODIFYFORMINPROCESS_H__F4ECD7DB_6A49_42D0_A544_B464A256987D__INCLUDED_)
#define AFX_IMODIFYFORMINPROCESS_H__F4ECD7DB_6A49_42D0_A544_B464A256987D__INCLUDED_


#include "IModifyProcess.h"

class IModifyForminProcess : public IModifyProcess
{
	// ****************************************
	// Construction/Destruction: ctor below is needed to pass 
	// data to the base class.
public:

	virtual ~IModifyForminProcess();


	// ****************************************
	// Global functions, static member functions and variables
public:

	static IModifyForminProcess* GetInterfacePointer(IModifyProcess* pIMP);

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

	IModifyForminProcess();

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members: No member variables as this is an interface class
private:

};

#endif // !defined(AFX_IMODIFYFORMINPROCESS_H__F4ECD7DB_6A49_42D0_A544_B464A256987D__INCLUDED_)

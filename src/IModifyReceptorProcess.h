// IModifyReceptorProcess.h: interface for the IModifyReceptorProcess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMODIFYRECEPTORPROCESS_H__6ff4016e_eaec_47a6_b922_bc8cb4d24b94__INCLUDED_)
#define AFX_IMODIFYRECEPTORPROCESS_H__6ff4016e_eaec_47a6_b922_bc8cb4d24b94__INCLUDED_



#include "IModifyProcess.h"

class IModifyReceptorProcess : public IModifyProcess
{
	// ****************************************
	// Construction/Destruction: ctor below is needed to pass 
	// data to the base class.
public:

	virtual ~IModifyReceptorProcess();


	// ****************************************
	// Global functions, static member functions and variables
public:

	static IModifyReceptorProcess* GetInterfacePointer(IModifyProcess* pIMP);

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

	IModifyReceptorProcess();

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members: No member variables as this is an interface class
private:

};

#endif // !defined(AFX_IMODIFYRECEPTORPROCESS_H__6ff4016e_eaec_47a6_b922_bc8cb4d24b94__INCLUDED_)

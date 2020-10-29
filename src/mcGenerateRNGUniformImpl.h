// mcGenerateRNGUniformImpl.h: interface for the mcGenerateRNGUniformImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCGENERATERNGUNIFORMIMPL_H__3D3D05A7_05BF_4CC3_A7D9_E2BF10D424FD__INCLUDED_)
#define AFX_MCGENERATERNGUNIFORMIMPL_H__3D3D05A7_05BF_4CC3_A7D9_E2BF10D424FD__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcGenerateRNGUniformImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcGenerateRNGUniformImpl();

	virtual ~mcGenerateRNGUniformImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void GenerateRNGUniform(const xxCommand* const pCommand);


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

};

#endif // !defined(AFX_MCGENERATERNGUNIFORMIMPL_H__3D3D05A7_05BF_4CC3_A7D9_E2BF10D424FD__INCLUDED_)

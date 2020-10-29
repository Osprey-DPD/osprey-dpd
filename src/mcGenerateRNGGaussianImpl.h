// mcGenerateRNGGaussianImpl.h: interface for the mcGenerateRNGGaussianImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCGENERATERNGGAUSSIANIMPL_H__BDA1DBD6_7316_4F41_88B3_422E0DB45C49__INCLUDED_)
#define AFX_MCGENERATERNGGAUSSIANIMPL_H__BDA1DBD6_7316_4F41_88B3_422E0DB45C49__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcGenerateRNGGaussianImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcGenerateRNGGaussianImpl();

	virtual ~mcGenerateRNGGaussianImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void GenerateRNGGaussian(const xxCommand* const pCommand);


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

#endif // !defined(AFX_MCGENERATERNGGAUSSIANIMPL_H__BDA1DBD6_7316_4F41_88B3_422E0DB45C49__INCLUDED_)

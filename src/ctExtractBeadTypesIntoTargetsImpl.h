// ctExtractBeadTypesIntoTargetsImpl.h: interface for the ctExtractBeadTypesIntoTargetsImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTEXTRACTBEADTYPESINTOTARGETSIMPL_H__CB2324B6_A4A7_49D5_A7BA_FC5E8F8B0AED__INCLUDED_)
#define AFX_CTEXTRACTBEADTYPESINTOTARGETSIMPL_H__CB2324B6_A4A7_49D5_A7BA_FC5E8F8B0AED__INCLUDED_


// Forward declarations

class xxCommand;

#include "ISimBoxCmd.h"

class ctExtractBeadTypesIntoTargetsImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ctExtractBeadTypesIntoTargetsImpl();

	virtual ~ctExtractBeadTypesIntoTargetsImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void ExtractBeadTypesIntoTargets(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CTEXTRACTBEADTYPESINTOTARGETSIMPL_H__CB2324B6_A4A7_49D5_A7BA_FC5E8F8B0AED__INCLUDED_)

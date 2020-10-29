// ccCreateActiveSimBoxImpl.h: interface for the ccCreateActiveSimBoxImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCCREATEACTIVESIMBOXIMPL_H__C597FB4C_D75B_49BF_8B7A_76D1C3170062__INCLUDED_)
#define AFX_CCCREATEACTIVESIMBOXIMPL_H__C597FB4C_D75B_49BF_8B7A_76D1C3170062__INCLUDED_


#include "ISimBoxCmd.h"

class ccCreateActiveSimBoxImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ccCreateActiveSimBoxImpl();

	virtual ~ccCreateActiveSimBoxImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void CreateActiveSimBox(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CCCREATEACTIVESIMBOXIMPL_H__C597FB4C_D75B_49BF_8B7A_76D1C3170062__INCLUDED_)

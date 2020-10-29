// ccAddCofilinBondImpl.h: interface for the ccAddCofilinBondImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCADDCOFILINBONDIMPL_H__97A48664_2E94_49CD_86F8_13085A42EFE3__INCLUDED_)
#define AFX_CCADDCOFILINBONDIMPL_H__97A48664_2E94_49CD_86F8_13085A42EFE3__INCLUDED_


#include "ISimBoxCmd.h"

class ccAddCofilinBondImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ccAddCofilinBondImpl();

	virtual ~ccAddCofilinBondImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void AddCofilinBond(const xxCommand* const pCommand);

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

#endif // !defined(AFX_CCADDCOFILINBONDIMPL_H__97A48664_2E94_49CD_86F8_13085A42EFE3__INCLUDED_)

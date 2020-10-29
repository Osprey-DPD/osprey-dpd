// ctCreateBondBindsForwardConeToPolymerHeadEventSourceImpl.h: interface for the ctCreateBondBindsForwardConeToPolymerHeadEventSourceImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTCREATEBONDBINDSFORWARDCONETOPOLYMERHEADEVENTSOURCEIMPL_H__75986951_CF21_4206_B414_8EC0D29AFBE9__INCLUDED_)
#define AFX_CTCREATEBONDBINDSFORWARDCONETOPOLYMERHEADEVENTSOURCEIMPL_H__75986951_CF21_4206_B414_8EC0D29AFBE9__INCLUDED_


#include "ISimBoxCmd.h"

class ctCreateBondBindsForwardConeToPolymerHeadEventSourceImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ctCreateBondBindsForwardConeToPolymerHeadEventSourceImpl();

	virtual ~ctCreateBondBindsForwardConeToPolymerHeadEventSourceImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void CreateBondBindsForwardConeToPolymerHeadEventSource(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CTCREATEBONDBINDSFORWARDCONETOPOLYMERHEADEVENTSOURCEIMPL_H__75986951_CF21_4206_B414_8EC0D29AFBE9__INCLUDED_)

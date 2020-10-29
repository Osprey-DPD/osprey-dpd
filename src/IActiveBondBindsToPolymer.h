// IActiveBondBindsToPolymer.h: interface for the IActiveBondBindsToPolymer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IACTIVEBONDBINDSTOPOLYMER_H__B0C7525A_F8B1_45D5_8C14_88088CA77DFF__INCLUDED_)
#define AFX_IACTIVEBONDBINDSTOPOLYMER_H__B0C7525A_F8B1_45D5_8C14_88088CA77DFF__INCLUDED_


#include "IActiveEventSource.h"

class IActiveBondBindsToPolymer : public IActiveEventSource
{
	// ****************************************
	// Construction/Destruction: interface class that wraps the aeActiveSimBox instance
public:

	IActiveBondBindsToPolymer(aeActiveSimBox* pShadow);

	virtual ~IActiveBondBindsToPolymer();


	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:


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
	// Data members: 
private:

};

#endif // !defined(AFX_IACTIVEBONDBINDSTOPOLYMER_H__B0C7525A_F8B1_45D5_8C14_88088CA77DFF__INCLUDED_)

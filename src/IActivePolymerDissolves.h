// IActivePolymerDissolves.h: interface for the IActivePolymerDissolves class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IACTIVEPOLYMERDISSOLVES_H__2C07E11E_8A64_47F7_A5FC_C6CA0845EB30__INCLUDED_)
#define AFX_IACTIVEPOLYMERDISSOLVES_H__2C07E11E_8A64_47F7_A5FC_C6CA0845EB30__INCLUDED_


#include "IActiveEventSource.h"

class IActivePolymerDissolves : public IActiveEventSource
{
	// ****************************************
	// Construction/Destruction: interface class that wraps the aeActiveSimBox instance
public:

	IActivePolymerDissolves(aeActiveSimBox* pShadow);

	virtual ~IActivePolymerDissolves();


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

#endif // !defined(AFX_IACTIVEPOLYMERDISSOLVES_H__2C07E11E_8A64_47F7_A5FC_C6CA0845EB30__INCLUDED_)

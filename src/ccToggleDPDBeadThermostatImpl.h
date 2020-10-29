// ccToggleDPDBeadThermostatImpl.h: interface for the ccToggleDPDBeadThermostatImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCTOGGLEDPDBEADTHERMOSTATIMPL_H__F5C1D6DD_23F1_4636_B806_2F2531B4F44E__INCLUDED_)
#define AFX_CCTOGGLEDPDBEADTHERMOSTATIMPL_H__F5C1D6DD_23F1_4636_B806_2F2531B4F44E__INCLUDED_


#include "ISimBoxCmd.h"

class ccToggleDPDBeadThermostatImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ccToggleDPDBeadThermostatImpl();

	virtual ~ccToggleDPDBeadThermostatImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void ToggleDPDBeadThermostat(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CCTOGGLEDPDBEADTHERMOSTATIMPL_H__F5C1D6DD_23F1_4636_B806_2F2531B4F44E__INCLUDED_)

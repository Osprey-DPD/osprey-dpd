// IModifySimStateIntegration.h: interface for the IModifySimStateIntegration class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMODIFYSIMSTATEINTEGRATION_H__F9427D95_4BCB_466C_B986_D8ECE984F547__INCLUDED_)
#define AFX_IMODIFYSIMSTATEINTEGRATION_H__F9427D95_4BCB_466C_B986_D8ECE984F547__INCLUDED_


#include "IModifySimState.h"

class IModifySimStateIntegration : public IModifySimState  
{
	// ****************************************
	// Construction/Destruction: 
public:

	IModifySimStateIntegration(CSimState& simState);

    virtual ~IModifySimStateIntegration();


	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes 
public:


	// ****************************************
	// Public access functions
public:

	void SetTimeStep(double dt);
	void SetTotalTime(long newTime);
	bool ToggleDPDBeadConservativeForces();
	bool ToggleDPDBeadForces();
	bool ToggleDPDBeadThermostat();


	// ****************************************
	// Protected local functions
protected:

	

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:

	// Explicitly disallow the copy constructor and assignment operators
	// by declaring them privately but providing NO definitions.

	IModifySimStateIntegration(const IModifySimStateIntegration& oldIModify);
	IModifySimStateIntegration& operator=(const IModifySimStateIntegration& rhs);

	// ****************************************
	// Data members
private:

};

#endif // !defined(AFX_IMODIFYSIMSTATEINTEGRATION_H__F9427D95_4BCB_466C_B986_D8ECE984F547__INCLUDED_)

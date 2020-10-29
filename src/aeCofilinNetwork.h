// aeCofilinNetwork.h: interface for the aeCofilinNetwork class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AECOFILINNETWORK_H__3C278C52_13D4_4B53_A9DD_0033D411AE60__INCLUDED_)
#define AFX_AECOFILINNETWORK_H__3C278C52_13D4_4B53_A9DD_0033D411AE60__INCLUDED_


#include "aeActiveCellNetwork.h"

class aeCofilinNetwork : public aeActiveCellNetwork
{
	// ****************************************
	// Construction/Destruction
public:

	aeCofilinNetwork(IActiveSimBox* const pShadow, long xn, long yn, long zn,
					 double xw, double yw, double zw);

	virtual ~aeCofilinNetwork();

	// ****************************************
	// Global functions, static member functions and variables
public:

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:


	// ****************************************
	// Public access functions
public:

	virtual void Evolve();

	// ****************************************
	// Protected local functions
protected:


	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:

	void Nucleate();		// Function to try and create new active polymers
	void Polymerise();		// Function to assemble/disaasemble active polymers


	// ****************************************
	// Data members
private:


};

#endif // !defined(AFX_AECOFILINNETWORK_H__3C278C52_13D4_4B53_A9DD_0033D411AE60__INCLUDED_)

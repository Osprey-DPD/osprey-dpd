// aeProfilinNetwork.h: interface for the aeProfilinNetwork class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AEPROFILINNETWORK_H__F3FEDD50_7227_47F3_9868_5EE9D75FD5D8__INCLUDED_)
#define AFX_AEPROFILINNETWORK_H__F3FEDD50_7227_47F3_9868_5EE9D75FD5D8__INCLUDED_


#include "aeActiveCellNetwork.h"

class aeProfilinNetwork : public aeActiveCellNetwork
{
	// ****************************************
	// Construction/Destruction
public:

	aeProfilinNetwork(IActiveSimBox* const pShadow, long xn, long yn, long zn,
					  double xw, double yw, double zw);

	virtual ~aeProfilinNetwork();

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

#endif // !defined(AFX_AEPROFILINNETWORK_H__F3FEDD50_7227_47F3_9868_5EE9D75FD5D8__INCLUDED_)

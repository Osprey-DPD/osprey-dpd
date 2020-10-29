// aefActinCPNetwork.h: interface for the aefActinCPNetwork class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AEFACTINCPNETWORK_H__2FE3E1CE_E2A3_4B34_8BE6_E7CE10B60957__INCLUDED_)
#define AFX_AEFACTINCPNETWORK_H__2FE3E1CE_E2A3_4B34_8BE6_E7CE10B60957__INCLUDED_


#include "aeActiveCellNetwork.h"

class aefActinCPNetwork : public aeActiveCellNetwork
{
	// ****************************************
	// Construction/Destruction
public:

	aefActinCPNetwork(IActiveSimBox* const pShadow, long xn, long yn, long zn,
					  double xw, double yw, double zw);

	virtual ~aefActinCPNetwork();

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

#endif // !defined(AFX_AEFACTINCPNETWORK_H__2FE3E1CE_E2A3_4B34_8BE6_E7CE10B60957__INCLUDED_)

// aeArp23Network.h: interface for the aeArp23Network class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AEARP23NETWORK_H__02BE2E31_CDA9_46C9_AF08_EAEFDD154948__INCLUDED_)
#define AFX_AEARP23NETWORK_H__02BE2E31_CDA9_46C9_AF08_EAEFDD154948__INCLUDED_




#include "aeActiveCellNetwork.h"

class aeArp23Network : public aeActiveCellNetwork
{
	// ****************************************
	// Construction/Destruction
public:

	aeArp23Network(IActiveSimBox* const pShadow, long xn, long yn, long zn,
				   double xw, double yw, double zw);

	virtual ~aeArp23Network();

	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();		// return the ACN name

private:

	static const zString m_Type;		// Identifier used to route commands

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	virtual void Evolve();

	virtual const zString GetACNType() const;

	// Functions to modify the network

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
	// Data members
private:



};

#endif // !defined(AFX_AEARP23NETWORK_H__02BE2E31_CDA9_46C9_AF08_EAEFDD154948__INCLUDED_)

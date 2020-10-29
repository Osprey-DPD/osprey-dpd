// aeBondConnection.h: interface for the aeBondConnection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AEBONDCONNECTION_H__978FF635_F171_42C1_8313_4ACAB4FB1FA1__INCLUDED_)
#define AFX_AEBONDCONNECTION_H__978FF635_F171_42C1_8313_4ACAB4FB1FA1__INCLUDED_


// Include files

#include "AbstractBead.h"
#include "Bond.h"
#include "BondPair.h"
#include "Polymer.h"
#include "aeActiveBond.h"

class aeBondConnection  
{
	// ****************************************
	// Construction/Destruction - protected constructor declared below
public:

	virtual ~aeBondConnection();


	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// Function used by aeActivePolymer to calculate the binding forces 
	// between bonds. Each aeBondConnection class can have a distinct force law.
	// All except AddForce() provide a do-nothing body here to avoid all derived
	// classes having to implement unused functions.

	virtual void AddForce() = 0;
	virtual void AddThreeBodyForce();

	// Functions used by aeActiveBond to get the specific connection instance
	// to update its force-generating function with a new head monomer.

	virtual bool AddHeadMonomer(CPolymer* const pHead) = 0;

	// New function to wrap the previous one so that an aeBondConnection instance
	// has access to all adjacent bonds when setting up its force law.

	virtual bool AddHeadMonomer(aeActiveBond* const pHeadBond) = 0;

	// Function that allows a bond connection instance to update the states
	// of its owner bond and neighbours when they are separated

	virtual bool RemoveHeadMonomer(aeActiveBond* const pHeadBond) = 0;

	
	// ****************************************
	// Public access functions
public:

	inline  CPolymer*  GetHeadMonomer()			const {return m_pHead;}
	inline  CPolymer*  GetTailMonomer()			const {return m_pTail;}
	inline  CPolymer*  GetFuelMonomer()			const {return m_pFuel;}


	inline bool	GetThreeBodyForce()				const {return m_bThreeBodyForce;}

	// ****************************************
	// Protected local functions
protected:

	aeBondConnection(CPolymer* const pHead, CPolymer* const pTail, CPolymer* const pFuel);

	// Functions used by derived classes and the aeActiveBond class
	// to toggle the calculation of 3-body forces

	inline void SetThreeBodyForceOn()  {m_bThreeBodyForce = true;}
	inline void SetThreeBodyForceOff() {m_bThreeBodyForce = false;}

	// ****************************************
	// Implementation
protected:

	// CPolymers that define the attachment points for the connection between 
	// two active bonds. We include the fuel polymer to allow for energy-consuming
	// processes.

	CPolymer*		  m_pHead;					// Variable monomer attached as the bond's head
	CPolymer*	const m_pTail;					// Fixed monomer wrapped by the active bond
	CPolymer*		  m_pFuel;					// Variable monomer attached as the bond's fuel

	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members

private:

	const bool m_bTwoBodyForce;		// Flag showing that 2-body forces are always present
	      bool m_bThreeBodyForce;	// Flag showing whether 3-body forces are present



};

#endif // !defined(AFX_AEBONDCONNECTION_H__978FF635_F171_42C1_8313_4ACAB4FB1FA1__INCLUDED_)

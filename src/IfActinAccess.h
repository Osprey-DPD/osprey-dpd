// IfActinAccess.h: interface for the IfActinAccess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IFACTINACCESS_H__9014A496_546B_4C5F_A00C_4A801051D789__INCLUDED_)
#define AFX_IFACTINACCESS_H__9014A496_546B_4C5F_A00C_4A801051D789__INCLUDED_


// Forward declarations

class aefActinNetwork;
class aeActiveBond;


#include "IfActinAccessFromArp23.h"
#include "IfActinAccessFromFormin.h"
#include "IfActinAccessFromReceptor.h"

class IfActinAccess : public IfActinAccessFromArp23, public IfActinAccessFromFormin, public IfActinAccessFromReceptor
{
	// ****************************************
	// Construction/Destruction: 
public:

	IfActinAccess(aefActinNetwork* const pNetwork);

	virtual ~IfActinAccess();

	// ****************************************
	// Global functions, static member functions and variables
public:

	// ****************************************
	// PVFs
public:

	// ****************************************
	// Public access functions
public:


	// ****************************************
	// Protected local functions
protected:

	// Functions used by the Arp23 network


	// Functions used by the Formin network

	virtual void AddFreeBond(aeActiveBond* const pBond);
	virtual void AddFreePolymer(aeActivePolymer* const pPoly);
	virtual aeActiveBond* GetNearbyMonomer(double x, double y, double z);
	virtual void RemoveNearbyMonomer(aeActiveBond* const pBond);
	virtual void SetBondExternalTriggerOn(aeActiveBond* pBond);


	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:

	// Explicitly disallow the copy constructor and assignment operators
	// by declaring them privately but providing NO definitions.

	IfActinAccess(const IfActinAccess& old);
	IfActinAccess& operator=(const IfActinAccess& rhs);

	// ****************************************
	// Data members:
private:

	aefActinNetwork* const m_pNetwork;	// Pointer to the fActin network being accessed

};

#endif // !defined(AFX_IFACTINACCESS_H__9014A496_546B_4C5F_A00C_4A801051D789__INCLUDED_)

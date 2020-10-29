// aevBondHydrolysesATP.h: interface for the aevBondHydrolysesATP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AEVBONDHYDROLYSESATP_H__538DD092_CE79_46D3_865B_7897B940579A__INCLUDED_)
#define AFX_AEVBONDHYDROLYSESATP_H__538DD092_CE79_46D3_865B_7897B940579A__INCLUDED_


// Forward declarations

class IActiveBondHydrolysesATP;
class aeActiveBond;
class aefActinBond;


#include "aevActiveEvent.h"

class aevBondHydrolysesATP : public aevActiveEvent  
{
	// ****************************************
	// Construction/Destruction
public:

	aevBondHydrolysesATP();
	aevBondHydrolysesATP(const aevBondHydrolysesATP& oldEvent);

	virtual ~aevBondHydrolysesATP();

	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	// Return the type of event

	static const zString m_Type;	// Identifier used in control data file

	// ****************************************
	// PVFs inherited from xxEvent
public:

	zOutStream& put(zOutStream& os) const;
	zInStream&  get(zInStream& is);

	virtual bool CheckPreconditions();
	virtual bool Execute(IActiveSimBox* const pShadow);

	virtual const zString GetEventType() const;

	virtual aevActiveEvent* GetEvent() const;

	virtual bool InternalValidateData();

protected:

	// ****************************************
	// Public access functions
public:

    // Function to return the id of the wrapped bond

	long GetBondId() const;
 
	// Parameters required for this event

	void SetBond(aeActiveBond* const pBond);

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
private:							// Local data specified by user


	IActiveBondHydrolysesATP* m_pIEvent;	// Pointer to the IActiveEventSource interface instance

	aefActinBond* m_pBond;		// Containing bond: hardwired to fActin for now

};

#endif // !defined(AFX_AEVBONDHYDROLYSESATP_H__538DD092_CE79_46D3_865B_7897B940579A__INCLUDED_)

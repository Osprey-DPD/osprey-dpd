// aevBondReleasesPi.h: interface for the aevBondReleasesPi class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AEVBONDRELEASESPI_H__D463A1BD_10BC_49FC_868E_0C5FA0FE8CDA__INCLUDED_)
#define AFX_AEVBONDRELEASESPI_H__D463A1BD_10BC_49FC_868E_0C5FA0FE8CDA__INCLUDED_


// Forward declarations

class IActiveBondReleasesPi;
class aeActiveBond;
class aefActinBond;


#include "aevActiveEvent.h"

class aevBondReleasesPi : public aevActiveEvent  
{
	// ****************************************
	// Construction/Destruction
public:

	aevBondReleasesPi();
	aevBondReleasesPi(const aevBondReleasesPi& oldEvent);

	virtual ~aevBondReleasesPi();

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


	IActiveBondReleasesPi* m_pIEvent;	// Pointer to the IActiveEventSource interface instance

	aefActinBond* m_pBond;		// Containing bond

};

#endif // !defined(AFX_AEVBONDRELEASESPI_H__D463A1BD_10BC_49FC_868E_0C5FA0FE8CDA__INCLUDED_)

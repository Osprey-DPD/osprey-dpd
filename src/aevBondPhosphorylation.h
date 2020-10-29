// aevBondPhosphorylation.h: interface for the aevBondPhosphorylation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AEVBONDPHOSPHORYLATION_H__EEFCA3CC_082B_4C04_9212_3FD6B747C3E0__INCLUDED_)
#define AFX_AEVBONDPHOSPHORYLATION_H__EEFCA3CC_082B_4C04_9212_3FD6B747C3E0__INCLUDED_


// Forward declarations

class IActiveBondPhosphorylation;
class aeActiveBond;
class aefActinBond;


#include "aevActiveEvent.h"

class aevBondPhosphorylation : public aevActiveEvent  
{
	// ****************************************
	// Construction/Destruction
public:

	aevBondPhosphorylation();
	aevBondPhosphorylation(const aevBondPhosphorylation& oldEvent);

	virtual ~aevBondPhosphorylation();

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


	IActiveBondPhosphorylation* m_pIEvent;	// Pointer to the IActiveEventSource interface instance

	aefActinBond* m_pBond;		// Containing bond

};

#endif // !defined(AFX_AEVBONDPHOSPHORYLATION_H__EEFCA3CC_082B_4C04_9212_3FD6B747C3E0__INCLUDED_)

// aevBondUnbindsFromPolymerTail.h: interface for the aevBondUnbindsFromPolymerTail class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AEVBONDUNBINDSFROMPOLYMERTAIL_H__6E211993_BC0D_4FB9_A8DA_EBC46E2FCCB4__INCLUDED_)
#define AFX_AEVBONDUNBINDSFROMPOLYMERTAIL_H__6E211993_BC0D_4FB9_A8DA_EBC46E2FCCB4__INCLUDED_


// Forward declarations

class IActiveBondUnbindsFromPolymer;
class aeActiveBond;
class CBond;


#include "aevActiveEvent.h"

class aevBondUnbindsFromPolymerTail : public aevActiveEvent  
{
	// ****************************************
	// Construction/Destruction
public:

	aevBondUnbindsFromPolymerTail();
	aevBondUnbindsFromPolymerTail(const aevBondUnbindsFromPolymerTail& oldEvent);

	virtual ~aevBondUnbindsFromPolymerTail();

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

    // Over-ridden VF to allow the event class to broadcast its state.

    virtual void BroadcastState();


	// ****************************************
	// Public access functions
public:

	// Parameters required for this event

	inline long   GetDuration()       const {return m_Duration;}
	inline double GetSpringConstant() const {return m_SpringConstant;}
	inline double GetLength()         const {return m_Length;}

	void SetPolymer(aeActivePolymer* const pPolymer);
	void SetDuration(long time);
	void SetSpringConstant(double spring);
	void SetLength(double length);

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

	long    m_Duration;					// No of steps needed for bond to bind
	double  m_SpringConstant;			// Final value of the transient bond spring constant
	double  m_Length;					// Final value of bond's unstretched length (= max bond separation)

	IActiveBondUnbindsFromPolymer* m_pIEvent;	// Pointer to the IActiveEventSource interface instance

	aeActivePolymer*  m_pPolymer;		// Active polymer growing by this event

	aeActiveBond* m_pOldTailBond;		// Current tail bond of polymer
	CBond*	const m_pInternalBond;		// Transient bond to connect the new end monomer
	long	m_Counter;					// Local counter for the bond addition time

};

#endif // !defined(AFX_AEVBONDUNBINDSFROMPOLYMERTAIL_H__6E211993_BC0D_4FB9_A8DA_EBC46E2FCCB4__INCLUDED_)

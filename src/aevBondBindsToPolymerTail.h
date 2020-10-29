// aevBondBindsToPolymerTail.h: interface for the aevBondBindsToPolymerTail class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AEVBONDBINDSTOPOLYMERTAIL_H__B989B054_003C_4509_8BF2_5097708E0ADF__INCLUDED_)
#define AFX_AEVBONDBINDSTOPOLYMERTAIL_H__B989B054_003C_4509_8BF2_5097708E0ADF__INCLUDED_


// Forward declarations

class IActiveBondBindsToPolymer;
class aeActiveBond;
class CBond;


#include "aevActiveEvent.h"

class aevBondBindsToPolymerTail : public aevActiveEvent  
{
	// ****************************************
	// Construction/Destruction
public:

	aevBondBindsToPolymerTail();
	aevBondBindsToPolymerTail(const aevBondBindsToPolymerTail& oldEvent);

	virtual ~aevBondBindsToPolymerTail();

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

	// Parameters required for this event

	inline long   GetDuration()       const {return m_Duration;}
	inline double GetRange()          const {return m_Range;}
	inline double GetSpringConstant() const {return m_SpringConstant;}
	inline double GetLength()         const {return m_Length;}

	void SetPolymer(aeActivePolymer* const pPolymer);
	void SetDuration(long time);
	void SetRange(double range);
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
	double  m_Range;					// Distance out to which a bond will looked for
	double  m_SpringConstant;			// Final value of the transient bond spring constant
	double  m_Length;					// Final value of bond's unstretched length (= max bond separation)

	IActiveBondBindsToPolymer* m_pIEvent;	// Pointer to the IActiveEventSource interface instance

	aeActivePolymer*  m_pPolymer;		// Active polymer growing by this event

	aeActiveBond* m_pNewTailBond;		// New bond that is attaching to the tail
	CBond*	const m_pInternalBond;		// Transient bond to connect the new end monomer
	long	m_Counter;					// Local counter for the bond addition time

};

#endif // !defined(AFX_AEVBONDBINDSTOPOLYMERTAIL_H__B989B054_003C_4509_8BF2_5097708E0ADF__INCLUDED_)

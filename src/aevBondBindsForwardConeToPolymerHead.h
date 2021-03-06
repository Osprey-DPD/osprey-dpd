// aevBondBindsForwardConeToPolymerHead.h: interface for the aevBondBindsForwardConeToPolymerHead class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AEVBONDBINDSFORWARDCONETOPOLYMERHEAD_H__47B20956_9E49_49C3_8349_807303AA60F7__INCLUDED_)
#define AFX_AEVBONDBINDSFORWARDCONETOPOLYMERHEAD_H__47B20956_9E49_49C3_8349_807303AA60F7__INCLUDED_


// Forward declarations

class IActiveBondBindsToPolymer;
class aeActiveBond;
class CBond;


#include "aevActiveEvent.h"

class aevBondBindsForwardConeToPolymerHead : public aevActiveEvent  
{
	// ****************************************
	// Construction/Destruction
public:

	aevBondBindsForwardConeToPolymerHead();
	aevBondBindsForwardConeToPolymerHead(const aevBondBindsForwardConeToPolymerHead& oldEvent);

	virtual ~aevBondBindsForwardConeToPolymerHead();

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
	inline double GetRange()          const {return m_Range;}
	inline double GetConeAngle()      const {return m_ConeAngle;} // Angle in radians
	inline double GetSpringConstant() const {return m_SpringConstant;}
	inline double GetLength()         const {return m_Length;}

	void SetPolymer(aeActivePolymer* const pPolymer);
	void SetDuration(long time);
	void SetRange(double range);
	void SetConeAngle(double angle);
	void SetSpringConstant(double spring);
	void SetLength(double length);

    // Functions used by Event Source Decorators to analyse this event

    const aeActiveBond* GetHeadBond() const;


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
    double  m_ConeAngle;				// Cone half-angle (radians <= pi/2)
	double  m_SpringConstant;			// Final value of the transient bond spring constant
	double  m_Length;					// Final value of bond's unstretched length (= max bond separation)

	IActiveBondBindsToPolymer* m_pIEvent;	// Pointer to the IActiveEventSource interface instance

	aeActivePolymer*  m_pPolymer;		// Active polymer growing by this event

	aeActiveBond* m_pNewHeadBond;		// New bond that is attaching to the head
	CBond*	const m_pInternalABond;		// Transient bond to connect the new end monomer
	CBond*	const m_pInternalBBond;		// Transient bond to connect the new end monomer
	long	m_Counter;					// Local counter for the bond addition time

	double  m_CosCA;					// Cosing of the cone angle
};

#endif // !defined(AFX_AEVBONDBINDSFORWARDCONETOPOLYMERHEAD_H__47B20956_9E49_49C3_8349_807303AA60F7__INCLUDED_)

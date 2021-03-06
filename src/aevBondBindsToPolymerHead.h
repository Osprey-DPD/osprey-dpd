// aevBondBindsToPolymerHead.h: interface for the aevBondBindsToPolymerHead class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AEVBONDBINDSTOPOLYMERHEAD_H__F9542221_6941_4001_918A_0E066C78FA8B__INCLUDED_)
#define AFX_AEVBONDBINDSTOPOLYMERHEAD_H__F9542221_6941_4001_918A_0E066C78FA8B__INCLUDED_


// Forward declarations

class IActiveBondBindsToPolymer;
class aeActiveBond;
class CBond;


#include "aevActiveEvent.h"

class aevBondBindsToPolymerHead : public aevActiveEvent  
{
	// ****************************************
	// Construction/Destruction
public:

	aevBondBindsToPolymerHead();
	aevBondBindsToPolymerHead(const aevBondBindsToPolymerHead& oldEvent);

	virtual ~aevBondBindsToPolymerHead();

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

	aeActiveBond* m_pNewHeadBond;		// New bond that is attaching to the head
	CBond*	const m_pInternalBond;		// Transient bond to connect the new end monomer
	long	m_Counter;					// Local counter for the bond addition time

};

#endif // !defined(AFX_AEVBONDBINDSTOPOLYMERHEAD_H__F9542221_6941_4001_918A_0E066C78FA8B__INCLUDED_)

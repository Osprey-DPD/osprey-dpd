// aevPolymerFormsBondTripleBondTriple.h: interface for the aevPolymerFormsBondTripleBondTriple class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AEVPOLYMERFORMSBONDTRIPLE_H__EBB16230_D216_4C71_BEFC_817FE88A831F__INCLUDED_)
#define AFX_AEVPOLYMERFORMSBONDTRIPLE_H__EBB16230_D216_4C71_BEFC_817FE88A831F__INCLUDED_


// Forward declarations

class IActivePolymerFormsBondTriple;
class aeActiveBond;
class CBond;


#include "aevActiveEvent.h"

class aevPolymerFormsBondTriple : public aevActiveEvent  
{
	// ****************************************
	// Construction/Destruction
public:

	aevPolymerFormsBondTriple();
	aevPolymerFormsBondTriple(const aevPolymerFormsBondTriple& oldEvent);

	virtual ~aevPolymerFormsBondTriple();

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

	void SetDuration(long time);
	void SetRange(double range);
	void SetSpringConstant(double spring);
	void SetLength(double length);

	inline void AddTailBond(ActiveBondListIterator iterBond) {m_iterTail = iterBond;}

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

	long    m_Duration;					// No of steps needed for complete binding
	double  m_Range;					// Distance out to which second bond will be captured
	double  m_SpringConstant;			// Final value of the transient bond spring constant
	double  m_Length;					// Final value of bond's unstretched length


	IActivePolymerFormsBondTriple* m_pIEvent;		// Pointer to the IActiveEventSource interface instance

	ActiveBondListIterator m_iterTail;	// Iterator to tail bond;

	aeActiveBond*  m_pTail;			    // First bond required to form an active polymer
	aeActiveBond*  m_pFirstHead;	    // Second bond required to form an active polymer
	aeActiveBond*  m_pSecondHead;	    // Second bond required to form an active polymer

	aeActivePolymer*  m_pPolymer;		// Polymer created as the result of this event

	CBond*	const m_pInternalBond;		// Transient bond to draw the monomers together
	long	m_Counter;					// Local counter for the bond formation time

};


#endif // !defined(AFX_AEVPOLYMERFORMSBONDTRIPLE_H__EBB16230_D216_4C71_BEFC_817FE88A831F__INCLUDED_)

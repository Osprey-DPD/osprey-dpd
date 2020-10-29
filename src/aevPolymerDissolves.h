// aevPolymerDissolves.h: interface for the aevPolymerDissolves class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AEVPOLYMERDISSOLVES_H__3FFB27BD_AAD0_4349_A22B_288E0C76474D__INCLUDED_)
#define AFX_AEVPOLYMERDISSOLVES_H__3FFB27BD_AAD0_4349_A22B_288E0C76474D__INCLUDED_


// Forward declarations

class IActivePolymerDissolves;
class aeActiveBond;
class CBond;


#include "aevActiveEvent.h"

class aevPolymerDissolves : public aevActiveEvent  
{
	// ****************************************
	// Construction/Destruction
public:

	aevPolymerDissolves();
	aevPolymerDissolves(const aevPolymerDissolves& oldEvent);

	virtual ~aevPolymerDissolves();

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

	long    m_MinLengthTime;			// No of steps needed to initiate dissolution
	long    m_Duration;			        // No of steps needed for separation
	double  m_SpringConstant;			// Final value of the transient bond spring constant
	double  m_Length;					// Final value of bond's unstretched length (= max bond separation)


	IActivePolymerDissolves* m_pIEvent;	// Pointer to the IActiveEventSource interface instance

	aeActivePolymer*  m_pPolymer;		// Active polymer being dissolved by this event

	CBond*	const m_pInternalBond;		// Transient bond to force the monomers apart
	long	m_Counter;					// Local counter for the bond formation time
};

#endif // !defined(AFX_AEVPOLYMERDISSOLVES_H__3FFB27BD_AAD0_4349_A22B_288E0C76474D__INCLUDED_)

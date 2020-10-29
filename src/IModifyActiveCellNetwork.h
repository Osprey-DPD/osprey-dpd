// IModifyActiveCellNetwork.h: interface for the IModifyActiveCellNetwork class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMODIFYACTIVECELLNETWORK_H__EA9E1F5F_9D08_4C40_8080_E5E036645E1C__INCLUDED_)
#define AFX_IMODIFYACTIVECELLNETWORK_H__EA9E1F5F_9D08_4C40_8080_E5E036645E1C__INCLUDED_


// Forward declarations

class xxCommand;


#include "aeActiveCellNetwork.h"

class IModifyActiveCellNetwork : public aeActiveCellNetwork
{
	// ****************************************
	// Construction/Destruction: ctor is needed to pass data to the
	// base class.	
public:


	virtual ~IModifyActiveCellNetwork();


	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

    virtual void SetFixedHeadOffRate(double rate)           = 0;
    virtual void SetFixedTailOffRate(double rate)           = 0;

	virtual void SetProximityOnSeparation(double max)		= 0;
	virtual void SetProximityOffSeparation(double max)		= 0;
	virtual void SetProximityHeadOnSeparation(double max)   = 0;
	virtual void SetProximityHeadOffSeparation(double max)  = 0;
	virtual void SetProximityTailOnSeparation(double max)   = 0;
	virtual void SetProximityTailOffSeparation(double max)  = 0;

	// ****************************************
	// Public access functions
public:

    // Command handler functions for ACNs

	void CancelConstantFreeBondConcentration();
	void CancelFreeActiveBondDisplayUpdate();
	void CancelFreeActivePolymerDisplayUpdate();
	bool ToggleCumulativeEventStatistics();

	void Save1dDensityField(const xxCommand* const pCommand);

	void SetActivePolymerLimit(long max);
	void SetConstantFreeBondConcentration(double fraction, long phantomsPerEvent);
	void SetFreeActiveBondsInvisible();
	void SetFreeActiveBondsVisible();
	void SetFreeActivePolymersInvisible();
	void SetFreeActivePolymersVisible();
	void SetFreeActivePolymersVisibleByMinLength(long minSize);
	void SetEventExecutionPeriod(const zString eventType, long period);

	// Forwarding functions to the ACN's events

	const zString GetPolymerFormsEventType() const;
	const zString GetPolymerDissolvesEventType() const;
	const zString GetBondBindsToPolymerHeadEventType() const;
	const zString GetBondBindsToPolymerTailEventType() const;
	const zString GetBondUnbindsFromPolymerHeadEventType() const;
	const zString GetBondUnbindsFromPolymerTailEventType() const;
	long  GetEventExecutionPeriodFromName(const zString eventType) const;

	aevActiveEvent* GetPolymerFormsEvent();
	aevActiveEvent* GetPolymerDissolvesEvent();
	aevActiveEvent* GetBondBindsToPolymerHeadEvent();
	aevActiveEvent* GetBondBindsToPolymerTailEvent();
	aevActiveEvent* GetBondUnbindsFromPolymerHeadEvent();
	aevActiveEvent* GetBondUnbindsFromPolymerTailEvent();

	// ****************************************
	// Protected local functions
protected:

	IModifyActiveCellNetwork(IActiveSimBox* const pShadow, long xn, long yn, long zn, double xw, double yw, double zw);

	
	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members: No member variables as this is an interface class
private:


};

#endif // !defined(AFX_IMODIFYACTIVECELLNETWORK_H__EA9E1F5F_9D08_4C40_8080_E5E036645E1C__INCLUDED_)

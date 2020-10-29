// taEventSourceDecorator.h: interface for the taEventSourceDecorator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TAEVENTSOURCEDECORATOR_H__7EE5CDCF_5CFC_4CD3_A17F_6548174E5B87__INCLUDED_)
#define AFX_TAEVENTSOURCEDECORATOR_H__7EE5CDCF_5CFC_4CD3_A17F_6548174E5B87__INCLUDED_


// Forward declarations

class taEventAnalysisDecorator;
class aevActiveEvent;

#include "CommandTargetNode.h"

class taEventSourceDecorator : public CCommandTargetNode
{
	// ****************************************
	// Construction/Destruction - protected constructor declared below
public:

	virtual ~taEventSourceDecorator();


	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be implemented by all instantiated derived classes 
public:

	// ****************************************
	// Function needed to distinguish node types.

	virtual bool IsComposite()		   const;

	// ****************************************
	// Functions common to simple and composite targets

	virtual bool IsBeadTypeInTarget(long type)  const;
	virtual long GetBeadTotalForType(long type)	const;
	virtual long GetBeadTotal()					const;
	virtual BeadVector GetBeads()				const;

	// ****************************************
	// Functions implemented by CCommandTarget

	virtual long GetCurrentBeadType()				const;
	virtual long GetPreviousBeadType(long newType)	const;
	virtual long GetOriginalBeadType()				const;
	virtual bool AddBeadTypePair(long newType, long oldType);
	virtual bool RemoveBeadTypePair(long newType);
	virtual bool ClearBeadTypePairs();

	// ****************************************
	// Functions implemented by CCompositeTarget

	virtual long GetTargetTotal() const;
	virtual bool AddTarget(CCommandTargetNode* const pTarget);
	virtual bool RemoveTarget(const zString label);
	virtual CCommandTargetNode* GetTarget(const zString label)	const;

	// Functions used by other decorator classes


    // Function used by ACNs to pass an event pointer to an ESD.

    virtual void BroadcastEventState(const aevActiveEvent* const pEvent) = 0;

	// ****************************************
	// Public access functions
public:


    // ****************************************
    // IEventSourceDecorator interface

    // Interface used by all Event Analysis Decorators to get access to the
    // generic data stream from the ESD.

	bool AddEventAnalysis(const zString key, taEventAnalysisDecorator* const pAnalysis);
	bool RemoveEventAnalysis(const zString sourceType);


	long GetSuccessTime() const;
	long GetFailureTime() const;

protected:

	void SetSuccessTime(long time);
    void SetFailureTime(long time);


    // ****************************************
    // Event-specific interfaces used by particular EADs
public:


	// ****************************************
	// Protected local functions
protected:

	// Protected constructor allows only derived classes to be instantiated

	taEventSourceDecorator(const zString label, CCommandTargetNode* const pDec);

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members

protected:

    StringEventAnalysisMMap  m_mEventAnalysisFromType;  // Multimap of (id, event analysis decorator) pairs

private:

    // Data related to IEventSourceDecorator interface

    long m_SuccessTime;  // Simulation time of event's last success
    long m_FailureTime;  // Simulation time of event's last failure

};

#endif // !defined(AFX_TAEVENTSOURCEDECORATOR_H__7EE5CDCF_5CFC_4CD3_A17F_6548174E5B87__INCLUDED_)

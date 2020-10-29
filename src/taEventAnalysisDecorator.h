// taEventAnalysisDecorator.h: interface for the taEventAnalysisDecorator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TAEVENTANALYSISDECORATOR_H__C1B414DD_BB59_449A_B62E_BABC2F708DE3__INCLUDED_)
#define AFX_TAEVENTANALYSISDECORATOR_H__C1B414DD_BB59_449A_B62E_BABC2F708DE3__INCLUDED_


// Forward declarations

class IEAD;
class taEventSourceDecorator;


#include "CommandTargetNode.h"

class taEventAnalysisDecorator : public CCommandTargetNode
{
	// ****************************************
	// Construction/Destruction - protected constructor declared below
public:

	virtual ~taEventAnalysisDecorator();


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


    // Connect the EAD to its source ESD using a pointer to an interface that  
    // each derived class specifies.

    virtual IEAD* ConnectESDInterface(taEventSourceDecorator* const pESD) = 0;

    // Function used by a taEventSourceDecorator to pass event data for analysis

    virtual void AnalyseEventState() = 0;

    virtual void Normalise() = 0;

	// ****************************************
	// Public access functions
public:

    bool IsTimeToNormalise();
    long GetSamplesPerAnalysis() const {return m_SamplesPerAnalysis;}
    long GetCounter()            const {return m_Counter;}

	// ****************************************
	// Protected local functions
protected:

	// Protected constructor allows only derived classes to be instantiated

	taEventAnalysisDecorator(const zString label, CCommandTargetNode* const pDec, long samples);


    void IncrementCounter();

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members

protected:

    IEAD* m_pIEAD; // Pointer to the EAD's interface wrapping its ESD

private:

    long m_SamplesPerAnalysis;  // Number of calls to Execute() function per analysis

    // Local data

    long m_Counter; // Counter used to determine when to normalize the data


};

#endif // !defined(AFX_TAEVENTANALYSISDECORATOR_H__C1B414DD_BB59_449A_B62E_BABC2F708DE3__INCLUDED_)

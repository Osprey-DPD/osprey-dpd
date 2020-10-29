// taCumulateDecorator.h: interface for the taCumulateDecorator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TACUMULATEDECORATOR_H__3557FCA1_DC37_49DB_B1B2_C15722E65D14__INCLUDED_)
#define AFX_TACUMULATEDECORATOR_H__3557FCA1_DC37_49DB_B1B2_C15722E65D14__INCLUDED_



#include "CommandTargetNode.h"

class taCumulateDecorator : public CCommandTargetNode
{
public:
	// ****************************************
	// Construction/Destruction - protected constructor declared below
public:

	virtual ~taCumulateDecorator();


	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be implemented by all instantiated derived classes 
public:


	// ****************************************
	// Public access functions
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

	long GetStartTime() const {return m_Start;}
	long GetEndTime()	const {return m_End;}

	// ****************************************
	// Protected local functions
protected:

	// Protected constructor allows only derived classes to be instantiated

	taCumulateDecorator(const zString label, CCommandTargetNode* const pDec, 
						const zString wrappedLabel, long start, long end);

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members

protected:

	bool			m_bWrapFailure;		// Flag showing a failure of the required wrapped decorator
	const zString	m_WrappedLabel;		// Label of wrapped decorator whose data are being accumulated
	const long		m_Start;			// Simulation time at which to start accumulation
	const long		m_End;				//	   "		"              end		  "

private:



};

#endif // !defined(AFX_TACUMULATEDECORATOR_H__3557FCA1_DC37_49DB_B1B2_C15722E65D14__INCLUDED_)

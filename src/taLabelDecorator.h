// taLabelDecorator.h: interface for the taLabelDecorator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TALABELDECORATOR_H__989F4A5A_AEED_4C93_8D28_998348CA7B64__INCLUDED_)
#define AFX_TALABELDECORATOR_H__989F4A5A_AEED_4C93_8D28_998348CA7B64__INCLUDED_



#include "CommandTargetNode.h"

class taLabelDecorator : public CCommandTargetNode
{
public:
	// ****************************************
	// Construction/Destruction - protected constructor declared below
public:

	virtual ~taLabelDecorator();


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


	// ****************************************
	// Public access functions
public:


	// ****************************************
	// Protected local functions
protected:

	// Protected constructor allows only derived classes to be instantiated

	taLabelDecorator(const zString label, CCommandTargetNode* const pDec);

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members

protected:

private:



};

#endif // !defined(AFX_TALABELDECORATOR_H__989F4A5A_AEED_4C93_8D28_998348CA7B64__INCLUDED_)

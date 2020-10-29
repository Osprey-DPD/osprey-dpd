// taForceDecorator.h: interface for the taForceDecorator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TAFORCEDECORATOR_H__09F78222_4EB4_4372_A2A8_5DF3E4DB913C__INCLUDED_)
#define AFX_TAFORCEDECORATOR_H__09F78222_4EB4_4372_A2A8_5DF3E4DB913C__INCLUDED_


// Include file

#include "Bead.h"


#include "CommandTargetNode.h"

class taForceDecorator : public CCommandTargetNode
{
public:
	// ****************************************
	// Construction/Destruction - protected constructor declared below
public:

	virtual ~taForceDecorator();


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

	virtual double GetXForce(long simTime) = 0;
	virtual double GetYForce(long simTime) = 0;
	virtual double GetZForce(long simTime) = 0;

	// ****************************************
	// Public access functions
public:

	void SetBeadPointer(const CBead* const pBead);


	// ****************************************
	// Protected local functions
protected:

	// Protected constructor allows only derived classes to be instantiated

	taForceDecorator(const zString label, CCommandTargetNode* const pDec);

	// Protected constructor that is used when reading an instance from a file

	taForceDecorator(const zString label);

	// ****************************************
	// Implementation

	// Functions that have access to the bead data to avoid making every
	// derived class a friend of CAbstractBead

	inline void AddBeadXForce(CBead* pBead, double fx) const {pBead->m_Force[0] += fx;}
	inline void AddBeadYForce(CBead* pBead, double fy) const {pBead->m_Force[1] += fy;}
	inline void AddBeadZForce(CBead* pBead, double fz) const {pBead->m_Force[2] += fz;}



	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members

protected:

	const CBead* m_pBead;	// Bead pointer for derived classes that need bead coordinates

private:



};

#endif // !defined(AFX_TAFORCEDECORATOR_H__09F78222_4EB4_4372_A2A8_5DF3E4DB913C__INCLUDED_)

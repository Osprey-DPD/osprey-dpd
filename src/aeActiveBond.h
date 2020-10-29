// aeActiveBond.h: interface for the aeActiveBond class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AEACTIVEBOND_H__FF5B51DE_578D_4B35_BDE0_DC971B712A4F__INCLUDED_)
#define AFX_AEACTIVEBOND_H__FF5B51DE_578D_4B35_BDE0_DC971B712A4F__INCLUDED_


// Forward declarations

class aeBondConnection;
class aeBondOnCondition;
class aeBondOffCondition;

// Include files

#include "aaVector.h"
#include "AbstractBead.h"
#include "Bond.h"
#include "BondPair.h"
#include "Polymer.h"


class aeActiveBond  
{
	// ****************************************
	// Construction/Destruction - protected constructor declared below
public:

	virtual ~aeActiveBond();

	// ****************************************
	// Global functions, static member functions and variables


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:
	// This function returns the maximum number of active bonds permitted
	// in each active polymer, and must be implemented by all derived classes
	// via a static member variable.

	virtual long    GetActiveBondsPerPolymer() const = 0;

	// These functions give access to the active bonds for the SimBox and active
	// polymer classes

	virtual bool	Activate(aeActiveBond* const pTargetBond) = 0;
	virtual bool	Deactivate()							  = 0;

	// ****************************************
	// Public access functions
public:

	inline long	GetId()							const {return GetTailMonomer()->GetId();}
	inline long GetTailMonomerType()			const {return GetTailMonomer()->GetType();}
	inline bool IsPolymerised()					const {return m_bPolymerised;}

	bool IsPhantom() const;

	long CountHeadAdjacentBonds() const;
	long CountTailAdjacentBonds() const;

	aeActiveBond* GetHeadAdjacentBond()	const;
	aeActiveBond* GetTailAdjacentBond()	const;

	aeBondConnection* GetBondConnection() const;

    aaVector GetCM() const;

	// Forwarding functions to the enclosed aeBondConnection instance

	CPolymer* GetHeadMonomer() const;
	CPolymer* GetTailMonomer() const;
	CPolymer* GetFuelMonomer() const;

	CAbstractBead*			GetHeadHeadBead()	const;
	CAbstractBead*			GetHeadTailBead()	const;
	inline CAbstractBead*   GetTailHeadBead()   const {return GetTailMonomer()->GetHead();}
	inline CAbstractBead*   GetTailTailBead()   const {return GetTailMonomer()->GetTail();}

	inline double			GetTailXPos()		const {return GetTailHeadBead()->GetXPos();}
	inline double			GetTailYPos()		const {return GetTailHeadBead()->GetYPos();}
	inline double			GetTailZPos()		const {return GetTailHeadBead()->GetZPos();}

	// Function used by aeActivePolymer to calculate the binding forces between bonds

	void AddForce();

	// Functions used by aeActivePolymer to tie two bonds together

	bool AddHeadAdjacentBond(aeActiveBond* const pTargetBond);
	bool RemoveHeadAdjacentBond();
	bool SeparateHeadAdjacentBond();
	bool AddTailAdjacentBond(aeActiveBond* const pTargetBond);
	bool RemoveTailAdjacentBond();
	void DissolveFromHead();


	// Functions implementing commands that are targetted to the active bonds

	void ReplaceConnection(aeBondConnection* const pNewConnector);
	void RestorePreviousConnection();

	void ReplaceOnCondition(aeBondOnCondition* const pNewOn);
	void ReplaceOffCondition(aeBondOffCondition* const pNewOff);

	void SetDepolymerised();
	void SetPolymerised();
    void SetPhantom(bool bPhantom);
	void SetVisible(bool bVisible);
	void SetVisible();
	void SetInvisible();

	// ****************************************
	// Protected local functions
protected:

	aeActiveBond(CPolymer* const pTail);
	aeActiveBond(CPolymer* const pTail, aeBondOnCondition* pOn, aeBondOffCondition* pOff);
	aeActiveBond(CPolymer* const pTail, aeBondConnection* pConnector, aeBondOnCondition* pOn, aeBondOffCondition* pOff);


	bool HeadOnCondition(const aeActiveBond* const pTargetBond) const;
	bool TailOnCondition(const aeActiveBond* const pTargetBond) const;

	bool HeadOffCondition() const;
	bool TailOffCondition() const;

	// Protected data members
protected:

	bool m_bPolymerised;	// Flag showing if a bond is part of an active polymer

	aeActiveBond*         m_pHeadAdjacentBond;		// Active bond attached to head
	aeActiveBond*         m_pTailAdjacentBond;		// Active bond attached to tail

	aeBondConnection*	 m_pConnector;				// Type of link between two active bonds in an active polymer

	aeBondOnCondition*   m_pOn;						// Activation condition for this bond type
	aeBondOffCondition*  m_pOff;					// Breakup condition for this bond type



	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:

	void RestorePreviousOnCondition();
	void RestorePreviousOffCondition();

	// ****************************************
	// Data members

private:

	aeBondConnection*    m_pOldConnector;	// Previous link between active bonds

	aeBondOnCondition*   m_pOldOn;		// Previous activation condition 
	aeBondOffCondition*  m_pOldOff;		// Previous breakup condition


};

#endif // !defined(AFX_AEACTIVEBOND_H__FF5B51DE_578D_4B35_BDE0_DC971B712A4F__INCLUDED_)

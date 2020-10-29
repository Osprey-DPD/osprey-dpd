// aeActivePolymer.h: interface for the aeActivePolymer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AEACTIVEPOLYMER_H__BFCC24D1_1C2E_421D_BFAA_6B3133624958__INCLUDED_)
#define AFX_AEACTIVEPOLYMER_H__BFCC24D1_1C2E_421D_BFAA_6B3133624958__INCLUDED_


// Include files

#include "xxBase.h"


class aeActivePolymer  
{
	// ****************************************
	// Construction/Destruction
public:

	// Constructor for use when creating a new filament out of two bonds

	aeActivePolymer(aeActiveBond* const pHead, aeActiveBond* const pTail);

	// Constructor for use when wrapping an existing filament by a polymer instance

	aeActivePolymer(aeActiveBond* const pTail);

	virtual ~aeActivePolymer();

	// ****************************************
	// Global functions, static member functions and variables
public:

	static long GetPolymerTotal();

	static void ZeroPolymerTotal();

private:

	static long m_PolymerTotal;

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:



	// ****************************************
	// Public access functions
public:

	inline long GetId()		 const {return m_id;}
	inline long GetSize()	 const {return m_Bonds.size();}

	long GetMaxSize() const;

	aeActiveBond*  GetHeadBond() const;
	aeActiveBond*  GetTailBond() const;
	ActiveBondList GetBonds() const {return m_Bonds;}

	void AddHeadBond(aeActiveBond* const pBond);
	void AddTailBond(aeActiveBond* const pBond);
	bool InsertBond(aeActiveBond* const pPreviousBond, aeActiveBond* const pBond);

	bool RemoveHeadBond();
	bool RemoveTailBond();

	void AddForce();

	void SetVisible(bool bVisible);
	void SetVisible();
	void SetInvisible();


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

private:

	const long m_id;		// Unique id for each active polymer

	ActiveBondList m_Bonds;	// Set of active bonds in the polymer

	aeActiveBond*  m_pHead;	// First and last active bonds in the active polymer
	aeActiveBond*  m_pTail;


};

#endif // !defined(AFX_AEACTIVEPOLYMER_H__BFCC24D1_1C2E_421D_BFAA_6B3133624958__INCLUDED_)

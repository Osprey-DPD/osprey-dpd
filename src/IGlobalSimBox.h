// IGlobalSimBox.h: interface for the IGlobalSimBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IGLOBALSIMBOX_H__C5334534_A3CC_4768_8C9A_AB6059774C0E__INCLUDED_)
#define AFX_IGLOBALSIMBOX_H__C5334534_A3CC_4768_8C9A_AB6059774C0E__INCLUDED_


// Forward declarations

class ISimBox;


#include "xxParallelBase.h"

class IGlobalSimBox : public xxParallelBase  
{
	// ****************************************
	// Construction/Destruction: protected constructor declared below
public:

	virtual ~IGlobalSimBox();

	// ****************************************
	// Public access functions forming a global interface 
	// to the ISimBox instance
public:

	bool IsDPDLG() const;   // Function showing if the lg interactions are used

	const zString GetRunId() const;

    long GetCurrentTime() const;
	long GetTotalTime() const;
	double GetStepSize() const;

	double GetSimBoxXLength() const;
	double GetSimBoxYLength() const;
	double GetSimBoxZLength() const;

	double GetHalfSimBoxXLength() const;
	double GetHalfSimBoxYLength() const;
	double GetHalfSimBoxZLength() const;

	// Functions returning information about the polymer types

    long GetBeadTotalForType(long type) const;
    long GetBondTotalForType(long type) const;
    long GetBondPairTotalForType(long type) const;
    long GetPolymerTotalForType(long type) const;
	
	
	long   GetPolymerSizeFromType(long type) const;
	double GetPolymerFractionFromType(long type) const;

    // Functions return exemplar bead, bond, bondpair and polymer types
	
	const BeadVector&		GetBeadTypes() const;
	const BondVector&		GetBondTypes() const;
	const BondPairVector&	GetBondPairTypes() const;
	const PolymerVector&	GetPolymerTypes() const;



	// ****************************************
	// Global functions, static member functions and variables
public:

	static const IGlobalSimBox*  Instance();

	// Private static data member holding a pointer to this
	// interface class
private:

	static ISimBox* m_pInstance;

	// ****************************************
	// Protected constructor to prevent external creation of this class

protected:

	IGlobalSimBox();

	IGlobalSimBox(ISimBox* pISimBox);


	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:


};

#endif // !defined(AFX_IGLOBALSIMBOX_H__C5334534_A3CC_4768_8C9A_AB6059774C0E__INCLUDED_)

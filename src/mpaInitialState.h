// mpaInitialState.h: interface for the mpaInitialState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MPAINITIALSTATE_H__F8DB2DCE_F3BF_49E4_8E92_E3DF152A5E17__INCLUDED_)
#define AFX_MPAINITIALSTATE_H__F8DB2DCE_F3BF_49E4_8E92_E3DF152A5E17__INCLUDED_




// Forward declarations

class CInitialState;


#include "xxParallelBase.h"

class mpaInitialState : public xxParallelBase
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mpaInitialState(CInitialState* const pSIS);

	mpaInitialState(const mpaInitialState& oldIS);

	virtual ~mpaInitialState();

	// ****************************************
	// Global functions, static member functions and variables

public:



	// ****************************************
	// PVFs that must be overridden by all derived classes
public:



	// ****************************************
	// Public access functions
public:

    inline double GetSimBoxXLength() const {return m_SimBoxXLength;}
    inline double GetSimBoxYLength() const {return m_SimBoxYLength;}
    inline double GetSimBoxZLength() const {return m_SimBoxZLength;}

    inline double GetSimBoxXOrigin()   const {return m_SimBoxXOrigin;}
    inline double GetSimBoxYOrigin()   const {return m_SimBoxYOrigin;}
    inline double GetSimBoxZOrigin()   const {return m_SimBoxZOrigin;}

    inline double GetSimSpaceXLength() const {return m_SimSpaceXLength;}
    inline double GetSimSpaceYLength() const {return m_SimSpaceYLength;}
    inline double GetSimSpaceZLength() const {return m_SimSpaceZLength;}

	bool CreatePolymers();

    inline long GetBeadTotal()         const {return m_BeadTotal;}
    inline long GetBondTotal()         const {return m_BondTotal;}
    inline long GetBondPairTotal()     const {return m_BondPairTotal;}
    inline long GetPolymerTotal()      const {return m_PolymerTotal;}
    inline long GetPolymerTypeTotal()  const {return m_vPolymerTypeTotal.size();}

    zLongVector GetPolymerTypeTotals()             const {return m_vPolymerTypeTotal;}
    zDoubleVector GetPolymerFractionsInProcessor() const {return m_vPolymerFractions;}


    // ****************************************
	// Protected local functions

protected:

    void CreatePolymersP0();    // Parallel versions of the CInitialState function to create required polymers

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:



	// ****************************************
	// Data members
protected:



private:

    CInitialState* const m_pSerialIS;   // Serial initial state instance

    long    m_BeadTotal;           // Total number of beads on P0
    long    m_BondTotal;           // Total number of bonds on P0
    long    m_BondPairTotal;       // Total number of bondpairs on P0
    long    m_PolymerTotal;        // Total number of polymers on P0
	
    double  m_SimBoxXLength;
    double  m_SimBoxYLength;       // Dimensions of the SimBox on each processor
    double  m_SimBoxZLength;
	
    double  m_SimBoxXOrigin;
    double  m_SimBoxYOrigin;       // Coordinates of SimBox origin in whole simulation Space
    double  m_SimBoxZOrigin;

    double  m_SimSpaceXLength;
    double  m_SimSpaceYLength;     // Dimension of the simulation Space
    double  m_SimSpaceZLength;

    zLongVector    m_vPolymerTypeTotal;  // Total number of each polymer type that must be created in whole simulation Space
    zDoubleVector  m_vPolymerFractions;  // Fraction of each polymer type's total that is owned by current processor
};

#endif // !defined(AFX_MPAINITIALSTATE_H__F8DB2DCE_F3BF_49E4_8E92_E3DF152A5E17__INCLUDED_)

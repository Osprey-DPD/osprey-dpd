// tguLatticeComponentType.h: interface for the tguLatticeComponentType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TGULATTICECOMPONENTTYPE_H__225923C7_A091_495D_B68E_67F4FC2FCDD3__INCLUDED_)
#define AFX_TGULATTICECOMPONENTTYPE_H__225923C7_A091_495D_B68E_67F4FC2FCDD3__INCLUDED_


// Forward declarations

class tguLatticeVectorType;


#include "tguArgumentType.h"

class tguLatticeComponentType : public tguArgumentType
{
    // ****************************************
	// Construction/Destruction:
public:

    tguLatticeComponentType(tguLatticeVectorType* const pLattice, long i);

    virtual ~tguLatticeComponentType();


	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:


    virtual void GetNextValue(long* pValue);
    virtual void GetNextValue(double* pValue);
    virtual void GetNextValue(zString* pValue);

	// ****************************************
	// Public access functions
public:

	virtual void GetValue(long* pValue) const;
	virtual void GetValue(double* pValue) const;
	virtual void GetValue(zString* pValue) const;

	// ****************************************
	// Protected local functions
protected:




	// ****************************************
	// Implementation
protected:


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

    tguLatticeVectorType* const m_pLattice;   // Potiner to a lattice instance

    const long  m_i;    // 0,1,2 components of lattice vector

};

#endif // !defined(AFX_TGULATTICECOMPONENTTYPE_H__225923C7_A091_495D_B68E_67F4FC2FCDD3__INCLUDED_)

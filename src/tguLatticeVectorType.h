// tguLatticeVectorType.h: interface for the tguLatticeVectorType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TGULATTICEVECTORTYPE_H__870985E9_0154_425A_BC6A_E760EF78E7C7__INCLUDED_)
#define AFX_TGULATTICEVECTORTYPE_H__870985E9_0154_425A_BC6A_E760EF78E7C7__INCLUDED_



#include "tguArgumentType.h"

class tguLatticeVectorType : public tguArgumentType
{
    // ****************************************
	// Construction/Destruction:
public:

    virtual ~tguLatticeVectorType();


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

    // Function that returns the number of lattice vectors calculated so far
    inline long   GetCounter()   const {return m_Counter;}

    // Functions that return the size of the lattice, the integer components 
    // of a vector, the actual components and the parameters defining the
    // lattice

    inline long   GetMaxIndex(long i)      const  {return m_MaxIndex[i];}
    inline long   GetIndex(long i)         const  {return m_C[i];}

    inline double GetComponent(long i)     const  {return m_V[i];}
    inline double GetOrigin(long i)        const  {return m_Origin[i];}
    inline double GetLength(long i)        const  {return m_Length[i];}
    inline double GetE1Component(long i)   const  {return m_e1[i];}
    inline double GetE2Component(long i)   const  {return m_e2[i];}
    inline double GetE3Component(long i)   const  {return m_e3[i];}

    // Function to calculate the components of the next lattice vector

	double GetNextComponent(long index);

	// ****************************************
	// Protected local functions
protected:

	// Protected constructors ensure that only subclasses can be instantiated

    tguLatticeVectorType(long xmax, long ymax, long zmax,
                         double ox, double oy, double oz,
                         double lx, double ly, double lz);


	// ****************************************
	// Implementation
protected:

    long    m_MaxIndex[3];     // Number of points in each lattice direction

    double  m_Origin[3];       // X,Y,Z coordinates of lattice origin
    double  m_Length[3];       // Lengths of lattice vectors in X,Y,Z
    double  m_e1[3];           // Unit vector of lattice
    double  m_e2[3];           // Unit vector of lattice
    double  m_e3[3];           // Unit vector of lattice

    // Local variables

    long    m_Counter;         // Count of the number of invocations so far
    long    m_C[3];            // Integer components of current lattice vector
    double  m_V[3];            // Actual components of current lattice vector

	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:


};


#endif // !defined(AFX_TGULATTICEVECTORTYPE_H__870985E9_0154_425A_BC6A_E760EF78E7C7__INCLUDED_)

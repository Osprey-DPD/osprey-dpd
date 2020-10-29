// tguTriangularLatticeType.h: interface for the tguTriangularLatticeType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TGUTRIANGULARLATTICETYPE_H__12D0EA8D_5A49_4B0C_9540_AC33BC0A2854__INCLUDED_)
#define AFX_TGUTRIANGULARLATTICETYPE_H__12D0EA8D_5A49_4B0C_9540_AC33BC0A2854__INCLUDED_



#include "tguLatticeVectorType.h"

class tguTriangularLatticeType : public tguLatticeVectorType
{
    // ****************************************
	// Construction/Destruction:
public:

	// 2d triangular lattice constructor
    tguTriangularLatticeType(long xmax, long ymax,
                              double ox, double oy,
                              double lx, double ly);
							  
	// 3d triangular lattice constructor
    tguTriangularLatticeType(long xmax, long ymax, long zmax,
                              double ox, double oy, double oz,
                              double lx, double ly, double lz);

    virtual ~tguTriangularLatticeType();


	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:


	// ****************************************
	// Public access functions
public:


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



};

#endif // !defined(AFX_TGUTRIANGULARLATTICETYPE_H__12D0EA8D_5A49_4B0C_9540_AC33BC0A2854__INCLUDED_)

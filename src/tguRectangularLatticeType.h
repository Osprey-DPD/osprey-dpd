// tguRectangularLatticeType.h: interface for the tguRectangularLatticeType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TGURECTANGULARLATTICETYPE_H__8B8A4170_BC4F_4EDB_971F_35D563AA850C__INCLUDED_)
#define AFX_TGURECTANGULARLATTICETYPE_H__8B8A4170_BC4F_4EDB_971F_35D563AA850C__INCLUDED_



#include "tguLatticeVectorType.h"

class tguRectangularLatticeType : public tguLatticeVectorType
{
    // ****************************************
	// Construction/Destruction:
public:

    // 2d lattice constructor
    tguRectangularLatticeType(long xmax, long ymax,
                              double ox, double oy,
                              double lx, double ly);

    // 3d lattice constructor
    tguRectangularLatticeType(long xmax, long ymax, long zmax,
                              double ox, double oy, double oz,
                              double lx, double ly, double lz);

    virtual ~tguRectangularLatticeType();


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

#endif // !defined(AFX_TGURECTANGULARLATTICETYPE_H__8B8A4170_BC4F_4EDB_971F_35D563AA850C__INCLUDED_)

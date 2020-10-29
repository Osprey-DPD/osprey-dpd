// mpsUnitSimBox.h: interface for the mpsUnitSimBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MPSUNITSIMBOX_H__A09D847D_59DD_4AB7_B221_B89C4AEEF1F6__INCLUDED_)
#define AFX_MPSUNITSIMBOX_H__A09D847D_59DD_4AB7_B221_B89C4AEEF1F6__INCLUDED_




#include "mpsSimBox.h"

class mpsUnitSimBox : public mpsSimBox
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mpsUnitSimBox(long lx, long ly, long lz, double dx, double dy, double dz, 
                  CNTCellVector&  rvCNTCells, PolymerVector& rvBulkPolymers);

	mpsUnitSimBox(const mpsUnitSimBox& oldSimBox);

	virtual ~mpsUnitSimBox();

	// ****************************************
	// Global functions, static member functions and variables

public:




	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

    // Function to connect up all processors in the Simulation Space - this class is special as there is only a single processor
	// that is periodically connected to itself in all directions.

	virtual void PartitionSpace();
	
	virtual void UpdateBorderPos();
	virtual void UpdateBorderForce();


	// ****************************************
	// Public access functions
public:



	// ****************************************
	// Protected local functions


	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:



	// ****************************************
	// Data members
private:


};

#endif // !defined(AFX_MPSUNITSIMBOX_H__A09D847D_59DD_4AB7_B221_B89C4AEEF1F6__INCLUDED_)

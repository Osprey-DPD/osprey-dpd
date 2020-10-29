// mpsPlanarXSimBox.h: interface for the mpsPlanarXSimBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MPSPLANARXSIMBOX_H__A5FE7EB7_3A6C_4FE2_8780_DC686B536FF6__INCLUDED_)
#define AFX_MPSPLANARXSIMBOX_H__A5FE7EB7_3A6C_4FE2_8780_DC686B536FF6__INCLUDED_




#include "mpsSimBox.h"

class mpsPlanarXSimBox : public mpsSimBox
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mpsPlanarXSimBox(long px, long py, long pz, long lx, long ly, long lz,
              double dx, double dy, double dz, CNTCellVector&  rvCNTCells,
			  PolymerVector& rvBulkPolymers);

	mpsPlanarXSimBox(const mpsPlanarXSimBox& oldSimBox);

	virtual ~mpsPlanarXSimBox();

	// ****************************************
	// Global functions, static member functions and variables

public:




	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

    // Functions to connect up all processors in the Simulation Space

	virtual void PartitionSpace();


    // Function to update bead coordinates according to the geometry of the Space

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

#endif // !defined(AFX_MPSPLANARXSIMBOX_H__A5FE7EB7_3A6C_4FE2_8780_DC686B536FF6__INCLUDED_)

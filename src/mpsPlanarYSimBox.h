// mpsPlanarYSimBox.h: interface for the mpsPlanarYSimBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MPSPLANARYSIMBOX_H__6E02A1F2_C883_4A37_8F12_60E189E94821__INCLUDED_)
#define AFX_MPSPLANARYSIMBOX_H__6E02A1F2_C883_4A37_8F12_60E189E94821__INCLUDED_




#include "mpsSimBox.h"

class mpsPlanarYSimBox : public mpsSimBox
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mpsPlanarYSimBox(long px, long py, long pz, long lx, long ly, long lz,
              double dx, double dy, double dz, CNTCellVector&  rvCNTCells,
			  PolymerVector& rvBulkPolymers);

	mpsPlanarYSimBox(const mpsPlanarYSimBox& oldSimBox);

	virtual ~mpsPlanarYSimBox();

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

#endif // !defined(AFX_MPSPLANARYSIMBOX_H__6E02A1F2_C883_4A37_8F12_60E189E94821__INCLUDED_)

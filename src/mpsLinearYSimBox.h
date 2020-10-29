// mpsLinearYSimBox.h: interface for the mpsLinearYSimBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MPSLINEARYSIMBOX_H__17AA6D2D_62CA_4C44_8B49_FA844E5AF831__INCLUDED_)
#define AFX_MPSLINEARYSIMBOX_H__17AA6D2D_62CA_4C44_8B49_FA844E5AF831__INCLUDED_




#include "mpsSimBox.h"

class mpsLinearYSimBox : public mpsSimBox
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mpsLinearYSimBox(long px, long py, long pz, long lx, long ly, long lz,
              double dx, double dy, double dz, CNTCellVector&  rvCNTCells,
			  PolymerVector& rvBulkPolymers);

	mpsLinearYSimBox(const mpsLinearYSimBox& oldSimBox);

	virtual ~mpsLinearYSimBox();

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

#endif // !defined(AFX_MPSLINEARYSIMBOX_H__17AA6D2D_62CA_4C44_8B49_FA844E5AF831__INCLUDED_)

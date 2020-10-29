// mpsLinearXSimBox.h: interface for the mpsLinearXSimBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MPSLINEARXSIMBOX_H__400B519B_5F0F_4441_AF44_9A3FC4571FD5__INCLUDED_)
#define AFX_MPSLINEARXSIMBOX_H__400B519B_5F0F_4441_AF44_9A3FC4571FD5__INCLUDED_




#include "mpsSimBox.h"

class mpsLinearXSimBox : public mpsSimBox
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mpsLinearXSimBox(long px, long py, long pz, long lx, long ly, long lz,
              double dx, double dy, double dz, CNTCellVector&  rvCNTCells,
			  PolymerVector& rvBulkPolymers);

	mpsLinearXSimBox(const mpsLinearXSimBox& oldSimBox);

	virtual ~mpsLinearXSimBox();

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

#endif // !defined(AFX_MPSLINEARXSIMBOX_H__400B519B_5F0F_4441_AF44_9A3FC4571FD5__INCLUDED_)

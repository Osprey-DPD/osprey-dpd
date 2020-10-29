// mpsLinearZSimBox.h: interface for the mpsLinearZSimBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MPSLINEARZSIMBOX_H__67980164_9DCF_4F58_A7C6_1677B13DB79A__INCLUDED_)
#define AFX_MPSLINEARZSIMBOX_H__67980164_9DCF_4F58_A7C6_1677B13DB79A__INCLUDED_



// Include files for nested message classes

#include "pmSendLinearBeadCoords.h"
#include "pmSendLinearBeadForces.h"
#include "pmSendLinearBeads.h"


#include "mpsSimBox.h"

class mpsLinearZSimBox : public mpsSimBox
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mpsLinearZSimBox(long px, long py, long pz, long lx, long ly, long lz,
              double dx, double dy, double dz, CNTCellVector&  rvCNTCells,
			  PolymerVector& rvBulkPolymers);

	mpsLinearZSimBox(const mpsLinearZSimBox& oldSimBox);

	virtual ~mpsLinearZSimBox();

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
protected:



	// ****************************************
	// Private functions
private:

	void TranslateBeadCoordsInNegativeZ();
	void TranslateBeadForcesInPositiveZ();


	// ****************************************
	// Data members
private:

    pmSendLinearBeadCoords  m_Msg1;  // Message to send/receive bead coordinates
    pmSendLinearBeadForces  m_Msg2;  // Message to send/receive bead forces
    pmSendLinearBeads       m_Msg3;  // Message to send/receive bead instances
    pmSendLinearBeads       m_Msg4;  // Message to send/receive bead instances
	
   static long  netTotal;
   static long  UFaceDifference;
   static long  DFaceDifference;


};

#endif // !defined(AFX_MPSLINEARZSIMBOX_H__67980164_9DCF_4F58_A7C6_1677B13DB79A__INCLUDED_)

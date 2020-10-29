// mpsPlanarZSimBox.h: interface for the mpsPlanarZSimBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MPSPLANARZSIMBOX_H__FC6788BB_4891_4C72_9FE7_CCACBEFDB4E6__INCLUDED_)
#define AFX_MPSPLANARZSIMBOX_H__FC6788BB_4891_4C72_9FE7_CCACBEFDB4E6__INCLUDED_



// Include files for nested message classes

#include "pmSendPlanarBeadCoords.h"
#include "pmSendPlanarEdgeBeadCoords.h"
#include "pmSendPlanarCornerBeadCoords.h"
#include "pmSendPlanarBeadForces.h"
#include "pmSendPlanarEdgeBeadForces.h"
#include "pmSendPlanarCornerBeadForces.h"
#include "pmSendPlanarBeads.h"
#include "pmSendPlanarEdgeBeads.h"
#include "pmSendCubicCornerBeads.h"


#include "mpsSimBox.h"

class mpsPlanarZSimBox : public mpsSimBox
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mpsPlanarZSimBox(long px, long py, long pz, long lx, long ly, long lz,
              double dx, double dy, double dz, CNTCellVector&  rvCNTCells,
			  PolymerVector& rvBulkPolymers);

	mpsPlanarZSimBox(const mpsPlanarZSimBox& oldSimBox);

	virtual ~mpsPlanarZSimBox();

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

	void TranslateBeadCoordsInNegativeX();
	void TranslateBeadCoordsInNegativeY();

	void TranslateBeadCoordsInNegXPosY();
	void TranslateBeadCoordsInNegXNegY();
	void TranslateBeadCoordsInNegYNegZ();
	void TranslateBeadCoordsInNegXPosZ();
	void TranslateBeadCoordsInNegYPosZ();
	void TranslateBeadCoordsInNegXNegZ();

	void TranslateBeadCoordsInNegXNegYNegZ();
	void TranslateBeadCoordsInNegXPosYPosZ();
	void TranslateBeadCoordsInNegXNegYPosZ();
	void TranslateBeadCoordsInNegXPosYNegZ();


	void TranslateBeadForcesInPositiveX();
	void TranslateBeadForcesInPositiveY();

	void TranslateBeadForcesInPosXNegY();
	void TranslateBeadForcesInPosXPosY();
	void TranslateBeadForcesInPosYPosZ();
	void TranslateBeadForcesInPosXNegZ();
	void TranslateBeadForcesInPosYNegZ();
	void TranslateBeadForcesInPosXPosZ();

	void TranslateBeadForcesInPosXPosYPosZ();
	void TranslateBeadForcesInPosXNegYNegZ();
	void TranslateBeadForcesInPosXPosYNegZ();
	void TranslateBeadForcesInPosXNegYPosZ();


	// ****************************************
	// Data members
private:

    pmSendPlanarBeadCoords        m_Msg1L;   // Message to send/receive bead coordinates
    pmSendPlanarBeadCoords        m_Msg1B;  

    pmSendPlanarEdgeBeadCoords    m_Msg1TL;  // Message to send/receive coords to a diagonal processor
    pmSendPlanarEdgeBeadCoords    m_Msg1BL;  
    pmSendPlanarEdgeBeadCoords    m_Msg1DB;  
    pmSendPlanarEdgeBeadCoords    m_Msg1UL;  
    pmSendPlanarEdgeBeadCoords    m_Msg1UB;  
    pmSendPlanarEdgeBeadCoords    m_Msg1DL; 

    pmSendPlanarCornerBeadCoords  m_Msg1DBL;  // Message to send/receive coords to a corner processor
    pmSendPlanarCornerBeadCoords  m_Msg1UTL;  
    pmSendPlanarCornerBeadCoords  m_Msg1UBL; 
    pmSendPlanarCornerBeadCoords  m_Msg1DTL; 

    pmSendPlanarBeadForces        m_Msg2R;   // Message to send/receive bead forces
    pmSendPlanarBeadForces        m_Msg2T;  

    pmSendPlanarEdgeBeadForces    m_Msg2BR;  // Message to send/receive forces to a diagonal processor
    pmSendPlanarEdgeBeadForces    m_Msg2TR;  
    pmSendPlanarEdgeBeadForces    m_Msg2UT;  
    pmSendPlanarEdgeBeadForces    m_Msg2DR;  
    pmSendPlanarEdgeBeadForces    m_Msg2DT;  
    pmSendPlanarEdgeBeadForces    m_Msg2UR;  

    pmSendPlanarCornerBeadForces  m_Msg2UTR;  
    pmSendPlanarCornerBeadForces  m_Msg2DBR;  
    pmSendPlanarCornerBeadForces  m_Msg2DTR;  
    pmSendPlanarCornerBeadForces  m_Msg2UBR;  

    pmSendPlanarBeads             m_Msg3R;  // Message to send/receive bead instances
    pmSendPlanarBeads             m_Msg3T;  
    pmSendPlanarBeads             m_Msg4R;  
    pmSendPlanarBeads             m_Msg4T;  

    pmSendPlanarEdgeBeads         m_Msg3TR;  
    pmSendPlanarEdgeBeads         m_Msg3TL;  
    pmSendPlanarEdgeBeads         m_Msg4TR;  
    pmSendPlanarEdgeBeads         m_Msg4TL;  

    pmSendPlanarEdgeBeads         m_Msg3UT; 	   // Beads moving updwards through an edge
    pmSendPlanarEdgeBeads         m_Msg3UB; 	
    pmSendPlanarEdgeBeads         m_Msg3UR; 	
    pmSendPlanarEdgeBeads         m_Msg3UL; 	
    pmSendPlanarEdgeBeads         m_Msg4UT; 	   // Beads moving downwards through an edge
    pmSendPlanarEdgeBeads         m_Msg4UB; 	
    pmSendPlanarEdgeBeads         m_Msg4UR; 	
    pmSendPlanarEdgeBeads         m_Msg4UL; 	
	
    pmSendCubicCornerBeads        m_Msg3UTR;    // Beads moving upward through a corner - currently using the cubic space message class
    pmSendCubicCornerBeads        m_Msg3UTL; 
    pmSendCubicCornerBeads        m_Msg3UBR; 
    pmSendCubicCornerBeads        m_Msg3UBL; 
    pmSendCubicCornerBeads        m_Msg4UTR;    
    pmSendCubicCornerBeads        m_Msg4UTL; 
    pmSendCubicCornerBeads        m_Msg4UBR; 
    pmSendCubicCornerBeads        m_Msg4UBL; 


   static long  netTotal;
   static long  RFaceDifference;
   static long  LFaceDifference;
   static long  TFaceDifference;
   static long  BFaceDifference;
   static long  TREdgeDifference;
   static long  BLEdgeDifference;
   static long  TLEdgeDifference;
   static long  BREdgeDifference;
   
   // Debug counters for bead coordinate transfers
   
   long m_LCoordCounter;
   long m_BCoordCounter;
   
   long m_RForceCounter;
   long m_TForceCounter;
   
   
   
   long  m_UTLCoordCounter;
   long  m_DTLCoordCounter;
   long  m_DBRForceCounter;
   long  m_UBRForceCounter;
   
   

};

#endif // !defined(AFX_MPSPLANARZSIMBOX_H__FC6788BB_4891_4C72_9FE7_CCACBEFDB4E6__INCLUDED_)

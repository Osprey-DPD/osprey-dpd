// mpsCubicSimBox.h: interface for the mpsCubicSimBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MPSCUBICSIMBOX_H__74504D0D_8481_42AB_ABBB_A1C0D86550F2__INCLUDED_)
#define AFX_MPSCUBICSIMBOX_H__74504D0D_8481_42AB_ABBB_A1C0D86550F2__INCLUDED_



// Include files for nested message classes

#include "pmSendCubicBeadCoords.h"
#include "pmSendCubicEdgeBeadCoords.h"
#include "pmSendCubicCornerBeadCoords.h"
#include "pmSendCubicBeadForces.h"
#include "pmSendCubicEdgeBeadForces.h"
#include "pmSendCubicCornerBeadForces.h"
#include "pmSendCubicBeads.h"
#include "pmSendCubicEdgeBeads.h"
#include "pmSendCubicCornerBeads.h"
#include "pmExtendedBondForceCount.h"
#include "pmExtendedPolymerProcessorCount.h"

#include "mpsSimBox.h"

class mpsCubicSimBox : public mpsSimBox
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mpsCubicSimBox(long px, long py, long pz, long lx, long ly, long lz,
              double dx, double dy, double dz, CNTCellVector&  rvCNTCells,
			  PolymerVector& rvBulkPolymers);

	mpsCubicSimBox(const mpsCubicSimBox& oldSimBox);

	virtual ~mpsCubicSimBox();

	// ****************************************
	// Global functions, static member functions and variables

public:



	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

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

	void TranslateBeadCoordsInNegativeX();
	void TranslateBeadCoordsInNegativeY();
	void TranslateBeadCoordsInNegativeZ();

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
	void TranslateBeadForcesInPositiveZ();

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

    // Force calculation messages
    pmSendCubicBeadCoords            m_Msg1D;    // Messages to send/receive bead coords during force calculation
    pmSendCubicBeadCoords            m_Msg1B; 	
    pmSendCubicBeadCoords            m_Msg1L; 
	
    pmSendCubicEdgeBeadCoords        m_Msg1TL; 	
    pmSendCubicEdgeBeadCoords        m_Msg1BL; 	
    pmSendCubicEdgeBeadCoords        m_Msg1DB;  
    pmSendCubicEdgeBeadCoords        m_Msg1UL;  
    pmSendCubicEdgeBeadCoords        m_Msg1UB;  
    pmSendCubicEdgeBeadCoords        m_Msg1DL; 
	
    pmSendCubicCornerBeadCoords      m_Msg1DBL; 	
    pmSendCubicCornerBeadCoords      m_Msg1UTL; 	
    pmSendCubicCornerBeadCoords      m_Msg1UBL; 	
    pmSendCubicCornerBeadCoords      m_Msg1DTL; 	
	
    pmSendCubicBeadForces            m_Msg2U;    // Messages to send/receive bead forces during force calculation
	pmSendCubicBeadForces            m_Msg2T;
    pmSendCubicBeadForces            m_Msg2R;
	
    pmSendCubicEdgeBeadForces        m_Msg2BR;
    pmSendCubicEdgeBeadForces        m_Msg2TR;
    pmSendCubicEdgeBeadForces        m_Msg2UT;
    pmSendCubicEdgeBeadForces        m_Msg2DR;
    pmSendCubicEdgeBeadForces        m_Msg2DT;
    pmSendCubicEdgeBeadForces        m_Msg2UR;
	
	pmSendCubicCornerBeadForces      m_Msg2UTR;
	pmSendCubicCornerBeadForces      m_Msg2DBR;
	pmSendCubicCornerBeadForces      m_Msg2DTR;
	pmSendCubicCornerBeadForces      m_Msg2UBR;
	
	// Messages for beads moving between processors
    pmSendCubicBeads                 m_Msg3U;      // Messages to send/receive bead instances in +U, +T, +R directions
    pmSendCubicBeads                 m_Msg3T;
    pmSendCubicBeads                 m_Msg3R; 	
    pmSendCubicBeads                 m_Msg4U;      // Messages to send/receive bead instances in -U, -T, -R directions, ie, D,B,L
    pmSendCubicBeads                 m_Msg4T;      // but we represent these by the number 4 instead
    pmSendCubicBeads                 m_Msg4R; 	
	
    pmSendCubicEdgeBeads             m_Msg3TR;     // In-plane edge bead messages	
    pmSendCubicEdgeBeads             m_Msg3TL; 	
    pmSendCubicEdgeBeads             m_Msg4TR; 	
    pmSendCubicEdgeBeads             m_Msg4TL; 	
	
    pmSendCubicEdgeBeads             m_Msg3UT; 	   // Beads moving updwards through an edge
    pmSendCubicEdgeBeads             m_Msg3UB; 	
    pmSendCubicEdgeBeads             m_Msg3UR; 	
    pmSendCubicEdgeBeads             m_Msg3UL; 	
    pmSendCubicEdgeBeads             m_Msg4UT; 	   // Beads moving downwards through an edge
    pmSendCubicEdgeBeads             m_Msg4UB; 	
    pmSendCubicEdgeBeads             m_Msg4UR; 	
    pmSendCubicEdgeBeads             m_Msg4UL; 	
	
    pmSendCubicCornerBeads           m_Msg3UTR;    // Beads moving upward through a corner
    pmSendCubicCornerBeads           m_Msg3UTL; 
    pmSendCubicCornerBeads           m_Msg3UBR; 
    pmSendCubicCornerBeads           m_Msg3UBL; 
    pmSendCubicCornerBeads           m_Msg4UTR;    
    pmSendCubicCornerBeads           m_Msg4UTL; 
    pmSendCubicCornerBeads           m_Msg4UBR; 
    pmSendCubicCornerBeads           m_Msg4UBL; 
	
	
	pmExtendedBondForceCount         m_MsgExtForceCounter;     // Debug message to count ext bond force calculations on all processors
	pmExtendedPolymerProcessorCount  m_MsgExtPolyProcCounter;  // Debug message to display all extended polymers owned by 3 or more processors
	
	
   static long  netTotal;
   static long  UFaceDifference;
   static long  DFaceDifference;
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
   long m_DCoordCounter;
   
   long m_RForceCounter;
   long m_TForceCounter;
   long m_UForceCounter;
   
   
   
   long  m_UTLCoordCounter;
   long  m_DTLCoordCounter;
   long  m_DBRForceCounter;
   long  m_UBRForceCounter;
   
		
			
					
};

#endif // !defined(AFX_MPSCUBICSIMBOX_H__74504D0D_8481_42AB_ABBB_A1C0D86550F2__INCLUDED_)

// SimBox.h: interface for the CSimBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMBOX_H__05CAAA00_2EE6_11D3_820E_0060088AD300__INCLUDED_)
#define AFX_SIMBOX_H__05CAAA00_2EE6_11D3_820E_0060088AD300__INCLUDED_


#include "SimACNFlags.h"
#include "SimCommandFlags.h"
#include "SimMiscellaneousFlags.h"
#include "SimMPSFlags.h"
#include "SimProcessFlags.h"

// Forward declarations

class CNanoparticle;


#if EnableParallelSimBox == SimMPSEnabled
// Forward declarations and parallel include files
class mpsSimBox;

#endif

#if EnableShadowSimBox == SimACNEnabled
// Foward declarations and ACN include files

class aeActiveSimBox;

    #include "ccAddArp23BondImpl.h"
    #include "ccAddCofilinBondImpl.h"
    #include "ccAddfActinBondImpl.h"
    #include "ccAddfActinCPBondImpl.h"
    #include "ccAddForminBondImpl.h"
    #include "ccAddProfilinBondImpl.h"
    #include "ccAddReceptorBondImpl.h"
    #include "ccCreateActiveSimBoxImpl.h"
    #include "ccDeleteActiveSimBoxImpl.h"
    #include "ccRemoveArp23BondImpl.h"
    #include "ccRemoveCofilinBondImpl.h"
    #include "ccRemovefActinBondImpl.h"
    #include "ccRemovefActinCPBondImpl.h"
    #include "ccRemoveForminBondImpl.h"
    #include "ccRemoveProfilinBondImpl.h"
    #include "ccRemoveReceptorBondImpl.h"
    #include "ccSelectACNAsTargetImpl.h"
    #include "ccSelectACNCurrentBondsAsTargetImpl.h"
    #include "ccSelectACNCurrentPolymersAsTargetImpl.h"
    #include "ccSelectACNFreeBondsAsTargetImpl.h"
    #include "ccSelectACNFreePolymersAsTargetImpl.h"
    #include "ctBinEventSuccessIntervalsImpl.h"
    #include "ctCreateBondBindsForwardConeToPolymerHeadEventSourceImpl.h"
    #include "ctCreateBondBindsForwardConeToPolymerTailEventSourceImpl.h"
    #include "ctSerialisePolymerHeadCoordinatesImpl.h"
    #include "ctSerialisePolymerTailCoordinatesImpl.h"
#endif

#if EnableCommandDocumentProcess == SimProcessEnabled
// Command document process
  #include "ccStartCommandDocumentProcessImpl.h"
#endif

#if EnableCommandGroups == SimCommandEnabled
// Commands directed to commands in groups
    #include "cgcSetArgumentToArgumentImpl.h"
    #include "cgcSetArgumentToIntegerConstantImpl.h"
    #include "cgcSetArgumentToIntegerSequenceImpl.h"
    #include "cgcSetArgumentToRealConstantImpl.h"
    #include "cgcSetArgumentToRealSequenceImpl.h"
    #include "cgcSetArgumentToStringConstantImpl.h"
    #include "cgcSetArgumentToStringSequenceImpl.h"
    #include "cgcSetArgumentsTo2dRectangularLatticeVectorImpl.h"
    #include "cgcSetArgumentsTo2dTriangularLatticeVectorImpl.h"
    #include "cgcSetArgumentsTo3dRectangularLatticeVectorImpl.h"
// Commands directed to the command groups
    #include "cgtAddCommandToGroupImpl.h"
    #include "cgtCreateCommandGroupImpl.h"
    #include "cgtDeleteCommandGroupImpl.h"
    #include "cgtDeleteExecuteCommandGroupSequenceImpl.h"
    #include "cgtExecuteCommandGroupOnceImpl.h"
    #include "cgtExecuteCommandGroupSequenceImpl.h"
    #include "cgtToggleAllCommandExecutionInGroupImpl.h"
    #include "cgtToggleCommandExecutionInGroupImpl.h"
#endif

#if EnableTargetCommand == SimCommandEnabled
// Command targets and associated commands

    #include "ccCreateNanoparticlePentagonImpl.h"
    #include "ccCreateNanoparticleSphereImpl.h"
    #include "ccSelectBeadTypeInCylinderImpl.h"
    #include "ccSelectBeadTypeInEllipsoidImpl.h"
    #include "ccSelectBeadTypeInPentagonImpl.h"
    #include "ccSelectBeadTypeInSimBoxImpl.h"
    #include "ccSelectBeadTypeInSliceImpl.h"
    #include "ccSelectBeadTypeInSphereImpl.h"
    #include "ccSelectBeadTypeInSphericalCapImpl.h"
    #include "ccSelectPolymerTypeHeadInCylinderImpl.h"
    #include "ccSelectPolymerTypeHeadInEllipsoidImpl.h"
    #include "ccSelectPolymerTypeHeadInPentagonImpl.h"
    #include "ccSelectPolymerTypeHeadInSliceImpl.h"
    #include "ccSelectPolymerTypeHeadInSphereImpl.h"
    #include "ccSelectPolymerTypeHeadInSphericalCapImpl.h"
    #include "ccSelectPolymerTypeInSimBoxImpl.h"
    #include "ctAnchorForceOnTargetImpl.h"
    #include "ctAxialForceOnTargetImpl.h"
    #include "ctChangeBeadTypeImpl.h"
    #include "ctChangeBondTypeImpl.h"
    #include "ctChangeBondPairTypeImpl.h"
    #include "ctChangeNamedBeadTypeImpl.h"
    #include "ctChangeNamedBeadTypeInCompositeTargetImpl.h"
    #include "ctConstantSpecificAreaGhostImpl.h"
    #include "ctCylinderLinearForceOnTargetImpl.h"
    #include "ctCylindricalDistributionOfCompositeTargetImpl.h"
    #include "ctCylindricalDistributionOfPolymerTargetImpl.h"
    #include "ctDistanceMovedByTargetImpl.h"
    #include "ctEdgeClampForceOnTargetImpl.h"
    #include "ctExtractBeadTypesIntoCompositeTargetImpl.h"
    #include "ctExtractBeadTypesIntoTargetsImpl.h"
    #include "ctExternalWorkOnTargetImpl.h"
    #include "ctListActiveCommandTargetsImpl.h"
    #include "ctListAllCommandTargetActivitiesImpl.h"
    #include "ctListCommandTargetActivitiesImpl.h"
    #include "ctListCommandTargetsImpl.h"
    #include "ctMSDOfBeadTargetImpl.h"
    #include "ctMSDOfPolymerTargetImpl.h"
    #include "ctPlanarAnchorForceOnTargetImpl.h"
    #include "ctPolymerisePolymersInTargetImpl.h"
    #include "ctPolymerisePolymerTripleInTargetImpl.h"
    #include "ctReflectingBarrierOnTargetImpl.h"
    #include "ctRgOfBeadTargetImpl.h"
    #include "ctRgOfPolymerTargetImpl.h"
    #include "ctRotationalMSDOfPolymerTargetImpl.h"
    #include "ctSelectBeadsInSphericalTargetRegionImpl.h"
    #include "ctSetTargetBeadTypeDisplayIdImpl.h"
    #include "ctSetTargetDisplayIdImpl.h"
    #include "ctSetTargetPolymerTypeDisplayIdImpl.h"
    #include "ctSetTargetInvisibleImpl.h"
    #include "ctSetTargetVisibleImpl.h"
    #include "ctSpringForceBetweenTargetsImpl.h"
    #include "ctSpringForceOnTargetImpl.h"
    #include "ctToggleAntiTargetDisplayImpl.h"
    #include "ctToggleTargetDisplayImpl.h"
    #include "ctTranslateTargetToC1PointImpl.h"
    #include "ctTranslateTargetToC2PointImpl.h"
    #include "ctTranslateTargetToPlaneImpl.h"
    #include "ctTranslateTargetToZPlaneImpl.h"
    #include "ctWorkDoneOnBeadTypeInTargetImpl.h"
#endif

#if EnableConstraintCommand == SimCommandEnabled
// Various constraint commands
    #include "ccChargeBeadByTypeImpl.h"
    #include "ccUnchargeBeadByTypeImpl.h"
#endif

#if EnableDirectImplCommand == SimCommandDisabled
    #include "ccStopImpl.h"
    #include "ccStopNoSaveImpl.h"
    #include "ccToggleDPDBeadConservativeForcesImpl.h"
    #include "ccToggleDPDBeadForcesImpl.h"
    #include "ccToggleDPDBeadThermostatImpl.h"
#endif

#include "ISimState.h"

class CSimBox : public ISimState,
 
#if EnableShadowSimBox == SimACNEnabled
                                  public ccAddArp23BondImpl, 
								  public ccAddCofilinBondImpl, 
								  public ccAddfActinBondImpl, 
								  public ccAddfActinCPBondImpl, 
								  public ccAddForminBondImpl, 
								  public ccAddProfilinBondImpl, 
								  public ccAddReceptorBondImpl, 
								  public ccCreateActiveSimBoxImpl, 
								  public ccDeleteActiveSimBoxImpl, 
								  public ccRemoveArp23BondImpl,
								  public ccRemoveCofilinBondImpl,
								  public ccRemovefActinBondImpl,
								  public ccRemovefActinCPBondImpl,
								  public ccRemoveForminBondImpl,
								  public ccRemoveProfilinBondImpl,
								  public ccRemoveReceptorBondImpl,
								  public ccSelectACNAsTargetImpl,
								  public ccSelectACNCurrentBondsAsTargetImpl,
								  public ccSelectACNCurrentPolymersAsTargetImpl,
								  public ccSelectACNFreeBondsAsTargetImpl,
								  public ccSelectACNFreePolymersAsTargetImpl,
								  public ctBinEventSuccessIntervalsImpl,
								  public ctCreateBondBindsForwardConeToPolymerHeadEventSourceImpl,
								  public ctCreateBondBindsForwardConeToPolymerTailEventSourceImpl,
								  public ctSerialisePolymerHeadCoordinatesImpl,
								  public ctSerialisePolymerTailCoordinatesImpl,
#endif
#if EnableCommandDocumentProcess == SimProcessEnabled
								  public ccStartCommandDocumentProcessImpl,
#endif
#if EnableCommandGroups == SimCommandEnabled
                                  public cgcSetArgumentToArgumentImpl,
                                  public cgcSetArgumentToIntegerConstantImpl,
                                  public cgcSetArgumentToIntegerSequenceImpl,
                                  public cgcSetArgumentToRealConstantImpl,
                                  public cgcSetArgumentToRealSequenceImpl,
                                  public cgcSetArgumentToStringConstantImpl,
                                  public cgcSetArgumentToStringSequenceImpl,
                                  public cgcSetArgumentsTo2dRectangularLatticeVectorImpl,
                                  public cgcSetArgumentsTo2dTriangularLatticeVectorImpl,
                                  public cgcSetArgumentsTo3dRectangularLatticeVectorImpl,
                                  public cgtAddCommandToGroupImpl,
                                  public cgtCreateCommandGroupImpl,
                                  public cgtDeleteCommandGroupImpl,
                                  public cgtDeleteExecuteCommandGroupSequenceImpl,
                                  public cgtExecuteCommandGroupOnceImpl,
                                  public cgtExecuteCommandGroupSequenceImpl,
                                  public cgtToggleAllCommandExecutionInGroupImpl,
                                  public cgtToggleCommandExecutionInGroupImpl,
#endif
#if EnableTargetCommand == SimCommandEnabled
                                  public ccCreateNanoparticlePentagonImpl,
                                  public ccCreateNanoparticleSphereImpl,
                                  public ccSelectBeadTypeInCylinderImpl,
                                  public ccSelectBeadTypeInEllipsoidImpl,
                                  public ccSelectBeadTypeInPentagonImpl,
                                  public ccSelectBeadTypeInSimBoxImpl,
                                  public ccSelectBeadTypeInSliceImpl,
                                  public ccSelectBeadTypeInSphereImpl,
                                  public ccSelectBeadTypeInSphericalCapImpl,
								  public ccSelectPolymerTypeHeadInCylinderImpl,
								  public ccSelectPolymerTypeHeadInEllipsoidImpl,
								  public ccSelectPolymerTypeHeadInPentagonImpl,
								  public ccSelectPolymerTypeHeadInSliceImpl,
								  public ccSelectPolymerTypeInSimBoxImpl,
								  public ccSelectPolymerTypeHeadInSphereImpl,
								  public ccSelectPolymerTypeHeadInSphericalCapImpl,
								  public ctAnchorForceOnTargetImpl,
								  public ctAxialForceOnTargetImpl,
								  public ctChangeBeadTypeImpl,
								  public ctChangeBondTypeImpl,
								  public ctChangeBondPairTypeImpl,
								  public ctChangeNamedBeadTypeImpl,
								  public ctChangeNamedBeadTypeInCompositeTargetImpl,
								  public ctConstantSpecificAreaGhostImpl,
								  public ctCylinderLinearForceOnTargetImpl,
                                  public ctCylindricalDistributionOfCompositeTargetImpl,
                                  public ctCylindricalDistributionOfPolymerTargetImpl,
								  public ctDistanceMovedByTargetImpl,
								  public ctEdgeClampForceOnTargetImpl,
								  public ctExternalWorkOnTargetImpl,
								  public ctExtractBeadTypesIntoCompositeTargetImpl,
								  public ctExtractBeadTypesIntoTargetsImpl,
								  public ctListActiveCommandTargetsImpl,
								  public ctListAllCommandTargetActivitiesImpl,
								  public ctListCommandTargetActivitiesImpl,
								  public ctListCommandTargetsImpl,
                                  public ctMSDOfBeadTargetImpl,
                                  public ctMSDOfPolymerTargetImpl,
								  public ctPlanarAnchorForceOnTargetImpl,
								  public ctPolymerisePolymersInTargetImpl,
								  public ctPolymerisePolymerTripleInTargetImpl,
                                  public ctReflectingBarrierOnTargetImpl,
                                  public ctRgOfBeadTargetImpl,
                                  public ctRgOfPolymerTargetImpl,
								  public ctRotationalMSDOfPolymerTargetImpl,
								  public ctSelectBeadsInSphericalTargetRegionImpl,
								  public ctSetTargetBeadTypeDisplayIdImpl,
								  public ctSetTargetDisplayIdImpl,
								  public ctSetTargetPolymerTypeDisplayIdImpl,
								  public ctSetTargetInvisibleImpl,
								  public ctSetTargetVisibleImpl,
                                  public ctSpringForceBetweenTargetsImpl,
								  public ctSpringForceOnTargetImpl,
								  public ctToggleAntiTargetDisplayImpl,
								  public ctToggleTargetDisplayImpl,
								  public ctTranslateTargetToC1PointImpl,
								  public ctTranslateTargetToC2PointImpl,
								  public ctTranslateTargetToPlaneImpl,
								  public ctTranslateTargetToZPlaneImpl,
								  public ctWorkDoneOnBeadTypeInTargetImpl,
#endif
#if EnableConstraintCommand == SimCommandEnabled
                                  public ccChargeBeadByTypeImpl, 
								  public ccUnchargeBeadByTypeImpl,
#endif
#if EnableDirectImplCommand == SimCommandDisabled
// If this block is not compiled in, the comma above must be removed.
                                  public ccStopImpl,
								  public ccStopNoSaveImpl,
								  public ccToggleDPDBeadConservativeForcesImpl,
								  public ccToggleDPDBeadForcesImpl,
								  public ccToggleDPDBeadThermostatImpl
#endif
{
#if EnableShadowSimBox == SimACNEnabled
	friend class ccAddArp23BondImpl;
	friend class ccAddCofilinBondImpl;
	friend class ccAddfActinBondImpl;
	friend class ccAddfActinCPBondImpl;
	friend class ccAddForminBondImpl;
	friend class ccAddProfilinBondImpl;
	friend class ccAddReceptorBondImpl;
	friend class ccCreateActiveSimBoxImpl;
	friend class ccDeleteActiveSimBoxImpl;
	friend class ccRemoveArp23BondImpl;
	friend class ccRemoveCofilinBondImpl;
	friend class ccRemovefActinBondImpl;
	friend class ccRemovefActinCPBondImpl;
	friend class ccRemoveForminBondImpl;
	friend class ccRemoveProfilinBondImpl;
	friend class ccRemoveReceptorBondImpl;
	friend class ccSelectACNAsTargetImpl;
	friend class ccSelectACNCurrentBondsAsTargetImpl;
	friend class ccSelectACNCurrentPolymersAsTargetImpl;
	friend class ccSelectACNFreeBondsAsTargetImpl;
	friend class ccSelectACNFreePolymersAsTargetImpl;
	friend class ctBinEventSuccessIntervalsImpl;
	friend class ctCreateBondBindsForwardConeToPolymerHeadEventSourceImpl;
	friend class ctCreateBondBindsForwardConeToPolymerTailEventSourceImpl;
	friend class ctSerialisePolymerHeadCoordinatesImpl;
	friend class ctSerialisePolymerTailCoordinatesImpl;
#endif
#if EnableCommandDocumentProcess == SimProcessEnabled
	friend class ccStartCommandDocumentProcessImpl;
#endif
#if EnableCommandGroups == SimCommandEnabled
	friend class cgcSetArgumentToArgumentImpl;
	friend class cgcSetArgumentToIntegerConstantImpl;
	friend class cgcSetArgumentToIntegerSequenceImpl;
	friend class cgcSetArgumentToRealConstantImpl;
	friend class cgcSetArgumentToRealSequenceImpl;
	friend class cgcSetArgumentToStringConstantImpl;
	friend class cgcSetArgumentToStringSequenceImpl;
	friend class cgcSetArgumentsTo2dRectangularLatticeVectorImpl;
	friend class cgcSetArgumentsTo2dTriangularLatticeVectorImpl;
	friend class cgcSetArgumentsTo3dRectangularLatticeVectorImpl;
	friend class cgtAddCommandToGroupImpl;
	friend class cgtCreateCommandGroupImpl;
	friend class cgtDeleteCommandGroupImpl;
	friend class cgtDeleteExecuteCommandGroupSequenceImpl;
	friend class cgtExecuteCommandGroupOnceImpl;
	friend class cgtExecuteCommandGroupSequenceImpl;
	friend class cgtToggleAllCommandExecutionInGroupImpl;
	friend class cgtToggleCommandExecutionInGroupImpl;
#endif
#if EnableTargetCommand == SimCommandEnabled
	friend class ccCreateNanoparticlePentagonImpl;
	friend class ccCreateNanoparticleSphereImpl;
	friend class ccSelectBeadTypeInCylinderImpl;
	friend class ccSelectBeadTypeInEllipsoidImpl;
	friend class ccSelectBeadTypeInPentagonImpl;
	friend class ccSelectBeadTypeInSimBoxImpl;
	friend class ccSelectBeadTypeInSliceImpl;
	friend class ccSelectBeadTypeInSphereImpl;
	friend class ccSelectBeadTypeInSphericalCapImpl;
	friend class ccSelectPolymerTypeHeadInCylinderImpl;
	friend class ccSelectPolymerTypeHeadInEllipsoidImpl;
	friend class ccSelectPolymerTypeHeadInPentagonImpl;
	friend class ccSelectPolymerTypeHeadInSliceImpl;
	friend class ccSelectPolymerTypeHeadInSphereImpl;
	friend class ccSelectPolymerTypeHeadInSphericalCapImpl;
	friend class ccSelectPolymerTypeInSimBoxImpl;
	friend class ctAnchorForceOnTargetImpl;
	friend class ctAxialForceOnTargetImpl;
	friend class ctChangeBeadTypeImpl;
	friend class ctChangeBondTypeImpl;
	friend class ctChangeBondPairTypeImpl;
	friend class ctChangeNamedBeadTypeImpl;
	friend class ctChangeNamedBeadTypeInCompositeTargetImpl;
	friend class ctConstantSpecificAreaGhostImpl;
	friend class ctCylinderLinearForceOnTargetImpl;
    friend class ctCylindricalDistributionOfCompositeTargetImpl;
    friend class ctCylindricalDistributionOfPolymerTargetImpl;
	friend class ctDistanceMovedByTargetImpl;
	friend class ctEdgeClampForceOnTargetImpl;
	friend class ctExternalWorkOnTargetImpl;
	friend class ctExtractBeadTypesIntoCompositeTargetImpl;
	friend class ctExtractBeadTypesIntoTargetsImpl;
	friend class ctListActiveCommandTargetsImpl;
	friend class ctListAllCommandTargetActivitiesImpl;
	friend class ctListCommandTargetActivitiesImpl;
	friend class ctListCommandTargetsImpl;
    friend class ctMSDOfBeadTargetImpl;
    friend class ctMSDOfPolymerTargetImpl;
	friend class ctPlanarAnchorForceOnTargetImpl;
	friend class ctPolymerisePolymersInTargetImpl;
	friend class ctPolymerisePolymerTripleInTargetImpl;
    friend class ctRgOfBeadTargetImpl;
    friend class ctReflectingBarrierOnTargetImpl;
    friend class ctRgOfPolymerTargetImpl;
	friend class ctRotationalMSDOfPolymerTargetImpl;
	friend class ctSelectBeadsInSphericalTargetRegionImpl;
	friend class ctSetTargetBeadTypeDisplayIdImpl;
	friend class ctSetTargetDisplayIdImpl;
	friend class ctSetTargetPolymerTypeDisplayIdImpl;
	friend class ctSetTargetInvisibleImpl;
	friend class ctSetTargetVisibleImpl;
	friend class ctSpringForceBetweenTargetsImpl;
	friend class ctSpringForceOnTargetImpl;
	friend class ctToggleAntiTargetDisplayImpl;
	friend class ctToggleTargetDisplayImpl;
	friend class ctTranslateTargetToC1PointImpl;
	friend class ctTranslateTargetToC2PointImpl;
	friend class ctTranslateTargetToPlaneImpl;
	friend class ctTranslateTargetToZPlaneImpl;
	friend class ctWorkDoneOnBeadTypeInTargetImpl;
#endif
#if EnableConstraintCommand == SimCommandEnabled
    friend class ccChargeBeadByTypeImpl;
	friend class ccUnchargeBeadByTypeImpl;
#endif
#if EnableDirectImplCommand == SimCommandDisabled
    friend class ccStopImpl;
	friend class ccStopNoSaveImpl;
	friend class ccToggleDPDBeadConservativeForcesImpl;
	friend class ccToggleDPDBeadForcesImpl;
	friend class ccToggleDPDBeadThermostatImpl;
#endif

public:

	virtual ~CSimBox();

	// Function to run the simulation proper

	void Run();		

	// Function to relax the initial bead coordinates before starting an MD simulation

	void RunMCRelaxation();	
		
	//Functions used to analyse the stress tensor in curvilinear coordinates
	inline long GetStressCellMultiplier() const {return m_StressCellMultiplier;}
	inline long GetStressCellRatio()      const {return m_StressCellMultiplier*m_StressCellMultiplier*m_StressCellMultiplier;}
	inline long GetStressCellXNo()        const {return m_StressCellXNo;}
	inline long GetStressCellYNo()        const {return m_StressCellYNo;}
	inline long GetStressCellZNo()        const {return m_StressCellZNo;}
	inline long GetStressCellTotal()      const {return m_StressCellTotal;}
	inline StressCellVector&  GetStressCells()  {return m_vStressCells;}
	
	void AddWeightedStressToLinearCells(double rp[3], double rq[3], double stress[9]);
	void AddBondStressSphere(const CBond *const pBond);                // Function to add bond stresses to the CNT cell's local stress tensor
	void AddBondPairStressSphere(const CBondPair *const pBondPair);    // Function to add bond stresses to the CNT cell's local stress tensor
    void SetStressSphereOrigin(double rcm[3]);                         // Function to set the origin for the stress tensor transformation
	void TransformStressSphere(double stress[9]);                      // Function to transform the stress tensor at each CNT cell into curvilinear coordinates

#if EnableShadowSimBox == SimACNEnabled
	// Access functions for shadow SimBox that is to be accessed by other classes.

	aeActiveSimBox* GetActiveSimBox() const;
#endif

#if EnableParallelSimBox == SimMPSEnabled
	// Access functions for parallel SimBox that is to be accessed by other classes.

    mpsSimBox* GetParallelSimBox() const;
    
    PolymerVector GetAllBulkPolymers() const;
    
    PolymerVector GetPolymersInRectangularRegion(double x, double y, double z, double rx, double ry, double rz);
    
#endif
    
    // PVFs inherited from the ISimState interface base class that change the CSimBox's internal structure
    
    virtual bool MoveBeadBetweenCNTCells(CAbstractBead* const pBead, double x, double y, double z) const;

    // Function to return all the (non-wall) polymers for manipulation by commands
    inline PolymerVector GetAllPolymers() const {return m_vAllPolymers;}
	
    // Public access functions to the CNT cell structure
	AbstractBeadVector GetAllBeadsInCNTCells();

	inline long	  GetCNTCellTotal()			const {return m_CNTCellTotal;}
	inline const  CNTCellVector& GetCNTCells() const {return m_vCNTCells;}
	inline double GetXLength()				const {return m_SimBoxXLength;}
	inline double GetYLength()				const {return m_SimBoxYLength;}
	inline double GetZLength()				const {return m_SimBoxZLength;}
	inline double GetHalfXLength()			const {return m_HalfSimBoxXLength;}
	inline double GetHalfYLength()			const {return m_HalfSimBoxYLength;}
	inline double GetHalfZLength()			const {return m_HalfSimBoxZLength;}
	inline double GetVolume()				const {return m_SimBoxVolume;}
    
    // Access functions to command groups.

	long CountActiveCommandGroups() const;
	long CountCommandGroups() const;
    tguCommandGroup* GetCommandGroup(const zString name);


	// Access functions to the Command Targets used by their implementation classes

	bool CreateCommandTarget(const zString label, long beadType, BeadVector vTargetBeads);
	bool CreateCompositeCommandTarget(const zString label);
	bool CreateCommandTargetPolymer(const zString label, long polyType, PolymerVector vTargetPolymers);
	bool CreateCommandTargetACNFreeBonds(const zString label, const zString acnType);
	bool CreateCommandTargetACNFreePolymers(const zString label, const zString acnType);
    

	CCommandTargetNode* GetCommandTarget(const zString label) const;
	CCommandTargetNode* GetActiveCommandTarget(const zString label) const;
	CCommandTargetNode* GetCommandTargetActivity(const zString label) const;
	CCommandTargetNode* GetActiveCommandTargetOutermost(const zString label) const;
	const CommandTargetSequence& GetCommandTargets() const;
	const CommandTargetSequence& GetActiveCommandTargets() const;
    long GetCommandTargetNodeTotal() const;
    
    // Access functions to the Nanoparticles
    
    bool CreateNanoparticle(BondVector vBonds);
    CNanoparticle* GetNanoparticle(long id);
    long GetNanoparticleTotal() const;

	// Public Instance() function and protected constructor ensure that
	// only a single CSimBox object can be created. We pass it the CSimState object
	// to initialise it.
	// Note that the interface pointers are const as they act only to route
	// messages to and from the SimBox and Monitor.

	static CSimBox* Instance(CSimState& simState, const ISimBox* const pISimBox);

protected:

	CSimBox(CSimState& simState, const ISimBox* const pISimBox);

private:

	// Functions inherited from the interface classes.
	//
	// ISimCmd
	// *******
	// ISimCmd provides access to functions that change the constraints on
	// the SimBox during a simulation. Each function takes an xxCommand-derived 
	// object as an argument that provides data needed for the constraint. 
	// We pass the commands as pointers to their base class and then cast 
	// them inside the function that implements the command.

	virtual void							  AddBeadsInCell(const xxCommand* const pCommand);
	virtual void							   AddBeadsInRow(const xxCommand* const pCommand);
	virtual void							 AddBeadsInSlice(const xxCommand* const pCommand);
	virtual void						   AddPolymersInCell(const xxCommand* const pCommand);
	virtual void							AddPolymersInRow(const xxCommand* const pCommand);
	virtual void						  AddPolymersInSlice(const xxCommand* const pCommand);
	virtual void						AddTargetToComposite(const xxCommand* const pCommand);
	virtual void					  AssignExistingBeadType(const xxCommand* const pCommand);
	virtual void							  ChargeBeadType(const xxCommand* const pCommand);
	virtual void					  CompositeCommandTarget(const xxCommand* const pCommand);
	virtual void							   ConstantForce(const xxCommand* const pCommand);
	virtual void					   ConstantForceOnTarget(const xxCommand* const pCommand);
	virtual void						 CountCommandTargets(const xxCommand* const pCommand);
	virtual void				   CountActiveCommandTargets(const xxCommand* const pCommand);
	virtual void						  CountBeadsInTarget(const xxCommand* const pCommand);
	virtual void					   CountBeadTypeInTarget(const xxCommand* const pCommand);
	virtual void                     CountTargetsInComposite(const xxCommand* const pCommand);
	virtual void							 ExtendTotalTime(const xxCommand* const pCommand);
	virtual void						  FreezeBeadsInSlice(const xxCommand* const pCommand);
	virtual void				 FreezeBeadsInSphericalShell(const xxCommand* const pCommand);
	virtual void						 FreezeBeadsInTarget(const xxCommand* const pCommand);
	virtual void								   GravityOn(const xxCommand* const pCommand);
	virtual void								  GravityOff(const xxCommand* const pCommand);
	virtual void								 LinearForce(const xxCommand* const pCommand);
	virtual void						 LinearForceOnTarget(const xxCommand* const pCommand);
	virtual void						 MovePolymersToSlice(const xxCommand* const pCommand);
	virtual void								 RadialForce(const xxCommand* const pCommand);
	virtual void						 RadialForceOnTarget(const xxCommand* const pCommand);
	virtual void				   RemoveActiveCommandTarget(const xxCommand* const pCommand);
	virtual void				  RemoveCNTCellsInsideRadius(const xxCommand* const pCommand);
	virtual void				 RemoveCNTCellsOutsideRadius(const xxCommand* const pCommand);
	virtual void				 RemoveCommandTargetActivity(const xxCommand* const pCommand);
	virtual void				   RemoveTargetFromComposite(const xxCommand* const pCommand);
	virtual void						  RenormaliseMomenta(const xxCommand* const pCommand);
	virtual void							 RestoreBeadType(const xxCommand* const pCommand);
	virtual void					 RestoreOriginalBeadType(const xxCommand* const pCommand);
	virtual void						   SelectBeadsInCell(const xxCommand* const pCommand);
	virtual void					   SelectBeadsInCylinder(const xxCommand* const pCommand);
	virtual void							SelectBeadsInRow(const xxCommand* const pCommand);
	virtual void						  SelectBeadsInSlice(const xxCommand* const pCommand);
	virtual void						SelectPolymersInCell(const xxCommand* const pCommand);
	virtual void						 SelectPolymersInRow(const xxCommand* const pCommand);
	virtual void					   SelectPolymersInSlice(const xxCommand* const pCommand);
	virtual void							SetBondStiffness(const xxCommand* const pCommand);
	virtual void		  SetBondStiffnessByPositionInTarget(const xxCommand* const pCommand);
	virtual void                    SetBondStiffnessInTarget(const xxCommand* const pCommand);
	virtual void					  		 SetBondStrength(const xxCommand* const pCommand);
	virtual void					   SetBondStrengthbyType(const xxCommand* const pCommand);
	virtual void		   SetBondStrengthByPositionInTarget(const xxCommand* const pCommand);
	virtual void					 SetBondStrengthInTarget(const xxCommand* const pCommand);
	virtual void							 SetCommandTimer(const xxCommand* const pCommand);
	virtual void						   SetDPDBeadConsInt(const xxCommand* const pCommand);
	virtual void					 SetDPDBeadConsIntByType(const xxCommand* const pCommand);
	virtual void						   SetDPDBeadDissInt(const xxCommand* const pCommand);
	virtual void					 SetDPDBeadDissIntByType(const xxCommand* const pCommand);
	virtual void							 SetTimeStepSize(const xxCommand* const pCommand);
	virtual void								   SineForce(const xxCommand* const pCommand);
	virtual void						   SineForceOnTarget(const xxCommand* const pCommand);
	virtual void								 SpringForce(const xxCommand* const pCommand);
	virtual void							UnchargeBeadType(const xxCommand* const pCommand);
	virtual void					   UnFreezeBeadsInTarget(const xxCommand* const pCommand);
	virtual void									 WallOff(const xxCommand* const pCommand);
	virtual void									  WallOn(const xxCommand* const pCommand);
	virtual void								ZeroTotalMom(const xxCommand* const pCommand);
	virtual void				ToggleBeadStressContribution(const xxCommand* const pCommand);
	virtual void				ToggleBondStressContribution(const xxCommand* const pCommand);
	virtual void			ToggleBondPairStressContribution(const xxCommand* const pCommand);

// On certain platforms, implement some commands directly in the SimBox as
// virtual inheritance appears not to work for them.

#if EnableDirectImplCommand == SimCommandEnabled
	virtual void						                Stop(const xxCommand* const pCommand);
	virtual void						          StopNoSave(const xxCommand* const pCommand);
	virtual void		     ToggleDPDBeadConservativeForces(const xxCommand* const pCommand);
	virtual void		                 ToggleDPDBeadForces(const xxCommand* const pCommand);
	virtual void		             ToggleDPDBeadThermostat(const xxCommand* const pCommand);
#endif

	// Functions to create the CNTCell network

	void MakeCNTCells();
	long MapCelltoIndex(long i, long j, long k, long alpha, long beta, long gamma) const;
	long MapCelltoNNIndex(long alpha, long beta, long gamma) const;
    CCNTCell* GetCNTCellFromCoords(double r[3]) const;
	
	
	// Functions to create and access the grid of cells to hold the stress tensor 
	void MakeStressCells();
	CStressGridCell* GetStressCellFromCoords(double r[3]) const;
		
	// Functions to assign the beads and wall beads to the CNT cells

	bool AssignBeadstoCells(const BeadVector& rvAllBeads);
	bool AssignWallBeadstoCells();

    // Functions related to command groups

	bool CreateCommandGroup(const zString name);
    bool DeleteCommandGroup(const zString name);

    bool AddActiveCommandGroup(const zString name);
 	bool RemoveActiveCommandGroup(const zString name);
   	void ExecuteCommandGroups();    // Execute any active command groups


	// Functions to evolve a serial simulation

	void Evolve();					// Calls CCNTCell functions to integrate equations of motion
	void CNTCellCheck();			// Check that beads are in the correct cells
	void AddBodyForce();			// Add the external body force to all affected beads
	void AddBondForces();			// Add bond forces to the beads in polymers
	void AddBondPairForces();		// Add 3-body bond forces to the beads in polymers
	void AddChargedBeadForces();	// Add the screened charge force to charged beads
	void UpdateRenormalisedMom();	// Normalises the momenta to the imposed temperature
	long MCPolymerRelaxation(PolymerVector& rPolymers);	// Relaxes a set of polymers using MC
    
    // Special function to calculate Morse potentials for Helix polymers to create an alpha helix.
    
    void AddHelixForces();

    // Functions to evolve a parallel simulation

    void EvolveP();

private:

	static CSimBox* m_pInstance;	// Pointer to the single instance of the CSimBox class

#if EnableShadowSimBox == SimACNEnabled
    // Pointer to the shadow SimBox if it has been created and ACN functionality 
    // is enabled.
	aeActiveSimBox* m_pShadow;
#endif

    // Parallel implementation classes

#if EnableParallelSimBox == SimMPSEnabled
    // Pointer to the parallel SimBox that is contained in this class and handles
    // all the inter-processor communication in a parallel simulation.

    mpsSimBox* m_pParallel;   
#endif


	// ****************************************
	// Containers holding the set of events, commands etc that are
	// monitored during a simulation.

	NanoparticleSequence  m_Nanoparticles;			// Set of nanoparticles created by command
	CommandGroupSequence  m_CommandGroups;			// Command groups wrap a set of commands
	CommandGroupSequence  m_ActiveCommandGroups;    // Command groups that have a sequential execution pattern
	CommandTargetSequence m_CommandTargets;			// Bead aggregates that can be targets of commands
	CommandTargetSequence m_ActiveCommandTargets;	// Command targets being currently updated
	ForceTargetSequence   m_ForceTargets;			// Bead aggregates that can be acted on by a force
	ForceTargetSequence   m_ActiveForceTargets;		// Bead aggregates currently acted on by a force

	// ****************************************
	// Local data relating to the SimBox

	long m_CNTXCellNo;				// No of CNT cells in each dimension
	long m_CNTYCellNo;
	long m_CNTZCellNo;
	long m_CNTXYCellTotal;			// No of CNT cells in XY plane
	long m_CNTCellTotal;			// Total number of CNT cells in SimBox

	long m_BeadTotal;				// Total number of movable (non-wall) beads
	long m_WallBeadTotal;			// Number of beads fixed in the wall
	long m_ChargedBeadTotal;		// Total number of charged beads
	long m_PolymerTotal;			// Total number of non-wall polymers
	long m_WallPolymerTotal;		// Total number of wall polymers

	double m_CNTXCellWidth;			// Width of CNT cells in units of bead radius
	double m_CNTYCellWidth;
	double m_CNTZCellWidth;
	double m_HalfSimBoxXLength;
	double m_HalfSimBoxYLength;		// Half SimBox side lengths
	double m_HalfSimBoxZLength;
	double m_SimBoxXLength;			// Length of SimBox in each dimension in units of bead radius
	double m_SimBoxYLength;
	double m_SimBoxZLength;
	double m_SimBoxVolume;			// Volume of SimBox

	double m_GravityXForce;			// Body force acting throughout SimBox on movable beads
	double m_GravityYForce;
	double m_GravityZForce;
	
	
	// ****************************************
	// Containers holding the set of beads, bonds, bondpairs and polymers
	// that represent the constituents of a simulation, and the CNT cells 
	// that are used to construct the SimBox.
	// Certain subsets of the polymers, and their constituent beads and bonds,
	// are duplicated in containers that hold objects that share a 
	// common behaviour, e.g., charged beads and wall polymers.

	CNTCellVector		m_vCNTCells;			// Vector of CNT cells filling the SimBox
	BeadVector			m_vGravityBeads;		// Vector of beads affected by the body force
	AbstractBeadVector	m_vWallBeads;			// Vector of beads composing the wall
	ChargedBeadList		m_lAllChargedBeads;		// List of charged beads

	PolymerVector		m_vAllPolymers;			// Vector of non-wall polymers
	PolymerVector		m_vWallPolymers;		// Vector of wall polymers
    PolymerVector       m_vHelixPolymers;       // Vector of polymers capable of forming a helix

	zLongVector			m_vChargedBeadTypes;	// Types of charged beads
	zLongVector			m_vChargedBeadTotals;	// No of each charged bead type
	
	// Local data relating to the stress tensor calculation in curvilinear coordinates.
	
	const long         m_StressCellMultiplier;       // No of stress grid cells per CNT cell in each dimension
    const long         m_StressCellXNo;
    const long         m_StressCellYNo;              // Size of the grid holding the local stress tensor
    const long         m_StressCellZNo;
	const long         m_StressCellTotal;
	
	const long         m_PointTotal;                 // Number of points along line joining two interacting beads at which stress is assigned
	const double       m_InversePointTotal;          
	const double       m_InversePointTotalMinusOne;  // Number of segments along line joining two interacting beads: one less because we include both endpoints
    const double       m_StressCellXWidth;
    const double       m_StressCellYWidth;           // Width of the stress grid cells in each dimension
    const double       m_StressCellZWidth;
	
	
	CStressGridCell*   m_pOldCell;
	CStressGridCell*   m_pNewCell;
	double             m_StressWeight;               // Fraction of stress added to each CNT cell along a vector joining two points
	double             m_rprime[3];
	double             m_dr[3];
	double             m_LocalStress[9];
    double             m_StressOrigin[3];           // Origin used to transform the stress from Cartesian to spherical polar coordinates

    StressCellVector   m_vStressCells;              // Vector of cells holding the local stress tensor in curvilinear coordinates

};

#endif // !defined(AFX_SIMBOX_H__05CAAA00_2EE6_11D3_820E_0060088AD300__INCLUDED_)

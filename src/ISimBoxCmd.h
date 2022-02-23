// ISimBoxCmd.h: interface for the ISimBoxCmd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISIMBOXCMD_H__4EB53FBD_5678_4017_B8CF_ACA9BE891A2C__INCLUDED_)
#define AFX_ISIMBOXCMD_H__4EB53FBD_5678_4017_B8CF_ACA9BE891A2C__INCLUDED_


#include "SimACNFlags.h"
#include "SimProcessFlags.h"

// Forward declarations

class xxCommand;


class ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction: Protected constructor declared below
public:

	virtual ~ISimBoxCmd();

	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// Command interface functions to change the simulation state during a run.

#if EnableShadowSimBox == SimACNEnabled
    // Active SimBox commands
	virtual void						   AddArp23Bond(const xxCommand* const pCommand) = 0;
	virtual void					     AddCofilinBond(const xxCommand* const pCommand) = 0;
	virtual void					      AddfActinBond(const xxCommand* const pCommand) = 0;
	virtual void					    AddfActinCPBond(const xxCommand* const pCommand) = 0;
	virtual void					      AddForminBond(const xxCommand* const pCommand) = 0;
	virtual void					    AddProfilinBond(const xxCommand* const pCommand) = 0;
	virtual void					    AddReceptorBond(const xxCommand* const pCommand) = 0;
	virtual void			   BinEventSuccessIntervals(const xxCommand* const pCommand) = 0;
	virtual void					 CreateActiveSimBox(const xxCommand* const pCommand) = 0;
	virtual void     CreateBondBindsForwardConeToPolymerHeadEventSource(const xxCommand* const pCommand) = 0;
	virtual void     CreateBondBindsForwardConeToPolymerTailEventSource(const xxCommand* const pCommand) = 0;
	virtual void					 DeleteActiveSimBox(const xxCommand* const pCommand) = 0;
	virtual void	                    RemoveArp23Bond(const xxCommand* const pCommand) = 0;
	virtual void				      RemoveCofilinBond(const xxCommand* const pCommand) = 0;
	virtual void				       RemovefActinBond(const xxCommand* const pCommand) = 0;
	virtual void				     RemovefActinCPBond(const xxCommand* const pCommand) = 0;
	virtual void				       RemoveForminBond(const xxCommand* const pCommand) = 0;
	virtual void				     RemoveProfilinBond(const xxCommand* const pCommand) = 0;
	virtual void				     RemoveReceptorBond(const xxCommand* const pCommand) = 0;
	virtual void		              SelectACNAsTarget(const xxCommand* const pCommand) = 0;
	virtual void		  SelectACNCurrentBondsAsTarget(const xxCommand* const pCommand) = 0;
	virtual void	   SelectACNCurrentPolymersAsTarget(const xxCommand* const pCommand) = 0;
	virtual void		     SelectACNFreeBondsAsTarget(const xxCommand* const pCommand) = 0;
	virtual void		  SelectACNFreePolymersAsTarget(const xxCommand* const pCommand) = 0;
	virtual void        SerialisePolymerHeadCoordinates(const xxCommand* const pCommand) = 0;
	virtual void        SerialisePolymerTailCoordinates(const xxCommand* const pCommand) = 0;
#endif
#if EnableCommandGroups == SimCommandEnabled
	virtual void			                 AddCommandToGroup(const xxCommand* const pCommand) = 0;
	virtual void			        InternalCreateCommandGroup(const xxCommand* const pCommand) = 0;
	virtual void			        InternalDeleteCommandGroup(const xxCommand* const pCommand) = 0;
	virtual void	         DeleteExecuteCommandGroupSequence(const xxCommand* const pCommand) = 0;
	virtual void			           ExecuteCommandGroupOnce(const xxCommand* const pCommand) = 0;
	virtual void			       ExecuteCommandGroupSequence(const xxCommand* const pCommand) = 0;
	virtual void                         SetArgumentToArgument(const xxCommand* const pCommand) = 0;
	virtual void                  SetArgumentToIntegerConstant(const xxCommand* const pCommand) = 0;
	virtual void                  SetArgumentToIntegerSequence(const xxCommand* const pCommand) = 0;
	virtual void                     SetArgumentToRealConstant(const xxCommand* const pCommand) = 0;
	virtual void                     SetArgumentToRealSequence(const xxCommand* const pCommand) = 0;
	virtual void                   SetArgumentToStringConstant(const xxCommand* const pCommand) = 0;
	virtual void                   SetArgumentToStringSequence(const xxCommand* const pCommand) = 0;
	virtual void      SetArgumentsTo2dRectangularLatticeVector(const xxCommand* const pCommand) = 0;
	virtual void       SetArgumentsTo2dTriangularLatticeVector(const xxCommand* const pCommand) = 0;
	virtual void      SetArgumentsTo3dRectangularLatticeVector(const xxCommand* const pCommand) = 0;
	virtual void              ToggleAllCommandExecutionInGroup(const xxCommand* const pCommand) = 0;
	virtual void                 ToggleCommandExecutionInGroup(const xxCommand* const pCommand) = 0;
#endif
#if EnableTargetCommand == SimCommandEnabled
    // Command Target commands
    // Note that the first set of SelectXXX commands have the prefix cc not ct as they create targets.
	virtual void		     CreateNanoparticlePentagon(const xxCommand* const pCommand) = 0;
	virtual void		       CreateNanoparticleSphere(const xxCommand* const pCommand) = 0;
	virtual void			   SelectBeadTypeInCylinder(const xxCommand* const pCommand) = 0;
	virtual void		      SelectBeadTypeInEllipsoid(const xxCommand* const pCommand) = 0;
	virtual void			   SelectBeadTypeInPentagon(const xxCommand* const pCommand) = 0;
	virtual void				 SelectBeadTypeInSimBox(const xxCommand* const pCommand) = 0;
	virtual void				  SelectBeadTypeInSlice(const xxCommand* const pCommand) = 0;
	virtual void				 SelectBeadTypeInSphere(const xxCommand* const pCommand) = 0;
	virtual void		   SelectBeadTypeInSphericalCap(const xxCommand* const pCommand) = 0;
	virtual void		SelectPolymerTypeHeadInCylinder(const xxCommand* const pCommand) = 0;
	virtual void	   SelectPolymerTypeHeadInEllipsoid(const xxCommand* const pCommand) = 0;
	virtual void	    SelectPolymerTypeHeadInPentagon(const xxCommand* const pCommand) = 0;
	virtual void		  SelectPolymerTypeHeadInSphere(const xxCommand* const pCommand) = 0;
	virtual void		      SelectPolymerTypeInSimBox(const xxCommand* const pCommand) = 0;
	virtual void	       SelectPolymerTypeHeadInSlice(const xxCommand* const pCommand) = 0;
	virtual void	SelectPolymerTypeHeadInSphericalCap(const xxCommand* const pCommand) = 0;
	virtual void		            AnchorForceOnTarget(const xxCommand* const pCommand) = 0;
	virtual void		             AxialForceOnTarget(const xxCommand* const pCommand) = 0;
	virtual void				         ChangeBeadType(const xxCommand* const pCommand) = 0;
	virtual void				         ChangeBondType(const xxCommand* const pCommand) = 0;
	virtual void				     ChangeBondPairType(const xxCommand* const pCommand) = 0;
	virtual void				    ChangeNamedBeadType(const xxCommand* const pCommand) = 0;
	virtual void   ChangeNamedBeadTypeInCompositeTarget(const xxCommand* const pCommand) = 0;
	virtual void                   ConstantSpecificArea(const xxCommand* const pCommand) = 0;
	virtual void		    CylinderLinearForceOnTarget(const xxCommand* const pCommand) = 0;
    virtual void      CylindricalDistributionOfCompositeTarget(const xxCommand* const pCommand) = 0;
    virtual void        CylindricalDistributionOfPolymerTarget(const xxCommand* const pCommand) = 0;
	virtual void			      DistanceMovedByTarget(const xxCommand* const pCommand) = 0;
	virtual void			     EdgeClampForceOnTarget(const xxCommand* const pCommand) = 0;
	virtual void			       ExternalWorkOnTarget(const xxCommand* const pCommand) = 0;
	virtual void    ExtractBeadTypesIntoCompositeTarget(const xxCommand* const pCommand) = 0;
	virtual void			ExtractBeadTypesIntoTargets(const xxCommand* const pCommand) = 0;
    virtual void			   ListActiveCommandTargets(const xxCommand* const pCommand) = 0;
    virtual void	     ListAllCommandTargetActivities(const xxCommand* const pCommand) = 0;
    virtual void			ListCommandTargetActivities(const xxCommand* const pCommand) = 0;
    virtual void			         ListCommandTargets(const xxCommand* const pCommand) = 0;
    virtual void                          MSDBeadTarget(const xxCommand* const pCommand) = 0;
    virtual void                       MSDPolymerTarget(const xxCommand* const pCommand) = 0;
	virtual void		      PlanarAnchorForceOnTarget(const xxCommand* const pCommand) = 0;
    virtual void			 PolymerisePolymersInTarget(const xxCommand* const pCommand) = 0;
	virtual void		PolymerisePolymerTripleInTarget(const xxCommand* const pCommand) = 0;
    virtual void              ReflectingBarrierOnTarget(const xxCommand* const pCommand) = 0;
    virtual void			               RgBeadTarget(const xxCommand* const pCommand) = 0;
    virtual void			            RgPolymerTarget(const xxCommand* const pCommand) = 0;
	virtual void			 RotationalMSDPolymerTarget(const xxCommand* const pCommand) = 0;
	virtual void     SelectBeadsInSphericalTargetRegion(const xxCommand* const pCommand) = 0;
	virtual void			 SetTargetBeadTypeDisplayId(const xxCommand* const pCommand) = 0;
	virtual void					 SetTargetDisplayId(const xxCommand* const pCommand) = 0;
	virtual void		  SetTargetPolymerTypeDisplayId(const xxCommand* const pCommand) = 0;
	virtual void					 SetTargetInvisible(const xxCommand* const pCommand) = 0;
	virtual void					   SetTargetVisible(const xxCommand* const pCommand) = 0;
	virtual void		      SpringForceBetweenTargets(const xxCommand* const pCommand) = 0;
	virtual void		            SpringForceOnTarget(const xxCommand* const pCommand) = 0;
	virtual void				ToggleAntiTargetDisplay(const xxCommand* const pCommand) = 0;
	virtual void				    ToggleTargetDisplay(const xxCommand* const pCommand) = 0;
	virtual void			 WorkDoneOnBeadTypeInTarget(const xxCommand* const pCommand) = 0;
#endif
#if EnableConstraintCommand == SimCommandEnabled
    // Constraint commands
	virtual void					     ChargeBeadType(const xxCommand* const pCommand) = 0;
	virtual void					   ChargeBeadByType(const xxCommand* const pCommand) = 0;
	virtual void				     FreezeBeadsInSlice(const xxCommand* const pCommand) = 0;
	virtual void	    	FreezeBeadsInSphericalShell(const xxCommand* const pCommand) = 0;
	virtual void		     RemoveCNTCellsInsideRadius(const xxCommand* const pCommand) = 0;
	virtual void		    RemoveCNTCellsOutsideRadius(const xxCommand* const pCommand) = 0;
	virtual void					 UnchargeBeadByType(const xxCommand* const pCommand) = 0;
	virtual void				       UnchargeBeadType(const xxCommand* const pCommand) = 0;
#endif
    virtual void						 AddBeadsInCell(const xxCommand* const pCommand) = 0;
	virtual void						  AddBeadsInRow(const xxCommand* const pCommand) = 0;
	virtual void					    AddBeadsInSlice(const xxCommand* const pCommand) = 0;
	virtual void				      AddPolymersInCell(const xxCommand* const pCommand) = 0;
	virtual void				       AddPolymersInRow(const xxCommand* const pCommand) = 0;
	virtual void				     AddPolymersInSlice(const xxCommand* const pCommand) = 0;
	virtual void			       AddTargetToComposite(const xxCommand* const pCommand) = 0;
	virtual void			     AssignExistingBeadType(const xxCommand* const pCommand) = 0;
	virtual void			     CompositeCommandTarget(const xxCommand* const pCommand) = 0;
	virtual void					      ConstantForce(const xxCommand* const pCommand) = 0;
	virtual void			      ConstantForceOnTarget(const xxCommand* const pCommand) = 0;
	virtual void		      CountActiveCommandTargets(const xxCommand* const pCommand) = 0;
	virtual void		            CountCommandTargets(const xxCommand* const pCommand) = 0;
	virtual void			         CountBeadsInTarget(const xxCommand* const pCommand) = 0;
	virtual void			      CountBeadTypeInTarget(const xxCommand* const pCommand) = 0;
	virtual void			    CountTargetsInComposite(const xxCommand* const pCommand) = 0;
	virtual void			            ExtendTotalTime(const xxCommand* const pCommand) = 0;
	virtual void			        FreezeBeadsInTarget(const xxCommand* const pCommand) = 0;
	virtual void						     GravityOff(const xxCommand* const pCommand) = 0;
	virtual void						      GravityOn(const xxCommand* const pCommand) = 0;
	virtual void						    LinearForce(const xxCommand* const pCommand) = 0;
	virtual void				    LinearForceOnTarget(const xxCommand* const pCommand) = 0;
	virtual void			        MovePolymersToSlice(const xxCommand* const pCommand) = 0;
	virtual void						    RadialForce(const xxCommand* const pCommand) = 0;
	virtual void				    RadialForceOnTarget(const xxCommand* const pCommand) = 0;
	virtual void	          RemoveActiveCommandTarget(const xxCommand* const pCommand) = 0;
	virtual void	        RemoveCommandTargetActivity(const xxCommand* const pCommand) = 0;
	virtual void	          RemoveTargetFromComposite(const xxCommand* const pCommand) = 0;
	virtual void				     RenormaliseMomenta(const xxCommand* const pCommand) = 0;
	virtual void				        RestoreBeadType(const xxCommand* const pCommand) = 0;
	virtual void		        RestoreOriginalBeadType(const xxCommand* const pCommand) = 0;
	virtual void				      SelectBeadsInCell(const xxCommand* const pCommand) = 0;
	virtual void				       SelectBeadsInRow(const xxCommand* const pCommand) = 0;
	virtual void					 SelectBeadsInSlice(const xxCommand* const pCommand) = 0;
	virtual void				  SelectBeadsInCylinder(const xxCommand* const pCommand) = 0;
	virtual void				   SelectPolymersInCell(const xxCommand* const pCommand) = 0;
	virtual void					SelectPolymersInRow(const xxCommand* const pCommand) = 0;
	virtual void				  SelectPolymersInSlice(const xxCommand* const pCommand) = 0;
	virtual void					   SetBondStiffness(const xxCommand* const pCommand) = 0;
	virtual void	 SetBondStiffnessByPositionInTarget(const xxCommand* const pCommand) = 0;
	virtual void			   SetBondStiffnessInTarget(const xxCommand* const pCommand) = 0;
	virtual void					    SetBondStrength(const xxCommand* const pCommand) = 0;
	virtual void			      SetBondStrengthbyType(const xxCommand* const pCommand) = 0;
	virtual void	  SetBondStrengthByPositionInTarget(const xxCommand* const pCommand) = 0;
	virtual void				SetBondStrengthInTarget(const xxCommand* const pCommand) = 0;
	virtual void					    SetCommandTimer(const xxCommand* const pCommand) = 0;
	virtual void				      SetDPDBeadConsInt(const xxCommand* const pCommand) = 0;
	virtual void			    SetDPDBeadConsIntByType(const xxCommand* const pCommand) = 0;
	virtual void				      SetDPDBeadDissInt(const xxCommand* const pCommand) = 0;
	virtual void			    SetDPDBeadDissIntByType(const xxCommand* const pCommand) = 0;
	virtual void					    SetTimeStepSize(const xxCommand* const pCommand) = 0;
	virtual void						      SineForce(const xxCommand* const pCommand) = 0;
	virtual void				      SineForceOnTarget(const xxCommand* const pCommand) = 0;
	virtual void						    SpringForce(const xxCommand* const pCommand) = 0;
	virtual void			   TranslateTargetToC1Point(const xxCommand* const pCommand) = 0;
	virtual void			   TranslateTargetToC2Point(const xxCommand* const pCommand) = 0;
	virtual void				 TranslateTargetToPlane(const xxCommand* const pCommand) = 0;
	virtual void				TranslateTargetToZPlane(const xxCommand* const pCommand) = 0;
	virtual void			      UnFreezeBeadsInTarget(const xxCommand* const pCommand) = 0;

#if EnableCommandDocumentProcess == SimProcessEnabled
	virtual void	        StartCommandDocumentProcess(const xxCommand* const pCommand) = 0;
#endif

    virtual void								   Stop(const xxCommand* const pCommand) = 0;
	virtual void							 StopNoSave(const xxCommand* const pCommand) = 0;
	virtual void		ToggleDPDBeadConservativeForces(const xxCommand* const pCommand) = 0;
	virtual void				    ToggleDPDBeadForces(const xxCommand* const pCommand) = 0;
	virtual void				ToggleDPDBeadThermostat(const xxCommand* const pCommand) = 0;
	virtual void							     WallOn(const xxCommand* const pCommand) = 0;
	virtual void							    WallOff(const xxCommand* const pCommand) = 0;
	virtual void					       ZeroTotalMom(const xxCommand* const pCommand) = 0;

	// Commands to turn on and off the various contributions to the stress tensor
	// calculation.

	virtual void     ToggleBeadStressContribution(const xxCommand* const pCommand) = 0;
	virtual void     ToggleBondStressContribution(const xxCommand* const pCommand) = 0;
	virtual void ToggleBondPairStressContribution(const xxCommand* const pCommand) = 0;

	// ****************************************
	// Public access functions
public:

	// ****************************************
	// Protected local functions
protected:

	ISimBoxCmd();

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:

	// ****************************************
	// Data members


};

#endif // !defined(AFX_ISIMBOXCMD_H__4EB53FBD_5678_4017_B8CF_ACA9BE891A2C__INCLUDED_)

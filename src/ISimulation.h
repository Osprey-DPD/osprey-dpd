// ISimulation.h: interface for the ISimulation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISIMULATION_H__E80CEB6F_4A40_43D9_989C_AAFA42578562__INCLUDED_)
#define AFX_ISIMULATION_H__E80CEB6F_4A40_43D9_989C_AAFA42578562__INCLUDED_


// Forward declarations

class IExperimentAccessControl;
class ISecureSimulation;
class ISetSimulationData;
class ISetSubstrateData;
class ISetSculptureData;
class ISetProbeFieldData;
class ISetExternalFieldData;
class ISetProcessData;


#include "xxParallelBase.h"

class ISimulation : public xxParallelBase
{
	// Main interface implemented by the simulation object

	// ****************************************
	// Construction/Destruction: 
public:

	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	virtual bool RunPrevis(StringSequence excludedPolymers) = 0;
	virtual bool Run() = 0;

	virtual IExperimentAccessControl* GetIExperimentAccessControl() = 0;
	virtual ISecureSimulation*        GetISecureSimulation()        = 0;
	virtual ISetSimulationData*       GetISetSimulationData()       = 0;
	virtual ISetSubstrateData*        GetISetSubstrateData()        = 0;
	virtual ISetSculptureData*        GetISetSculptureData()        = 0;
	virtual ISetProbeFieldData*       GetISetProbeFieldData()       = 0;
	virtual ISetExternalFieldData*    GetISetExternalFieldData()    = 0;
	virtual ISetProcessData*          GetISetProcessData()          = 0;



	// ****************************************
	// Public access functions
public:


	// ****************************************
	// Protected local functions
protected:

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:



};

#endif // !defined(AFX_ISIMULATION_H__E80CEB6F_4A40_43D9_989C_AAFA42578562__INCLUDED_)

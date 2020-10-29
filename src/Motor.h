// Motor.h: interface for the CMotor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOTOR_H__796009A4_FCDD_4F8C_8F14_687CCECD95ED__INCLUDED_)
#define AFX_MOTOR_H__796009A4_FCDD_4F8C_8F14_687CCECD95ED__INCLUDED_


// Forward declarations

class CMotorData;
class CSliceProfile;

// Include files

#include "Polymer.h"

#include "Analysis.h"

class CMotor : public CAnalysis  
{
	// ****************************************
	// Construction/Destruction
public:

	CMotor(const zString runId, long samplePeriod, const CMotorData* const pAD);
	virtual ~CMotor();


	// ****************************************
	// IEventXXX interface functions for all supported events
public:


	// ****************************************
	// Public access functions
public:

	inline const zString GetPolymer() const {return m_Polymer;}
	inline const zString GetFuel()    const {return m_Fuel;}
	inline const zString GetSolvent() const {return m_Solvent;}

	inline long GetMotorTotal()		const {return m_MotorTotal;}
	inline long GetMotorHeadType()	const {return m_MotorHeadType;}
	inline long GetMotorNeckType()	const {return m_MotorNeckType;}
	inline long GetMotorHingeType()	const {return m_MotorHingeType;}
	inline long GetMotorTailType()	const {return m_MotorTailType;}


	// ****************************************
	// PVS inherited from CAnalysis base class
public:

	// Function to get information about the motor from the ISimBox

	virtual void UpdateState(CSimState& rSimState, const ISimBox* const pISimBox);

	// Function to convert any bead, bond or polymer names from strings to integers

	virtual void ConvertNames(const CAnalysisState& raState);


private:
	// Data specified by user for motor analysis

	zString m_Polymer;		// Polymer composing motor
	zString m_Fuel;			// Polymer representing fuel for the motor
	zString m_Solvent;		// Solvent polymer name (usually Water)

	// Local data used in the analysis

	CTimeSeriesData* m_pTSD;	// Store time-dependent data here

	long m_MotorTotal;			// Number of motor polymers in simulation

	long m_MotorType;			// Motor polymer type
	long m_MotorHeadType;		// Motor's head bead
	long m_MotorNeckType;		// Motor's neck bead
	long m_MotorHingeType;		// Motor's hinge bead 
	long m_MotorTailType;		// Motor's tail bead

	long m_FuelType;			// Fuel polymer type
	long m_FuelHeadType;		// Fuel polymer head bead type

	long m_SolventType;			// Solvent polymer type

	CSliceProfile* m_pFuelSP;	// SliceProfile for motor fuel polymers

	aaScalarProfile* m_pFuelDensity;	// Density of fuel in SimBox

	PolymerList m_lMotors;				// List of pointers to all motor polymers
	BeadList	m_lMotorHead;			// Pointers to motor head beads
	BeadList	m_lMotorNeck;			// Pointers to motor neck beads
	BeadList	m_lMotorHinge;			// Pointers to motor hinge beads


	zLongVector	m_vMotorCellIndices;	// Vector of CNT cell indices containing motors


	LongPolymerMap m_mPolymerFromHeadId;		// Map taking head bead id to polymer
	LongPolymerMap m_mPolymerFromTailId;		// Map taking tail bead id to polymer



};

#endif // !defined(AFX_MOTOR_H__796009A4_FCDD_4F8C_8F14_687CCECD95ED__INCLUDED_)


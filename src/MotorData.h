// MotorData.h: interface for the CMotorData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOTORDATA_H__95931DF6_007B_4E91_BB5D_89EAA53D9354__INCLUDED_)
#define AFX_MOTORDATA_H__95931DF6_007B_4E91_BB5D_89EAA53D9354__INCLUDED_


#include "AnalysisData.h"

class CMotorData : public CAnalysisData  
{
	friend class CMotor;		// Needed to copy data into CMotor

	// ****************************************
	// Construction/Destruction
public:

	CMotorData();

	virtual ~CMotorData();

	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();

private:

	static zString m_Type;		// Keyword used to identify aggregate analysis

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	zOutStream& put(zOutStream& os) const;
	zInStream&  get(zInStream& is);

	virtual const zString GetAnalysisType() const;

	// Function to copy the data into a CAnalysis-derived object

	virtual CAnalysis* CreateAnalysis(const zString runId, long samplePeriod);

	virtual bool ValidateData(const CInputData& riData) const;

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


	zString m_Polymer;				// Polymer composing motor
	zString m_Fuel;					// Polymer fuel for motor
	zString m_Solvent;				// Polymer representing the solvent (usually W)

};

#endif // !defined(AFX_MOTORDATA_H__95931DF6_007B_4E91_BB5D_89EAA53D9354__INCLUDED_)

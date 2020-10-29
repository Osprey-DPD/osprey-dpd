// mcSaveBeadVelocityScalarProduct1dProfile.h: interface for the mcSaveBeadVelocityScalarProduct1dProfile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSAVEBEADVELOCITYSCALARPRODUCT1DPROFILE_H__F0BF3269_7088_4006_8F9B_0A669A636DFD__INCLUDED_)
#define AFX_MCSAVEBEADVELOCITYSCALARPRODUCT1DPROFILE_H__F0BF3269_7088_4006_8F9B_0A669A636DFD__INCLUDED_


#include "xxCommand.h" 

class mcSaveBeadVelocityScalarProduct1dProfile : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	mcSaveBeadVelocityScalarProduct1dProfile(long executionTime);
	mcSaveBeadVelocityScalarProduct1dProfile(const mcSaveBeadVelocityScalarProduct1dProfile& oldCommand);

	virtual ~mcSaveBeadVelocityScalarProduct1dProfile();
	
	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	// Return the type of command

	
private:

	static const zString m_Type;	// Identifier used in control data file for command


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	zOutStream& put(zOutStream& os) const;
	zInStream&  get(zInStream& is);

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an xxCommand pointer,
	// respond to the SimBox's request to execute and return the name of the command.

	virtual bool Execute(long simTime, ISimCmd* const pISimCmd) const;

	virtual const xxCommand* GetCommand() const;

	virtual bool IsDataValid(const CInputData& riData) const;

	// ****************************************
	// Public access functions
public:

	inline long	GetAnalysisPeriods() const {return m_TotalAnalysisPeriods;}
	inline long GetTotalDataPoints() const {return m_TotalDataPoints;}

	// ****************************************
	// Protected local functions
protected:

	virtual const zString GetCommandType() const;

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

	long m_TotalAnalysisPeriods;	// Number of analysis periods to sample over
	long m_TotalDataPoints;	        // Number of data points required


};

#endif // !defined(AFX_MCSAVEBEADVELOCITYSCALARPRODUCT1DPROFILE_H__F0BF3269_7088_4006_8F9B_0A669A636DFD__INCLUDED_)

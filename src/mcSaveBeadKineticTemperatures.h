// mcSaveBeadKineticTemperatures.h: interface for the mcSaveBeadKineticTemperatures class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSAVEBEADKINETICTEMPERATURES_H__44A5351E_A047_41AF_90EA_054A697F8136__INCLUDED_)
#define AFX_MCSAVEBEADKINETICTEMPERATURES_H__44A5351E_A047_41AF_90EA_054A697F8136__INCLUDED_


#include "xxCommand.h" 

class mcSaveBeadKineticTemperatures : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	mcSaveBeadKineticTemperatures(long executionTime);
	mcSaveBeadKineticTemperatures(const mcSaveBeadKineticTemperatures& oldCommand);

	virtual ~mcSaveBeadKineticTemperatures();
	
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

#endif // !defined(AFX_MCSAVEBEADKINETICTEMPERATURES_H__44A5351E_A047_41AF_90EA_054A697F8136__INCLUDED_)

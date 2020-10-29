// mcSaveBeadDensityFluctuations.h: interface for the mcSaveBeadDensityFluctuations class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSAVEBEADDENSITYFLUCTUATIONS_H__0DBB41E7_9D21_4169_B353_EC0B65AF9EEA__INCLUDED_)
#define AFX_MCSAVEBEADDENSITYFLUCTUATIONS_H__0DBB41E7_9D21_4169_B353_EC0B65AF9EEA__INCLUDED_


#include "xxCommand.h" 

class mcSaveBeadDensityFluctuations : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	mcSaveBeadDensityFluctuations(long executionTime);
	mcSaveBeadDensityFluctuations(const mcSaveBeadDensityFluctuations& oldCommand);

	virtual ~mcSaveBeadDensityFluctuations();
	
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

    inline const zString GetBeadName()   const {return m_BeadName;}
	inline long	GetTotalDataPoints()     const {return m_TotalDataPoints;}
	inline long	GetTotalDensityPeriods() const {return m_TotalDensityPeriods;}
    inline long GetXCellNo()             const {return m_XCellNo;}
    inline long GetYCellNo()             const {return m_YCellNo;}
    inline long GetZCellNo()             const {return m_ZCellNo;}
    inline bool GetConjugateFlag()       const {return m_bConjugate;}

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


	zString m_BeadName;	            // String identifier for bead type
    long    m_TotalDataPoints;      // No of values to output
    long    m_TotalDensityPeriods;  // No of density periods to average over
    long    m_XCellNo;
    long    m_YCellNo;              // Indices for cell being analysed
    long    m_ZCellNo;
    bool    m_bConjugate;           // Flag showing if results are output for
                                    // the cell and its complementary volume
};

#endif // !defined(AFX_MCSAVEBEADDENSITYFLUCTUATIONS_H__0DBB41E7_9D21_4169_B353_EC0B65AF9EEA__INCLUDED_)

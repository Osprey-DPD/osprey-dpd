// mcSaveBeadDensityFluctuationsByType.h: interface for the mcSaveBeadDensityFluctuationsByType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSAVEBEADDENSITYFLUCTUATIONSBYTYPE_H__92592C1E_9AB1_46A5_B25E_7806B4CDC0D6__INCLUDED_)
#define AFX_MCSAVEBEADDENSITYFLUCTUATIONSBYTYPE_H__92592C1E_9AB1_46A5_B25E_7806B4CDC0D6__INCLUDED_


#include "xxCommand.h" 

class mcSaveBeadDensityFluctuationsByType : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	mcSaveBeadDensityFluctuationsByType(long executionTime);
	mcSaveBeadDensityFluctuationsByType(const mcSaveBeadDensityFluctuationsByType& oldCommand);

	virtual ~mcSaveBeadDensityFluctuationsByType();
	
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

    inline long GetBeadType()            const {return m_BeadType;}
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


	long    m_BeadType;	            // Numeric identifier for bead type
    long    m_TotalDataPoints;      // No of values to output
    long    m_TotalDensityPeriods;  // No of density periods to average over
    long    m_XCellNo;
    long    m_YCellNo;              // Indices for cell being analysed
    long    m_ZCellNo;
    bool    m_bConjugate;           // Flag showing if results are output for
                                    // the cell and its complementary volume
};

#endif // !defined(AFX_MCSAVEBEADDENSITYFLUCTUATIONSBYTYPE_H__92592C1E_9AB1_46A5_B25E_7806B4CDC0D6__INCLUDED_)

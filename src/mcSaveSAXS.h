// mcSaveSAXS.h: interface for the mcSaveSAXS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSAVESAXS_H__0e115646_598e_4483_96ac_eb42c7be21d1__INCLUDED_)
#define AFX_MCSAVESAXS_H__0e115646_598e_4483_96ac_eb42c7be21d1__INCLUDED_


#include "xxCommand.h" 

class mcSaveSAXS : public xxCommand
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	mcSaveSAXS(long executionTime);
	mcSaveSAXS(const mcSaveSAXS& oldCommand);

	virtual ~mcSaveSAXS();
	
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

	inline long	  GetAnalysisPeriods()                const {return m_TotalAnalysisPeriods;}
    inline long   GetTotalDataPoints()                const {return m_TotalDataPoints;}
    inline double GetQMin()                           const {return m_QMin;}
    inline double GetQMax()                           const {return m_QMax;}
    inline const  zBoolVector GetIncludedBeads()      const {return m_vIncludedBeads;}

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

	long    m_TotalAnalysisPeriods;	     // Number of analysis periods to sample over
	long    m_TotalDataPoints;	         // Number of q values in I(q)
    
    double  m_QMin;                      // Minimum q value to use (> 0.0, inverse Angstrom)
    double  m_QMax;                      // Maximum q value to use (if == 0.0, default range will be applied)
	
    zBoolVector   m_vIncludedBeads;      // Bead types to include in the calculation


};

#endif // !defined(AFX_MCSAVESAXS_H__0e115646_598e_4483_96ac_eb42c7be21d1__INCLUDED_)

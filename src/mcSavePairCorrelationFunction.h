// mcSavePairCorrelationFunction.h: interface for the mcSavePairCorrelationFunction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSAVEPAIRCORRELATIONFUNCION_H__d142dc1c_436b_46c5_a2ad_248ed1626ebb__INCLUDED_)
#define AFX_MCSAVEPAIRCORRELATIONFUNCION_H__d142dc1c_436b_46c5_a2ad_248ed1626ebb__INCLUDED_


#include "xxCommand.h" 

class mcSavePairCorrelationFunction : public xxCommand
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	mcSavePairCorrelationFunction(long executionTime);
	mcSavePairCorrelationFunction(const mcSavePairCorrelationFunction& oldCommand);

	virtual ~mcSavePairCorrelationFunction();
	
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
	inline double GetRMax()                           const {return m_RMax;}
    inline const  zBoolVector GetExcludedPolymers()   const {return m_vExcludedPolymers;}

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
	long    m_TotalDataPoints;	         // Number of bins in histogram
	double  m_RMax;                      // Maximum separation to calculate histogram
	
    zBoolVector   m_vExcludedPolymers;   // Polymers to exclude from the calculation


};

#endif // !defined(AFX_MCSAVEPAIRCORRELATIONFUNCION_H__d142dc1c_436b_46c5_a2ad_248ed1626ebb__INCLUDED_)

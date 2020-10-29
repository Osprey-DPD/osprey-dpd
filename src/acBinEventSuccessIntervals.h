// acBinEventSuccessIntervals.h: interface for the acBinEventSuccessIntervals class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACBINEVENTSUCCESSINTERVALS_H__A997F03B_5D19_4685_B8A3_D80E126C5334__INCLUDED_)
#define AFX_ACBINEVENTSUCCESSINTERVALS_H__A997F03B_5D19_4685_B8A3_D80E126C5334__INCLUDED_


#include "acACNCreateEventAnalysis.h" 

class acBinEventSuccessIntervals : public acACNCreateEventAnalysis  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	acBinEventSuccessIntervals(long executionTime);
	acBinEventSuccessIntervals(const acBinEventSuccessIntervals& oldCommand);

	virtual ~acBinEventSuccessIntervals();
	
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

    inline long   GetBinTotal()    const {return m_BinTotal;}
    inline double GetBinWidth()    const {return m_BinWidth;}
    inline long   GetSampleTotal() const {return m_Samples;}

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

    long m_BinTotal;   // Number of bins in the histogram
    long m_Samples;    // Number of SamplePeriods over which to construct histogram
    double m_BinWidth; // Fixed width of bins if number of bins is not specified


};

#endif // !defined(AFX_ACBINEVENTSUCCESSINTERVALS_H__A997F03B_5D19_4685_B8A3_D80E126C5334__INCLUDED_)

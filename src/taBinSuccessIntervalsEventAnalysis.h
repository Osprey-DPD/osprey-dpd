// taBinSuccessIntervalsEventAnalysis.h: interface for the taBinSuccessIntervalsEventAnalysis class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABINSUCCESSINTERVALSEVENTANALYSIS_H__DA4CA7E6_94E5_4BF0_987D_0076685930EB__INCLUDED_)
#define AFX_TABINSUCCESSINTERVALSEVENTANALYSIS_H__DA4CA7E6_94E5_4BF0_987D_0076685930EB__INCLUDED_



// Forward declarations

class taBinTimeIntervals;
class taSerializeHistogram;


#include "taEventAnalysisDecorator.h"

class taBinSuccessIntervalsEventAnalysis : public taEventAnalysisDecorator
{
public:
	// ****************************************
	// Construction/Destruction
public:

	taBinSuccessIntervalsEventAnalysis(const zString label, CCommandTargetNode* const pDec, long bins, double binWidth, long samples);

	virtual ~taBinSuccessIntervalsEventAnalysis();

    // ****************************************
	// Global functions, static member functions and variables
public:

    static const zString GetType();		// return the target's type

private:

	static const zString m_Type;

	// ****************************************
	// PVFs that must be implemented by all instantiated derived classes 
public:

    const zString GetTargetType() const;    // return the target's type

    // Implementation of the ISerialiseInclusiveRestartState interface
    // to allow this class to read/write data that can be modified
    // for restarts.

    virtual zInStream& Read(zInStream& is);
    virtual zOutStream& Write(zOutStream& is) const;

	// ****************************************

	// Functions used by other decorator classes

	virtual void Execute(long simTime);

    // Function to return a pointer to the interface that this EAD requires

    virtual IEAD* ConnectESDInterface(taEventSourceDecorator* const pESD);

    virtual void AnalyseEventState();

	virtual void Normalise();

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

protected:

private:

    long    m_BinTotal;     // Number of bins in histogram
    double  m_BinWidth;     // Fixed width of bins in histogram. Note that one of these two must be zero.

    taBinTimeIntervals*   m_pHistogram;  // Histogram object to perform binning
    taSerializeHistogram* m_pFile;      // Label decorator to write histogram data to file
};

#endif // !defined(AFX_TABINSUCCESSINTERVALSEVENTANALYSIS_H__DA4CA7E6_94E5_4BF0_987D_0076685930EB__INCLUDED_)

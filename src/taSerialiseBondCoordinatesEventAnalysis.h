// taSerialiseBondCoordinatesEventAnalysis.h: interface for the taSerialiseBondCoordinatesEventAnalysis class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASERIALISEBONDCOORDINATESEVENTANALYSIS_H__CFA5FA3F_F9E5_4C40_A2C1_5161C9AC95A5__INCLUDED_)
#define AFX_TASERIALISEBONDCOORDINATESEVENTANALYSIS_H__CFA5FA3F_F9E5_4C40_A2C1_5161C9AC95A5__INCLUDED_



// Forward declarations

class taTimeSeriesBondCoordinates;
class taSerializeTimeSeries;


#include "taEventAnalysisDecorator.h"

class taSerialiseBondCoordinatesEventAnalysis : public taEventAnalysisDecorator
{
public:
	// ****************************************
	// Construction/Destruction
public:

	taSerialiseBondCoordinatesEventAnalysis(const zString label, CCommandTargetNode* const pDec, long period);

	virtual ~taSerialiseBondCoordinatesEventAnalysis();

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

    long m_PreviousSampleTotal;   // Previous number of data points in the time series

    taTimeSeriesBondCoordinates* m_pTimeSeries;  // Time series instance to store data
    taSerializeTimeSeries* m_pFile;              // Label decorator to write time series data to file
};

#endif // !defined(AFX_TASERIALISEBONDCOORDINATESEVENTANALYSIS_H__CFA5FA3F_F9E5_4C40_A2C1_5161C9AC95A5__INCLUDED_)

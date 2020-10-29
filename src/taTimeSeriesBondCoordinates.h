// taTimeSeriesBondCoordinates.h: interface for the taTimeSeriesBondCoordinates class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TATIMESERIESBONDCOORDINATES_H__2CD97901_EEEF_4486_9A9B_E323016A7A6C__INCLUDED_)
#define AFX_TATIMESERIESBONDCOORDINATES_H__2CD97901_EEEF_4486_9A9B_E323016A7A6C__INCLUDED_



#include "taLabelDecorator.h"

class taTimeSeriesBondCoordinates : public taLabelDecorator
{
public:
	// ****************************************
	// Construction/Destruction - protected constructor declared below
public:

    taTimeSeriesBondCoordinates(const zString label, CCommandTargetNode* const pDec);

	virtual ~taTimeSeriesBondCoordinates();

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
	// Public access functions
public:

    // We assume that the x, y, z containers have the same size.

    inline long GetSampleTotal() const {return m_XData.size();}

    // Functions to return the x, y, z coordinates of an active bond.

    double GetXPos(long i) const;
	double GetYPos(long i) const;
	double GetZPos(long i) const;
    
    double GetLastXPos() const;
	double GetLastYPos() const;
	double GetLastZPos() const;
    
    // Functions to initialize the time series, add a new data point and
    // serialize it to file. Note that we use the "Normalise" function to
    // actually write the data to the file.

	void Reset();
	void AddDataPoint(double x, double y, double z);
	void Normalise();

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

    // Local data

    zDoubleVector m_XData; // Time series data
    zDoubleVector m_YData; // Time series data
    zDoubleVector m_ZData; // Time series data
};

#endif // !defined(AFX_TATIMESERIESBONDCOORDINATES_H__2CD97901_EEEF_4486_9A9B_E323016A7A6C__INCLUDED_)

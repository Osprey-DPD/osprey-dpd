// taSerializeTimeSeries.h: interface for the taSerializeTimeSeries class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASERIALIZETIMESERIES_H__26CDC620_2E1E_4753_BA73_E96B21F80AFA__INCLUDED_)
#define AFX_TASERIALIZETIMESERIES_H__26CDC620_2E1E_4753_BA73_E96B21F80AFA__INCLUDED_


// Forward declarations

class taTimeSeriesBondCoordinates;


#include "taFileLabel.h"

class taSerializeTimeSeries : public taFileLabel
{
public:
	// ****************************************
	// Construction/Destruction
public:

	taSerializeTimeSeries(const zString fileName, const zString label, 
                CCommandTargetNode* const pDec);

	virtual ~taSerializeTimeSeries();

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


    bool Serialize();


	// ****************************************
	// Public access functions
public:

	void SetTimeSeries(taTimeSeriesBondCoordinates* const pTimeSeries);


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

    taTimeSeriesBondCoordinates* m_pTimeSeries;   // Pointer to the time series instance

};

#endif // !defined(AFX_TASERIALIZETIMESERIES_H__26CDC620_2E1E_4753_BA73_E96B21F80AFA__INCLUDED_)

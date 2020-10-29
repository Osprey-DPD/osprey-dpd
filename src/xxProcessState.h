// xxProcessState.h: interface for the xxProcessState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXPROCESSSTATE_H__16913C30_C267_4F04_95F6_52E90D4A12A1__INCLUDED_)
#define AFX_XXPROCESSSTATE_H__16913C30_C267_4F04_95F6_52E90D4A12A1__INCLUDED_

// Forward declarations

class CTimeSeriesData;
class xxProcess;


#include "xxState.h"

class xxProcessState : public xxState  
{
	// ****************************************
	// Construction/Destruction
public:

	xxProcessState(const zString fileName, long startTime, long endTime);

	xxProcessState(const zString fileName, long startTime, long endTime, 
				   const zString runId, const zString type);

	virtual ~xxProcessState();

	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an xxProcess pointer,
	// and respond to the SimBox's request to sample their data.


	// ****************************************
	// Public access functions
public:

	bool Serialize();

	void AddTimeSeriesData(CTimeSeriesData *pTSD);

	zOutStream&  putASCIIStartTags();
	zOutStream&  putASCIIEndTags();

	zOutStream&  putXMLStartTags();
	zOutStream&  putXMLEndTags();

	void WriteRawEndline();
	void WriteRawData(long x);
	void WriteRawData(double x);
	void WriteRawData(long x, long y);
	void WriteRawData(double x, double y);

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

private:

	const zString m_Type;		// String identifier for associated process

	long m_Start;				// Simulation time at which process starts
	long m_End;					// Simulation time at which process ends

	long m_SamplesSerialized;

	TimeSeriesSequence m_vTimeSeries;

};

#endif // !defined(AFX_XXPROCESSSTATE_H__16913C30_C267_4F04_95F6_52E90D4A12A1__INCLUDED_)

// DensityFieldState.h: interface for the CDensityFieldState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DENSITYFIELDSTATE_H__BFD34FE7_1FD1_4FCF_8E64_F9425ABC18C9__INCLUDED_)
#define AFX_DENSITYFIELDSTATE_H__BFD34FE7_1FD1_4FCF_8E64_F9425ABC18C9__INCLUDED_


#include "xxState.h"

class CDensityFieldState : public xxState  
{
	// ****************************************
	// Construction/Destruction
public:

	CDensityFieldState(const zString runId, long currentTime, const zString acnType, const zString dim,
					   long start, long end,long sample);

	virtual ~CDensityFieldState();

	// ****************************************
	// Global functions, static member functions and variables
public:

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	virtual void Sample()	 = 0;
	virtual void Normalize() = 0;
	virtual bool Serialize() = 0;

	// ****************************************
	// Public access functions
public:

	inline const zString GetDataType()  const {return m_DataType;}
	inline long GetStartTime()			const {return m_Start;}
	inline long GetEndTime()			const {return m_End;}
	inline long GetSamplePeriod()		const {return m_SamplePeriod;}

    inline bool IsTimeToNormalize()     const {return m_bNormalize;}

	bool IsTimeToSample(long currentTime);

	// ****************************************
	// Protected local functions
protected:

    inline long GetExpectedSamples() const {return m_ExpectedSamples;}
    inline void NormalizationDone()        {m_bNormalize = false;}

	// ****************************************
	// Implementation

protected:

	zDoubleVector	m_Data;	// Density field data for serialization	

    long m_ActualSamples;  // Actual number of samples stored here for convenience

	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

	// Data common to 1,2 and 3 d density fields

	const zString m_DataType;	// Identifier for the objects being sampled
	long	m_Start;			// Time at which density field analysis starts
	long	m_End;				// Time at which density field analysis ends
	long	m_SamplePeriod;		// Number of time steps between data samples

    long    m_ExpectedSamples;  // Number of samples within sampling period
    bool    m_bNormalize;       // Flag showing if density field should be erased

};

#endif // !defined(AFX_DENSITYFIELDSTATE_H__BFD34FE7_1FD1_4FCF_8E64_F9425ABC18C9__INCLUDED_)

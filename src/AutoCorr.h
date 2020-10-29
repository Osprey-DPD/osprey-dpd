// AutoCorr.h: interface for the CAutoCorr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUTOCORR_H__04ED53A9_5F5E_4732_BB20_9DBD2AAC8EB1__INCLUDED_)
#define AFX_AUTOCORR_H__04ED53A9_5F5E_4732_BB20_9DBD2AAC8EB1__INCLUDED_


#include "xxBase.h"

class CAutoCorr : public xxBase  
{

	// ****************************************
	// Construction/Destruction
public:

	CAutoCorr(long n, long m);

	virtual ~CAutoCorr();

	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:


	// ****************************************
	// Public access functions
public:

	void AddSample(double x);

	inline long GetMaxValues()   const {return m_N;}
	inline long GetMaxTimeLag()  const {return m_M;}
	inline long GetSampleTotal() const {return m_SampleIndex;}

	double GetAutoCorrValue(long index);

	zDoubleVector GetAutoCorr();

	zLongVector GetSampleTotals();

	// ****************************************
	// Protected local functions
protected:


	


	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:

	// Explicitly disallow the copy constructor and assignment operators
	// by declaring them privately but providing NO definitions.

	CAutoCorr(const CAutoCorr& old);
	CAutoCorr& operator=(const CAutoCorr& rhs);

	// Helper function to normalize the autocorrelation function when all
	// samples have been added.

	void Normalize();


	// ****************************************
	// Data members
private:

	const long m_N;					// Max number of samples x(n)
	const long m_M;					// Max time separation of samples

	bool m_bNormalized;				// Flag showing when calculation is over
	long m_SampleIndex;				// Current sample index

	zLongVector    m_SampleTotals;	// Number of samples in m_ProductSum
	zDoubleVector  m_Values;		// Sequence of x(n) values for n=0,1,2,...
	zDoubleVector  m_ProductSum;	// Sum of x(n).x(n+m) values for n,m=0,1,2,...


};

#endif // !defined(AFX_AUTOCORR_H__04ED53A9_5F5E_4732_BB20_9DBD2AAC8EB1__INCLUDED_)

// taBinTimeIntervals.h: interface for the taBinTimeIntervals class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABINTIMEINTERVALS_H__DDA01256_8832_45EC_8067_19F09E9F5182__INCLUDED_)
#define AFX_TABINTIMEINTERVALS_H__DDA01256_8832_45EC_8067_19F09E9F5182__INCLUDED_



#include "taLabelDecorator.h"

class taBinTimeIntervals : public taLabelDecorator
{
public:
	// ****************************************
	// Construction/Destruction - protected constructor declared below
public:

    taBinTimeIntervals(const zString label, CCommandTargetNode* const pDec, bool bBinTotal);

	virtual ~taBinTimeIntervals();


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

    inline long   GetSampleTotal()   const {return m_Data.size();}
    inline long      GetBinTotal()   const {return m_BinTotal;}
    inline long    GetLowerBound()   const {return m_XMin;}
    inline long    GetUpperBound()   const {return m_XMax;}
    inline double    GetBinWidth()   const {return m_XWidth;}

    // Functions to return the contents of a bin as an integer and
    // as a fraction of the total contents.

	long   GetBinNumber(long i)   const;
	double GetBinFraction(long i) const;
    
    // Functions to initialize the histogram calculation, add data and normalize it
    // when required. We provide two overloaded functions to reset the histogram.
    // The first requires the number of bins and calculates their width from the data,
    // the second requires the bin width and calculates the number of bins from the data.

	void ResetFixedBinTotal(long bins);
	void ResetFixedBinWidth(double width);
	void AddDataPoint(long x);
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

    // Input data

    const bool m_bFixedBinTotal; // Flag showing if the number of bins or their width 
                                 // is fixed by the calling routine.
    // Local data

    long m_BinTotal;    // Number of bins in the histogram
    long m_XMin;        // Minimum value of the variable x (inclusive)
    long m_XMax;        // Maximum value of the variable x (inclusive)
    long m_OldValue;    // Previous value of time needed to calculate the interval
    double m_XWidth;    // Width of each bin


    zLongVector    m_Data;       // Unsorted positive INTEGER data
    zLongVector    m_Bins;       // Number of values of x in each bin
};


#endif // !defined(AFX_TABINTIMEINTERVALS_H__DDA01256_8832_45EC_8067_19F09E9F5182__INCLUDED_)

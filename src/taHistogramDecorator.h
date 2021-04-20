// taHistogramDecorator.h: interface for the taHistogramDecorator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TAHISTOGRAMDECORATOR_H__b5ad02c7_a160_4156_abd6_231288c9b706__INCLUDED_)
#define AFX_TAHISTOGRAMDECORATOR_H__b5ad02c7_a160_4156_abd6_231288c9b706__INCLUDED_



#include "taLabelDecorator.h"

class taHistogramDecorator : public taLabelDecorator
{
public:
	// ****************************************
	// Construction/Destruction - protected constructor declared below
public:

    taHistogramDecorator(const zString label, CCommandTargetNode* const pDec);

	virtual ~taHistogramDecorator();


    // ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be implemented by all instantiated derived classes 
public:
    
    // PVFs implemented by derived classes to define how they calculate the histogram
    
    virtual void AddDataPoint(double x) = 0;
    virtual void Normalise(double norm) = 0;

    // Implementation of the ISerialiseInclusiveRestartState interface
    // to allow this class to read/write data that can be modified
    // for restarts.

    virtual zInStream& Read(zInStream& is);
    virtual zOutStream& Write(zOutStream& is) const;

    // ****************************************
	// Public access functions
public:


    inline long      GetSampleTotal()  const {return m_Data.size();}
    inline long      GetBinTotal()     const {return m_Bins.size();}
    inline double    GetLowerBound()   const {return m_XMin;}
    inline double    GetUpperBound()   const {return m_XMax;}
    inline double    GetBinWidth()     const {return m_XWidth;}

    // Functions to return the contents of a bin as an integer and
    // as a fraction of the total contents.

	long   GetBinNumber(long i)   const;
	double GetBinFraction(long i) const;
 
	// ****************************************
	// Protected local functions
protected:

    // Function to allow derived classes to define their own normalisation for the stored histogram.
    
    void   SetBinFraction(long i, double newValue);


	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members

protected:

    // Local data

    double m_XMin;      // Minimum value of the variable x (inclusive)
    double m_XMax;      // Maximum value of the variable x (inclusive)
    double m_XWidth;    // Width of each bin


    zDoubleVector  m_Data;          // Unsorted positive real data
    zLongVector    m_Bins;          // Number of values of x in each bin
    zDoubleVector  m_BinFractions;  // Normalised values of the binned data
};


#endif // !defined(AFX_TAHISTOGRAMDECORATOR_H__b5ad02c7_a160_4156_abd6_231288c9b706__INCLUDED_)

// taBinCylinderSpaceCoordinates.h: interface for the taBinCylinderSpaceCoordinates class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABINCYLINDERSPACECOORDINATES_H__b78c3fcf_52d7_4402_8a7f_7c63881277d4__INCLUDED_)
#define AFX_TABINCYLINDERSPACECOORDINATES_H__b78c3fcf_52d7_4402_8a7f_7c63881277d4__INCLUDED_



#include "taHistogramDecorator.h"

class taBinCylinderSpaceCoordinates : public taHistogramDecorator
{
public:
	// ****************************************
	// Construction/Destruction - protected constructor declared below
public:

    taBinCylinderSpaceCoordinates(const zString label, CCommandTargetNode* const pDec, long binTotal, double binWidth);

	virtual ~taBinCylinderSpaceCoordinates();


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

    // Functions to add data points and normalise the histogram.

    virtual void AddDataPoint(double x);
    virtual void Normalise(double norm);

// Implementation of the ISerialiseInclusiveRestartState interface
    // to allow this class to read/write data that can be modified
    // for restarts.

    virtual zInStream& Read(zInStream& is);
    virtual zOutStream& Write(zOutStream& is) const;

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

 };


#endif // !defined(AFX_TABINCYLINDERSPACECOORDINATES_H__b78c3fcf_52d7_4402_8a7f_7c63881277d4__INCLUDED_)

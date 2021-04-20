// taCylindricalDistribution.h: interface for the taCylindricalDistribution class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TACYLINDRICALDISTRIBUTION_H__aa086afc_8092_4de5_a0f2_3bcfa7258572__INCLUDED_)
#define AFX_TACYLINDRICALDISTRIBUTION_H__aa086afc_8092_4de5_a0f2_3bcfa7258572__INCLUDED_


// Forward declarations

class CCommandTargetPolymer;
class CCommandTargetComposite;
class taBinCylinderSpaceCoordinates;
class taSerializeHistogram;


#include "taCumulateDecorator.h"

class taCylindricalDistribution : public taCumulateDecorator
{
public:
	// ****************************************
	// Construction/Destruction
public:

	taCylindricalDistribution(const zString label, CCommandTargetNode* const pDec, long start, long end,
                         long samples, long xn, long yn, long zn, double xc, double yc, double zc,
                         double shellWidth, double outerRadius);

	virtual ~taCylindricalDistribution();

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

	// ****************************************
	// Public access functions
public:



	// ****************************************
	// Protected local functions
protected:

    void Normalise();

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:

    // Helper function to do the calculation for a polymer target
    
    void CalculatePolymerCM(long simTime);
    
    // Helper function to do the calculation for a composite target that holds a set of simple targets
    
    void CalculateTargetCM(long simTime);


	// ****************************************
	// Data members

protected:

private:

    // Data common to both types of calculation

    const long    m_Samples;               // Multiple of "SamplePeriod" values between binning the data
    const long    m_XN, m_YN, m_ZN;        // Axis about which to calculate distribution (must be X, Y or Z)
    const double  m_XC, m_YC, m_ZC;        // Single point on the axis (units of r0)
    const double  m_ShellWidth;            // Width of the cylindrical shells (units of r0)
    const double  m_OuterRadius;           // Max width of cylindrical shells (units of r0); inner width is 0

    long           m_BinTotal;             // Number of binds in the histogram
    long           m_TimesCalled;          // Number of times the histogram has had data added
    double         m_ShellArea;            // Area of a cylindrical shell needed for normalisation

    taBinCylinderSpaceCoordinates*  m_pHistogram;  // Histogram object to perform binning
    taSerializeHistogram*           m_pFile;          // Label decorator to write histogram data to file

    // Data needed for polymer target calculation
    
    CCommandTargetPolymer*    m_pPolymerTarget;    // Pointer to a simple polymer target

    PolymerVector  m_vPolymers;
    
    // Data needed for composite target calculation
    
    CCommandTargetComposite*  m_pCompositeTarget;  // Pointer to a composite target containing simple bead/polymer targets

    CommandTargetSequence     m_Targets;           // List of simple targets contained in the composite target
 };

#endif // !defined(AFX_TACYLINDRICALDISTRIBUTION_H__aa086afc_8092_4de5_a0f2_3bcfa7258572__INCLUDED_)

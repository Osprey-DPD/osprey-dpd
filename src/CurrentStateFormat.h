// CurrentStateFormat.h: interface for the CCurrentStateFormat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CURRENTSTATEFORMAT_H__CA965943_AEE4_4F08_B39B_3A604000D13E__INCLUDED_)
#define AFX_CURRENTSTATEFORMAT_H__CA965943_AEE4_4F08_B39B_3A604000D13E__INCLUDED_


// Include file needed for access to the stream objects

#include "xxBase.h"

class CCurrentStateFormat  
{
	// ****************************************
	// Construction/Destruction: protected constructor declared below
public:

	virtual ~CCurrentStateFormat();

	// ****************************************
	// Global functions, static member functions and variables
public:

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// Function to return the file extension used by derived classes
	// to specify their output files.

	virtual const zString GetFileExtension() const = 0;

	// Return true if this format needs to know about bonds
	virtual bool UsesBonds() const;

	// Function to ensure that derived classes can write their data to file

	virtual void SerializeHeader(zOutStream& os, const long beadTotal) = 0;
	virtual void SerializeBead(zOutStream& os, const zString name, const long type, const double radius,
								const double x, const double y, const double z) = 0;
	virtual void SerializeBond(zOutStream& os, const CPolymer &polymer, const CBond &bond);
	virtual void SerializeFooter(zOutStream& os, const long beadTotal);

	// ****************************************
	// Public access functions
public:


	// ****************************************
	// Protected local functions
protected:

	// Protected constructor ensures that only subclasses can be instantiated

	CCurrentStateFormat(double lx, double ly, double lz, bool bDisplayBox,
						const long beadTypeTotal);

    // Function that returns a named colour for a given bead type

    const zString GetBeadColourFromType(long beadType) const;

	
	// ****************************************
	// Implementation

protected:

	// Data needed by derived classes for output but we put it here to avoid
	// duplicating it in each class

	const double m_SimBoxXLength;	// CSimBox side lengths in units of bead diameters
	const double m_SimBoxYLength; 
	const double m_SimBoxZLength; 

	bool		 m_bDisplayBox;		// Flag showing whether to draw a bounding box

	double		 m_BLBoxCoord[3];	// Bounding box coordinates
	double		 m_TRBoxCoord[3];

	const long   m_BeadTypeTotal;	// Number of bead types

	StringSequence			m_BeadColours;

	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:



};

#endif // !defined(AFX_CURRENTSTATEFORMAT_H__CA965943_AEE4_4F08_B39B_3A604000D13E__INCLUDED_)

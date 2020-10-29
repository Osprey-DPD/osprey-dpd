// StressGridCell.h: interface for the CStressGridCell class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STRESSGRIDCELL_H__4698109D_4D07_4860_8FED_E6AB37ECC3DF__INCLUDED_)
#define AFX_STRESSGRIDCELL_H__4698109D_4D07_4860_8FED_E6AB37ECC3DF__INCLUDED_



// Forward declarations



// Include file to allow calculation of stress tensor in curvilinear coordinates
#include "SimMiscellaneousFlags.h"


#include "AbstractCell.h"

class CStressGridCell : public CAbstractCell  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	CStressGridCell();

	CStressGridCell(long index, long coords[3], double dx, double dy, double dz);

    // Copy constructor
    CStressGridCell(const CStressGridCell& oldCell);

	virtual ~CStressGridCell();

	// ****************************************
	// Global functions, static member functions and variables
public:

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:


	// ****************************************
	// Public access functions
public:

	// Functions to access the local stress tensor
	inline double GetStress11() const {return m_aStressSphere[0];}
	inline double GetStress22() const {return m_aStressSphere[4];}
	inline double GetStress33() const {return m_aStressSphere[8];}
	inline double GetStressComponent(long i) const {return m_aStressSphere[i];}
	
	// Function to write out the components of the stress tensor
	void WriteStressTensor() const;

	// Function to zero all components of the stress tensor
	void Clear();

	// Function to add a stress to the local stress tensor
	void AddStress(double stress[9]);

	// Function to transform the local tensor into spherical polar coordinates
	void TransformStressToSphCoords(double origin[3]);

	// Function to transform the local tensor into cylindrical polar coordinates
	void TransformStressToCylCoords(double origin[3]);

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

    double m_aStressSphere[9];      // Stress tensor within current cell
	

};

#endif // !defined(AFX_STRESSGRIDCELL_H__4698109D_4D07_4860_8FED_E6AB37ECC3DF__INCLUDED_)

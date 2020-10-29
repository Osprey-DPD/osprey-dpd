// InitialStateData.h: interface for the CInitialStateData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INITIALSTATEDATA_H__0843C363_63B7_11D3_820E_0060088AD300__INCLUDED_)
#define AFX_INITIALSTATEDATA_H__0843C363_63B7_11D3_820E_0060088AD300__INCLUDED_



// Forward declarations

class CInputData;
class CInitialState;
class mpmInitialState;


// Include files

#include "SimMPSFlags.h"  // Needed for conditional compilation of parallel features in derived classes
#include "acfInitialStateFactory.h"	// Needed to create initial state objects


class CInitialStateData  
{
	// ****************************************
	// Construction/Destruction: protected constructor declared below
public:

	virtual ~CInitialStateData();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CInitialStateData& inState);
	friend zInStream&  operator>>(zInStream& is,        CInitialStateData& inState);

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	virtual zOutStream& put(zOutStream& os)		  const = 0;
	virtual zInStream&  get(zInStream& is)				= 0;

	virtual zString GetInitialStateType()		  const = 0;
	virtual bool Assemble(CInitialState&   riState) const = 0;

	virtual CInitialStateData* Copy()             const = 0;

    // Function to allow each ISD-derived class to set its data from the
    // corresponding mpmInitialState class during a parallel simulation.

    virtual void SetData(const mpmInitialState* const pMsg);


	// ****************************************
	// Public access functions
public:

    // Parallel code function to calculate how many polymers of each type must be created in each processor's space
	// in order to assemble the initial configuration
	
    virtual void CalculatePolymerFractionsP(double lx, double ly, double lz, 
	                                     double xorigin, double yorigin, double zorigin, 
	                                     double xspace, double yspace, double zspace, 
	                                     zDoubleVector& rvPolymerFractions) const;    

	bool IsDataValid() const;		   // Indicates whether data is valid or not

	// We provide a default ValidateData function (that just returns true) 
	// but each derived class should provide its own version if it wants 
	// to check the data before trying to assemble the initial state.

	virtual bool ValidateData(const CInputData& riData);

    // Parallel version of the above function to validate initial states
    // that contain structures that span the whole simulation Space.

    virtual bool ValidateDataP(const CInputData& riData);

	// ****************************************
	// Protected local functions
protected:

	CInitialStateData();		// Protected constructor prevents instantiation


	void SetDataValid(bool bValid);	// Allows derived classes to set their m_bDataValid flag



	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

	bool m_bDataValid;
};

#endif // !defined(AFX_INITIALSTATEDATA_H__0843C363_63B7_11D3_820E_0060088AD300__INCLUDED_)

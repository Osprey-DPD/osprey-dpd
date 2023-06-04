// CurrentState.h: interface for the CCurrentState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CURRENTSTATE_H__D70C3D02_3D5E_11D3_820E_0060088AD300__INCLUDED_)
#define AFX_CURRENTSTATE_H__D70C3D02_3D5E_11D3_820E_0060088AD300__INCLUDED_


// Forward declarations

class ISimBox;
class CCurrentStateFormat;

// Include files 

#include "pmCurrentState.h"   // Needed for the array sizes


#include "xxState.h"

class CCurrentState : public xxState  
{
	// ****************************************
	// Construction/Destruction: 
public:

	CCurrentState(const long currentTime, const zString runId, const ISimBox* pISimBox,
				  CCurrentStateFormat* const pFormat, bool bRestrictCoords, 
				  double xmin, double ymin, double zmin,
				  double xmax, double ymax, double zmax);

	virtual ~CCurrentState();


	// ****************************************
	// Global functions, static member functions and variables
public:

	static void ClearBeadDisplayIdMap();
	static long GetBeadDisplayIdMapSize();
    static LongLongMap& GetMap();

	static long GetBeadDisplayId(long beadId);
	static void SetBeadDisplayId(long beadId, long displayId);

private:

	// Map of (beadId, displayId) pairs that determines what colour each
	// bead is drawn in current state snapshots.

	static LongLongMap	m_mBeadDisplayId;

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

    // Serial code serialization function

	bool Serialize();

    // Parallel code functions

	bool SerializeP0();
	bool SerializePN(long beadTotal, long idArray[], long beadTypeArray[], double RadiusArray[], double XArray[], double YArray[], double ZArray[]);
	bool SerializeEnd();

	// ****************************************
	// Protected local functions
protected:

	// ****************************************
	// Implementation

protected:


	// ****************************************
	// Private functions
private:

	long CountBeadsDisplayed() const;

	// ****************************************
	// Data members
private:

	// Format object that determines the style of the output data

	CCurrentStateFormat* m_pFormat;

	
	// Local copy of bead vector from the CSimBox so that it contains any beads
	// that may have moved between processors

	const AbstractBeadVector	m_vAllBeads;
	const PolymerVector	       m_vAllPolymers;

	StringSequence				m_BeadNames;
	StringSequence				m_BeadColours;

	// We can no longer use a bool if we have more than two output format classes

	bool		m_bPovray;			// Flag showing whether to output in PovRay format

	bool		m_bRestrictCoords;	// Flags showing bead coordinates are restricted

	const double m_SimBoxXLength;   // Size of whole simulation Space: this is just
	const double m_SimBoxYLength;   // the SimBox size for the serial code but is
	const double m_SimBoxZLength;   // equal to the World size for the parallel code

	double		m_XMin;				// Range of bead coordinates to display
	double		m_YMin;				// in units of bead diameters: note that
	double		m_ZMin;				// the arguments to the constructor are
	double		m_XMax;				// specified as fractions of the SimBox size
	double		m_YMax;
	double		m_ZMax;

	// Povray format data
	double		m_Camera[3];		// Coordinates of camera for snapshot
	double		m_Target[3];		// Coordinates of target for snapshot

	zDoubleVector m_vLightX;		// X Coordinates of lights
	zDoubleVector m_vLightY;		// Y Coordinates of lights
	zDoubleVector m_vLightZ;		// Z Coordinates of lights

	// Other data stored here to simplify the output

	bool		m_bDisplayBox;		// Flag showing whether to draw a bounding box
	double		m_BLBoxCoord[3];
	double		m_TRBoxCoord[3];

};

#endif // !defined(AFX_CURRENTSTATE_H__D70C3D02_3D5E_11D3_820E_0060088AD300__INCLUDED_)

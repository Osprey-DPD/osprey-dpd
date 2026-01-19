// AbstractBead.h: interface for the CAbstractBead class.
//
// Note that this class cannot be instantiated and all its 
// derived classes must provide implementations for the functions
// that update a bead's essential properties: position, momentum, force.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ABSTRACTBEAD_H__12767EC0_3849_11D3_820E_0060088AD300__INCLUDED_)
#define AFX_ABSTRACTBEAD_H__12767EC0_3849_11D3_820E_0060088AD300__INCLUDED_


// Forward declarations

class CPolymer;

// Include file to allow conditional compilation of bead properties for different
// simulation types.

#include "SimDefs.h"
#include "ExperimentDefs.h"
#include "SimMPSFlags.h"

class CAbstractBead  
{
	// friend classes need access to bead coordinates in order to
	// calculate forces between bonded beads, and observables
	
	friend class aaStressTensor1d;
	friend class aeCNTCell;			// Active SimBox needs to update forces
	friend class CBeadChargeWrapper;
	friend class CBond;
	friend class CBondPair;
	friend class CCNTCell;
	friend class CExternalCNTCell;
	friend class CForceTarget;
	friend class CMonitor;
    friend class CPolymer;  //BIOENG 2025 needed to add Morces forces
	friend class CSimBox;
	friend class taForceDecorator;	// Needed by command target force decorators

// Friend classes for parallel code
#if EnableParallelSimBox == SimMPSEnabled
    friend class mpsCNTCell;
    friend class mpmMonitor;
    friend class mpsSimBox;
	friend class mpuGhostBead;
	friend class mpuExtendedBond;
#endif

public:
	CAbstractBead();
	CAbstractBead(long type);
	CAbstractBead(long id, long type, bool movable, double x0[3], double v0[3] );
	CAbstractBead(long id, long type, bool movable, double radius,
				  double x0[3], double v0[3] );

#if EnableDPDLG == ExperimentEnabled
	CAbstractBead(long id, long type, bool movable, double radius, double lgRadius,
				  double x0[3], double v0[3] );
#endif

#if EnableParallelSimBox == SimMPSEnabled
    CAbstractBead(CPolymer* const pPolymer, long id, long type, bool movable, double radius,
                             double x0[3], double v0[3] );
#endif

	CAbstractBead(const CAbstractBead& oldBead);

	virtual ~CAbstractBead();

	// Public access functions to bead state and coordinates
    
    // BIOENG 455 Function to add the Morse forces to a bead
    
    void AddHelixForces(const double fx, const double fy, const double fz);

	inline long	  GetId()		const {return m_id;}
	inline long	  GetType()		const {return m_Type;}
	inline bool	  GetVisible()	const {return m_bIsVisible;}
	inline bool	  GetMovable()	const {return m_bIsMovable;}
	inline bool	  GetFrozen()	const {return m_bIsFrozen;}
	inline double GetRadius()	const {return m_Radius;}

#if EnableDPDLG == ExperimentEnabled
	inline double GetLGRadius()	    const {return m_LGRadius;}
	inline double GetLGDensity()    const {return m_LGDensity;}
#endif

#if SimIdentifier == BD
	inline double GetTransDiff()	const {return m_TransDiff;}
	inline double GetRotDiff()	    const {return m_RotDiff;}
#endif

#if EnableParallelSimBox == SimMPSEnabled
	void SetOwningPolymer(CPolymer* pPolymer);

    inline CPolymer* GetPolymer()   const  {return m_pPolymer;}
#endif

    inline double GetXPos()		const {return m_Pos[0];}
	inline double GetYPos()		const {return m_Pos[1];}
	inline double GetZPos()		const {return m_Pos[2];}

	inline double GetXMom()		const {return m_Mom[0];}
	inline double GetYMom()		const {return m_Mom[1];}
	inline double GetZMom()		const {return m_Mom[2];}

	inline double GetXForce()	const {return m_Force[0];}
	inline double GetYForce()	const {return m_Force[1];}
	inline double GetZForce()	const {return m_Force[2];}

	inline double GetXAngMom()	const {return m_AngMom[0];}
	inline double GetYAngMom()	const {return m_AngMom[1];}
	inline double GetZAngMom()	const {return m_AngMom[2];}

	inline double GetunPBCXPos()	const {return m_unPBCPos[0];}
	inline double GetunPBCYPos()	const {return m_unPBCPos[1];}
	inline double GetunPBCZPos()	const {return m_unPBCPos[2];}
	inline double GetInitialXPos()	const {return m_InitialPos[0];}
	inline double GetInitialYPos()	const {return m_InitialPos[1];}
	inline double GetInitialZPos()	const {return m_InitialPos[2];}

    // Functions to returned bead coordinates shifted to whole simulation Space

    double GetShiftedXPos() const;
	double GetShiftedYPos() const;
    double GetShiftedZPos() const;

    // Function to return the beads' kinetic energy

    double GetKE() const;


	// Coordinates at previous time step

	inline double GetOldXPos()		const {return m_oldPos[0];}
	inline double GetOldYPos()		const {return m_oldPos[1];}
	inline double GetOldZPos()		const {return m_oldPos[2];}

	inline double GetOldXMom()		const {return m_oldMom[0];}
	inline double GetOldYMom()		const {return m_oldMom[1];}
	inline double GetOldZMom()		const {return m_oldMom[2];}

	inline double GetOldXForce()	const {return m_oldForce[0];}
	inline double GetOldYForce()	const {return m_oldForce[1];}
	inline double GetOldZForce()	const {return m_oldForce[2];}

	// Differential changes in coordinates

	// Check that unPBC and PBC coords give the same results

	inline double GetdXPos()	const {return m_dPos[0];}
	inline double GetdYPos()	const {return m_dPos[1];}
	inline double GetdZPos()	const {return m_dPos[2];}

	inline double GetdXPos1()	const {return m_Pos[0] - m_oldPos[0];}
	inline double GetdYPos1()	const {return m_Pos[1] - m_oldPos[1];}
	inline double GetdZPos1()	const {return m_Pos[2] - m_oldPos[2];}

	inline double GetdXMom()	const {return m_Mom[0] - m_oldMom[0];}
	inline double GetdYMom()	const {return m_Mom[1] - m_oldMom[1];}
	inline double GetdZMom()	const {return m_Mom[2] - m_oldMom[2];}

	inline double GetdXForce()	const {return m_Force[0] - m_oldForce[0];}
	inline double GetdYForce()	const {return m_Force[1] - m_oldForce[1];}
	inline double GetdZForce()	const {return m_Force[2] - m_oldForce[2];}

	inline long GetForceCounter() const {return m_ForceCounter;}

	// Public functions to set bead state and coordinates

	inline void   SetId(long id)			{m_id			= id;}
	inline void   SetType(long type)		{m_Type			= type;}
	inline void   SetInvisible()			{m_bIsVisible	= false;}
	inline void   SetVisible()				{m_bIsVisible	= true;}
	inline void   SetVisible(bool bVisible)	{m_bIsVisible	= bVisible;}
	inline void   SetRadius(double radius)	{m_Radius		= radius;}

#if EnableDPDLG == ExperimentEnabled
	inline void   SetLGRadius(double radius)   {m_LGRadius	= radius;}
	inline void   SetLGDensity(double density) {m_LGDensity	= density;}
#endif

#if SimIdentifier == BD
	inline void   SetTransDiff(double diff)	{m_TransDiff    = diff;}
	inline void   SetRotDiff(double diff)	{m_RotDiff      = diff;}
#endif

	inline void   SetXPos(double x)			{m_Pos[0] = x;}
	inline void   SetYPos(double y)			{m_Pos[1] = y;}
	inline void   SetZPos(double z)			{m_Pos[2] = z;}
	inline void   SetXMom(double x)			{m_Mom[0] = x;}
	inline void   SetYMom(double y)			{m_Mom[1] = y;}
	inline void   SetZMom(double z)			{m_Mom[2] = z;}
	inline void   SetXForce(double x)		{m_Force[0] = x;}
	inline void   SetYForce(double y)		{m_Force[1] = y;}
	inline void   SetZForce(double z)		{m_Force[2] = z;}
	inline void	  SetunPBCXPos(double x)	{m_unPBCPos[0] = x;}
	inline void   SetunPBCYPos(double y)	{m_unPBCPos[1] = y;}
	inline void   SetunPBCZPos(double z)	{m_unPBCPos[2] = z;}
	inline void   SetInitialXPos(double x)	{m_InitialPos[0] = x;}
	inline void   SetInitialYPos(double y)	{m_InitialPos[1] = y;}
	inline void   SetInitialZPos(double z)	{m_InitialPos[2] = z;}
	inline void   SetdXPos(double x)		{m_dPos[0] = x;}
	inline void   SetdYPos(double y)		{m_dPos[1] = y;}
	inline void   SetdZPos(double z)		{m_dPos[2] = z;}

	// Functions used in parallel code to to set bead properties when they pass
	// between processors

	inline void   SetOldXPos(double x)		{m_oldPos[0] = x;}
	inline void   SetOldYPos(double y)		{m_oldPos[1] = y;}
	inline void   SetOldZPos(double z)		{m_oldPos[2] = z;}
	inline void   SetOldXMom(double x)		{m_oldMom[0] = x;}
	inline void   SetOldYMom(double y)		{m_oldMom[1] = y;}
	inline void   SetOldZMom(double z)		{m_oldMom[2] = z;}
	inline void   SetOldXForce(double x)	{m_oldForce[0] = x;}
	inline void   SetOldYForce(double y)	{m_oldForce[1] = y;}
	inline void   SetOldZForce(double z)	{m_oldForce[2] = z;}

	// PVFs to force derived classes to implement beads' ability to move and be frozen

	virtual bool  SetMovable()		= 0;	// must be provided by derived classes
	virtual bool  SetNotMovable()	= 0;	// must be provided by derived classes
	virtual bool  SetFrozen()		= 0;	// must be provided by derived classes
	virtual bool  SetNotFrozen()	= 0;	// must be provided by derived classes

protected:

	long m_id;				// member variable order here sets order of initialisation
	long m_Type;			// in the constructor
	bool m_bIsVisible;		// Indicates a bead is written to current state snapshots
	bool m_bIsMovable;		// Indicates a bead can move in the current time step
	bool m_bIsFrozen;		// Indicates a bead has been frozen in place		

	long   m_ForceCounter;  // Debug variable to count how many interactions a bead has 

	double m_Radius;		// Interaction radius

#if EnableDPDLG == ExperimentEnabled
	double m_LGRadius;		// Interaction radius for density-dependent LG force
	double m_LGDensity;		// Local bead density for density-dependent LG force
#endif

#if SimIdentifier == BD
    double m_TransDiff;     // Translational diffusion coefficient for BD beads
    double m_RotDiff;       // Rotational diffusion coefficient for BD beads
#endif

	double m_Pos[3];		// Current coordinates
	double m_Mom[3];
	double m_Force[3];
	double m_AngMom[3];

	double m_oldPos[3];		// Coordinates at previous time step
	double m_oldMom[3];
	double m_oldForce[3];

	double m_unPBCPos[3];
	double m_InitialPos[3];
	double m_dPos[3];		// Differential position coordinates
	double m_Stress[9];
    
#if EnableParallelSimBox == SimMPSEnabled
    CPolymer* m_pPolymer;   // Parent polymer needed for trans-processor messaging
#endif

};

#endif // !defined(AFX_ABSTRACTBEAD_H__12767EC0_3849_11D3_820E_0060088AD300__INCLUDED_)

// InitialStateLamella.h: interface for the CInitialStateLamella class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INITIALSTATELAMELLA_H__6CE669F4_04B5_11D4_BF30_004095086186__INCLUDED_)
#define AFX_INITIALSTATELAMELLA_H__6CE669F4_04B5_11D4_BF30_004095086186__INCLUDED_




#include "InitialStateData.h"

class CInitialStateLamella : public CInitialStateData  
{
public:
	CInitialStateLamella();
	virtual ~CInitialStateLamella();

	CInitialStateLamella& operator=(const CInitialStateLamella& oldISL);

	zOutStream& put(zOutStream& os) const;		// overrides the virtual base function
	zInStream&  get(zInStream&  is);			// overrides the virtual base function

	virtual zString GetInitialStateType() const;
	virtual bool Assemble(CInitialState& riState) const;
	virtual CInitialStateData* Copy() const;

	void SetData(const mpmInitialState *const pMsg);

	static const zString GetType();

    // Parallel code function to calculate how many polymers of each type must be created in each processor's space
	// in order to assemble the initial configuration
	
    virtual void CalculatePolymerFractionsP(double lx, double ly, double lz, double xorigin, double yorigin, double zorigin, double xspace, double yspace, double zspace, 
	                                     zDoubleVector& rvPolymerFractions) const;    
	
    // Public access functions to the IS data

    inline const zString GetPolymer()       const {return m_Polymer;}
    inline bool          GetLinearise()     const {return m_bLinearise;}
    inline bool          GetPolymerise()    const {return m_bPolymerise;}
    inline long          GetXNormal()       const {return m_X;}
    inline long          GetYNormal()       const {return m_Y;}
    inline long          GetZNormal()       const {return m_Z;}
    inline double        GetCentre()        const {return m_Centre;}
    inline double        GetThickness()     const {return m_Thickness;}
    inline double        GetUpperFraction() const {return m_UpperFraction;}


protected:
	virtual bool ValidateData(const CInputData& riData);
	virtual bool ValidateDataP(const CInputData& riData);

private:
	static const zString m_Type;		// Keyword used to identify initial state type

	zString m_Polymer;					// Polymer composing bilayer
	long	m_X;						
	long	m_Y;						// Bilayer normal: only x, y or z allowed
	long	m_Z;
	double	m_Centre;					// Bilayer centre as fraction of SimBox thickness
	double	m_Thickness;				// In units of bead diameters
	bool	m_bLinearise;				// Flag showing whether polymer is initially straightened
	double	m_UpperFraction;			// Fraction of polymer in upper monolayer

	// Data relating to a polymerised initial state
	bool	m_bPolymerise;				// Flag showing if initial state is polymerised
	zString	m_PolymerisedName;			// Name of polymerised polymer (for compatibility with CCompositeLamella)
	double  m_PolymerisedFraction;		// Fraction of polymers polymerised
	long	m_Position;					// Bead number in polymer which is polymerised
	double	m_SpringConstant;			// Hookean spring constant for polymerised bond
	double  m_UnstretchedLength;		// Unstretched length for polymerised bond


	// Local data

	long   m_PolymerType;				// Type of polymer composing lamella
	long   m_PolymerisedType;			// Type of polymerised polymer
	double m_BilayerArea;				// Area per monolayer
	double m_Bilayer1Length;			// Inplane size of bilayer: NOT necessarily x,y coordinates
	double m_Bilayer2Length;
	double m_BilayerCentre;				// Coordinate of bilayer midplane (depends on orientation)
	double m_UpperHead;					// Coordinate of upper monolayer head beads
	double m_LowerHead;					// Coordinate of lower monolayer head beads


};

#endif // !defined(AFX_INITIALSTATELAMELLA_H__6CE669F4_04B5_11D4_BF30_004095086186__INCLUDED_)

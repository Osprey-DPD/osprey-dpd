// InitialStateCompositeLamella.h: interface for the CInitialStateCompositeLamella class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INITIALSTATECOMPOSITELAMELLA_H__2E32FD83_2D5A_11D4_BF36_004095086186__INCLUDED_)
#define AFX_INITIALSTATECOMPOSITELAMELLA_H__2E32FD83_2D5A_11D4_BF36_004095086186__INCLUDED_



#include "InitialStateData.h"

class CInitialStateCompositeLamella : public CInitialStateData  
{
public:
	CInitialStateCompositeLamella();
	virtual ~CInitialStateCompositeLamella();

	CInitialStateCompositeLamella& operator=(const CInitialStateCompositeLamella& oldISL);

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

    inline  StringSequence GetPolymers()           const {return m_Polymers;}
    inline zDoubleVector   GetUpperFractions()     const {return m_UpperFractions;}
    inline long            GetXNormal()            const {return m_X;}
    inline long            GetYNormal()            const {return m_Y;}
    inline long            GetZNormal()            const {return m_Z;}
    inline double          GetCentre()             const {return m_Centre;}
    inline double          GetThickness()          const {return m_Thickness;}
    inline bool            GetLinearise()          const {return m_bLinearise;}
    inline bool            GetPolymerise()         const {return m_bPolymerise;}
	inline  bool           GetUpperPatchesFlag()   const {return m_bPatches[0];}
	inline  bool           GetLowerPatchesFlag()   const {return m_bPatches[1];}


protected:
	virtual bool ValidateData(const CInputData& riData);
	virtual bool ValidateDataP(const CInputData& riData);

private:
	static const zString m_Type;			// Keyword used to identify initial state type

	StringSequence m_Polymers;			// Polymers composing bilayer
	zDoubleVector  m_UpperFractions;		// Fractions of polymers in upper monolayer
	long	       m_X;						
	long	       m_Y;				// Bilayer normal: only x, y or z allowed
	long	       m_Z;
	double	       m_Centre;			// Bilayer centre as fraction of SimBox thickness
	double	       m_Thickness;			// In units of bead diameters
	bool	       m_bLinearise;			// Flag showing whether polymer is initially straightened
	bool	       m_bPatches[2];			// Arrange polymers in patches: 0/1

	// Data relating to a polymerised initial state
	bool		m_bPolymerise;	               // Flag showing if initial state is polymerised
	PolymerCrossLinkVector	m_CrossLinks;	       // Vector of cross-linker pointers holding data

	// Local data

	zLongVector     m_PolymerTypes;			// Types of polymer composing lamella
	double		m_BilayerArea;			// Area per monolayer
	double		m_Bilayer1Length;		// Inplane size of bilayer: NOT necessarily x,y coordinates
	double		m_Bilayer2Length;
	double		m_BilayerCentre;		// Coordinate of bilayer midplane (depends on orientation)
	double		m_UpperHead;			// Coordinate of upper monolayer head beads
	double		m_LowerHead;			// Coordinate of lower monolayer head beads

};

#endif // !defined(AFX_INITIALSTATECOMPOSITELAMELLA_H__2E32FD83_2D5A_11D4_BF36_004095086186__INCLUDED_)

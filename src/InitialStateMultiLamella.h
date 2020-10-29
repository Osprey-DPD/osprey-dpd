// InitialStateMultiLamella.h: interface for the CInitialStateMultiLamella class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INITIALSTATEMULTILAMELLA_H__2E32FD85_2D5A_11D4_BF36_004095086186__INCLUDED_)
#define AFX_INITIALSTATEMULTILAMELLA_H__2E32FD85_2D5A_11D4_BF36_004095086186__INCLUDED_


#include "InitialStateData.h"

class CInitialStateMultiLamella : public CInitialStateData  
{
	// ****************************************
	// Construction/Destruction
public:

	CInitialStateMultiLamella();
	virtual ~CInitialStateMultiLamella();

	CInitialStateMultiLamella& operator=(const CInitialStateMultiLamella& oldISL);

	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();

private:

	static const zString m_Type;		// Keyword used to identify initial state type

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	zOutStream& put(zOutStream& os) const;		// overrides the virtual base function
	zInStream&  get(zInStream&  is);			// overrides the virtual base function

	virtual zString GetInitialStateType() const;

	virtual bool	Assemble(CInitialState& riState) const;

	virtual CInitialStateData* Copy() const;


	// ****************************************
	// Public access functions
public:


	// ****************************************
	// Protected local functions
protected:

	virtual bool ValidateData(const CInputData& riData);

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:

	// ****************************************
	// Data members

	StringSequence m_Polymers;			// Polymers composing lamellae
	long	m_X;						
	long	m_Y;						// Lamellae normal: only x, y or z allowed
	long	m_Z;
	double	m_Centre;					// First lamella centre as fraction of SimBox thickness
	double	m_Thickness;				// Lamellae thickness in units of bead diameters
	double	m_SolventGap;				// Solvent thickness in units of bead diameters
	bool	m_bLinearise;				// Flag showing whether polymer is initially straightened
	zDoubleVector	m_UpperFractions;	// Fractions of polymers in upper monolayer

	// Local data

	zLongVector		m_PolymerTypes;			// Types of polymer composing lamellae
	long			m_LamellaTotal;			// Number of lamellae
	double			m_BilayerArea;			// Area per monolayer
	double			m_Bilayer1Length;		// Inplane size of lamellae: NOT necessarily x,y coordinates
	double			m_Bilayer2Length;
	double			m_DeltaCentre;			// Distance between lamellae centre coordinates
	zDoubleVector	m_BilayerCentres;		// Coordinates of lamellae midplanes (depends on orientation)

};

#endif // !defined(AFX_INITIALSTATEMULTILAMELLA_H__2E32FD85_2D5A_11D4_BF36_004095086186__INCLUDED_)

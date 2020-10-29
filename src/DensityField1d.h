// DensityField1d.h: interface for the CDensityField1d class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DENSITYFIELD1D_H__2210C48C_5B27_499D_941F_5107B764BB52__INCLUDED_)
#define AFX_DENSITYFIELD1D_H__2210C48C_5B27_499D_941F_5107B764BB52__INCLUDED_


#include "DensityFieldState.h"

class CDensityField1d : public CDensityFieldState
{
	// ****************************************
	// Construction/Destruction
public:

	CDensityField1d(const zString runId, long currentTime, const zString acnType,
					long start, long end,long sample,
					long nx, long ny, long nz, long sliceTotal, double lx, double ly, double lz, 
					double lowerCoord, double upperCoord);

	virtual ~CDensityField1d();

	// ****************************************
	// Global functions, static member functions and variables
public:

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	virtual void Sample()	 = 0;
	virtual void Normalize() = 0;
	virtual bool Serialize();		// Generic for all derived classes

	// ****************************************
	// Public access functions
public:

	inline long   GetXNormal()		const {return m_NX;}
	inline long   GetYNormal()		const {return m_NY;}
	inline long   GetZNormal()		const {return m_NZ;}
	inline long	  GetSliceTotal()	const {return m_SliceTotal;}
	inline double GetSliceWidth()	const {return m_SliceWidth;}
	inline double GetSliceVolume()	const {return m_SliceVolume;}
	inline double GetLowerCoord()	const {return m_LowerCoord;}
	inline double GetUpperCoord()	const {return m_UpperCoord;}


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

	// Data specific to 1d density profiles

	long	m_NX;
	long	m_NY;				// Direction of density profile
	long	m_NZ;
	long	m_SliceTotal;		// Number of slices in the density profile
	double	m_LX;				// SimBox X length
	double	m_LY;				// SimBox Y length
	double	m_LZ;				// SimBox Z length
	double	m_LowerCoord;		// Minimum value of coordinates in profile direction
	double  m_UpperCoord;		// Maximum    "

	// Local data

	double m_SliceWidth;
	double m_SliceVolume;

};

#endif // !defined(AFX_DENSITYFIELD1D_H__2210C48C_5B27_499D_941F_5107B764BB52__INCLUDED_)

// DensityState.h: interface for the CDensityState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DENSITYSTATE_H__E03CC0A0_607E_11D3_820E_0060088AD300__INCLUDED_)
#define AFX_DENSITYSTATE_H__E03CC0A0_607E_11D3_820E_0060088AD300__INCLUDED_


// Forward declarations

class CGridObservable;


#include "xxState.h"

class CDensityState : public xxState  
{
	// ****************************************
	// Construction/Destruction
public:

	CDensityState(long currentTime, const zString runId, const CGridObservable* pOb);

	virtual ~CDensityState();

	// ****************************************
	// Global functions, static member functions and variables
public:

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	bool Serialize();

	// ****************************************
	// Public access functions
public:

	const zString GetFieldName() const;
	long GetFieldId() const;

	double GetDensity(long ix, long iy, long iz) const;
	double GetSum(long ix, long iy, long iz) const;


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

    const zString m_FieldName;
    long          m_FieldId;

	long m_GridXCellNo;
	long m_GridYCellNo;
	long m_GridZCellNo;

	double m_GridXCellWidth;
	double m_GridYCellWidth;
	double m_GridZCellWidth;

	const zDoubleVector* m_pDensity;
	const zDoubleVector* m_pSum;

};

#endif // !defined(AFX_DENSITYSTATE_H__E03CC0A0_607E_11D3_820E_0060088AD300__INCLUDED_)

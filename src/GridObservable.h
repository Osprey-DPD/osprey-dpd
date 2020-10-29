// GridObservable.h: interface for the CGridObservable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRIDOBSERVABLE_H__5652BEC5_5495_11D3_820E_0060088AD300__INCLUDED_)
#define AFX_GRIDOBSERVABLE_H__5652BEC5_5495_11D3_820E_0060088AD300__INCLUDED_


#include "Observable.h"


class CGridObservable : public CObservable  
{
	friend class CMonitor;	    // allow CMonitor to add data to the observable
	friend class CDensityState;	// allow data to be written to file

	// ****************************************
	// Construction/Destruction:
public:

	CGridObservable(const zString name, long analysisPeriod, long samplePeriod,
					long GridXCellNo, long GridYCellNo, long GridZCellNo,
					double GridXCellWidth, double GridYCellWidth, double GridZCellWidth,
					long ObjectTotal, const zString fieldName, long fieldId);

	virtual ~CGridObservable();

	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
protected:

	void Normalize(long currentTime);
	void Serialize(zOutStream& os) const;
	void DisplayDebug();

	// ****************************************
	// Public access functions
public:

    inline const zString GetFieldName()      const {return m_FieldName;}
    inline long          GetFieldId()        const {return m_FieldId;}
    inline long          GetGridXCellNo()    const {return m_GridXCellNo;}
    inline long          GetGridYCellNo()    const {return m_GridYCellNo;}
    inline long          GetGridZCellNo()    const {return m_GridZCellNo;}
    inline double        GetGridXCellWidth() const {return m_GridXCellWidth;}
    inline double        GetGridYCellWidth() const {return m_GridYCellWidth;}
    inline double        GetGridZCellWidth() const {return m_GridZCellWidth;}

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

	long m_GridXCellNo;
	long m_GridYCellNo;
	long m_GridZCellNo;
	double m_GridXCellWidth;
	double m_GridYCellWidth;
	double m_GridZCellWidth;

	long m_ObjectTotal;

	const zString m_FieldName;  // String identifier for the field
    long          m_FieldId;    // Numeric identifier for the field

	double m_DensitySampleSize;

	zDoubleVector m_vField;
	zDoubleVector m_vSum;
	zDoubleVector m_vDensity;


};

#endif // !defined(AFX_GRIDOBSERVABLE_H__5652BEC5_5495_11D3_820E_0060088AD300__INCLUDED_)

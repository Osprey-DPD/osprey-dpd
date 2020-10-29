// ISetExternalFieldDataImpl.h: interface for the ISetExternalFieldDataImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISETEXTERNALFIELDDATAIMPL_H__32DBF389_5A50_456A_98AB_70BDB431A0BC__INCLUDED_)
#define AFX_ISETEXTERNALFIELDDATAIMPL_H__32DBF389_5A50_456A_98AB_70BDB431A0BC__INCLUDED_


// Forward declarations

class CSimulation;


#include "ISetExternalFieldData.h"

class ISetExternalFieldDataImpl : public ISetExternalFieldData
{
	// ****************************************
	// Construction/Destruction
public:

	ISetExternalFieldDataImpl(CSimulation& rSim);

	virtual ~ISetExternalFieldDataImpl();

	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs inherited from base interface class
public:

	virtual ISimulation* IISimulation() const;


	// ****************************************
	// Public access functions
public:



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

	CSimulation& m_rSimulation;		// Reference to the containing class

};

#endif // !defined(AFX_ISETEXTERNALFIELDDATAIMPL_H__32DBF389_5A50_456A_98AB_70BDB431A0BC__INCLUDED_)

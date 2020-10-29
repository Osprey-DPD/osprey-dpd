// ISetSubstrateDataImpl.h: interface for the ISetSubstrateDataImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISETSUBSTRATEDATAIMPL_H__4054B2F6_014C_45D7_98F9_C01DB3829C96__INCLUDED_)
#define AFX_ISETSUBSTRATEDATAIMPL_H__4054B2F6_014C_45D7_98F9_C01DB3829C96__INCLUDED_


// Forward declarations

class CSimulation;


#include "ISetSubstrateData.h"

class ISetSubstrateDataImpl : public ISetSubstrateData
{
	// ****************************************
	// Construction/Destruction
public:

	ISetSubstrateDataImpl(CSimulation& rSim);

	virtual ~ISetSubstrateDataImpl();

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

#endif // !defined(AFX_ISETSUBSTRATEDATAIMPL_H__4054B2F6_014C_45D7_98F9_C01DB3829C96__INCLUDED_)

// ISetProbeFieldDataImpl.h: interface for the ISetProbeFieldDataImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISETPROBEFIELDDATAIMPL_H__240B78D2_67AB_47A1_BB67_E16CF7C246D7__INCLUDED_)
#define AFX_ISETPROBEFIELDDATAIMPL_H__240B78D2_67AB_47A1_BB67_E16CF7C246D7__INCLUDED_


// Forward declarations

class CSimulation;


#include "ISetProbeFieldData.h"

class ISetProbeFieldDataImpl : public ISetProbeFieldData
{
	// ****************************************
	// Construction/Destruction
public:

	ISetProbeFieldDataImpl(CSimulation& rSim);

	virtual ~ISetProbeFieldDataImpl();

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

#endif // !defined(AFX_ISETPROBEFIELDDATAIMPL_H__240B78D2_67AB_47A1_BB67_E16CF7C246D7__INCLUDED_)

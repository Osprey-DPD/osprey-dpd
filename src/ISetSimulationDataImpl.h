// ISetSimulationDataImpl.h: interface for the ISetSimulationDataImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISETSIMULATIONDATAIMPL_H__FE8422D4_CD61_4A57_8138_2A0CFF8B2C2F__INCLUDED_)
#define AFX_ISETSIMULATIONDATAIMPL_H__FE8422D4_CD61_4A57_8138_2A0CFF8B2C2F__INCLUDED_


// Forward declarations

class CSimulation;


#include "ISetSimulationData.h"

class ISetSimulationDataImpl : public ISetSimulationData
{
	// ****************************************
	// Construction/Destruction
public:

	ISetSimulationDataImpl(CSimulation& rSim);

	virtual ~ISetSimulationDataImpl();

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

#endif // !defined(AFX_ISETSIMULATIONDATAIMPL_H__FE8422D4_CD61_4A57_8138_2A0CFF8B2C2F__INCLUDED_)

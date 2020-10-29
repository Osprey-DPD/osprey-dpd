// ISetSculptureDataImpl.h: interface for the ISetSculptureDataImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISETSCULPTUREDATAIMPL_H__DC08BE4E_0853_4C94_90C7_BB44895B79D1__INCLUDED_)
#define AFX_ISETSCULPTUREDATAIMPL_H__DC08BE4E_0853_4C94_90C7_BB44895B79D1__INCLUDED_


// Forward declarations

class CSimulation;


#include "ISetSculptureData.h"

class ISetSculptureDataImpl : public ISetSculptureData
{
	// ****************************************
	// Construction/Destruction
public:

	ISetSculptureDataImpl(CSimulation& rSim);

	virtual ~ISetSculptureDataImpl();

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

#endif // !defined(AFX_ISETSCULPTUREDATAIMPL_H__DC08BE4E_0853_4C94_90C7_BB44895B79D1__INCLUDED_)

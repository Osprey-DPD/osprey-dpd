// ISetProcessDataImpl.h: interface for the ISetProcessDataImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISETPROCESSDATAIMPL_H__4D2758E0_9148_4909_A6E4_3A3B71389A2E__INCLUDED_)
#define AFX_ISETPROCESSDATAIMPL_H__4D2758E0_9148_4909_A6E4_3A3B71389A2E__INCLUDED_


// Forward declarations

class CSimulation;


#include "ISetProcessData.h"

class ISetProcessDataImpl : public ISetProcessData
{
	// ****************************************
	// Construction/Destruction
public:

	ISetProcessDataImpl(CSimulation& rSim);

	virtual ~ISetProcessDataImpl();

	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs inherited from base interface class
public:

	virtual ISimulation* IISimulation() const;

	virtual bool AddProcess(xxProcess* const pProcess);

	// ****************************************
	// Public access functions
public:
	long GetProcessTotal();



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

#endif // !defined(AFX_ISETPROCESSDATAIMPL_H__4D2758E0_9148_4909_A6E4_3A3B71389A2E__INCLUDED_)



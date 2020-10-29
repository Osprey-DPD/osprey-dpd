// IConnectSimulationImpl.h: interface for the IConnectSimulationImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICONNECTSIMULATIONIMPL_H__A31EE621_05B7_4C0E_9D80_4FBC37AB7440__INCLUDED_)
#define AFX_ICONNECTSIMULATIONIMPL_H__A31EE621_05B7_4C0E_9D80_4FBC37AB7440__INCLUDED_


// Forward declarations

class CExperiment;


#include "IConnectSimulation.h"

class IConnectSimulationImpl : public IConnectSimulation
{
	// ****************************************
	// Construction/Destruction
public:

	IConnectSimulationImpl(CExperiment& rExpt);

	virtual ~IConnectSimulationImpl();

	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:


	// ****************************************
	// Public access functions
public:

	IExperiment* IIExperiment() const;

	IDocumentHandler* GetIDocumentHandler();

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

	CExperiment& m_rExperiment;		// Reference to the containing class

};


#endif // !defined(AFX_ICONNECTSIMULATIONIMPL_H__A31EE621_05B7_4C0E_9D80_4FBC37AB7440__INCLUDED_)

// IExperimentAccessControlImpl.h: interface for the IExperimentAccessControlImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IEXPERIMENTACCESSCONTROLIMPL_H__339CABD0_E109_4684_87C9_EAC888EFBCC8__INCLUDED_)
#define AFX_IEXPERIMENTACCESSCONTROLIMPL_H__339CABD0_E109_4684_87C9_EAC888EFBCC8__INCLUDED_


// Forward declarations

class CSimulation;


#include "IExperimentAccessControl.h"

class IExperimentAccessControlImpl : public IExperimentAccessControl
{
	// ****************************************
	// Construction/Destruction
public:

	IExperimentAccessControlImpl(CSimulation& rSim);

	virtual ~IExperimentAccessControlImpl();

	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs inherited from base interface class
public:

	virtual ISimulation* IISimulation() const;

	virtual bool IsExperimentAccessible() const;

	virtual IDocumentHandler* GetIDocumentHandler();

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

#endif // !defined(AFX_IEXPERIMENTACCESSCONTROLIMPL_H__339CABD0_E109_4684_87C9_EAC888EFBCC8__INCLUDED_)

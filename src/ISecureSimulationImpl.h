// ISecureSimulationImpl.h: interface for the ISecureSimulationImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISECURESIMULATIONIMPL_H__E5F6D8BE_7180_4F07_B87D_5DC70D1C3D34__INCLUDED_)
#define AFX_ISECURESIMULATIONIMPL_H__E5F6D8BE_7180_4F07_B87D_5DC70D1C3D34__INCLUDED_


// Forward declarations

class CSimulation;


#include "ISecureSimulation.h"

class ISecureSimulationImpl : public ISecureSimulation
{
	// ****************************************
	// Construction/Destruction
public:

	ISecureSimulationImpl(CSimulation& rSim);

	virtual ~ISecureSimulationImpl();

	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:


	// ****************************************
	// Public access functions
public:

	ISimulation* IISimulation() const;


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

#endif // !defined(AFX_ISECURESIMULATIONIMPL_H__E5F6D8BE_7180_4F07_B87D_5DC70D1C3D34__INCLUDED_)

// ISetSimulationData.h: interface for the ISetSimulationData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISETSIMULATIONDATA_H__FD8FEE8E_8933_4E8A_BBB8_E7C789AC7870__INCLUDED_)
#define AFX_ISETSIMULATIONDATA_H__FD8FEE8E_8933_4E8A_BBB8_E7C789AC7870__INCLUDED_


// Forward declarations

class ISimulation;


class ISetSimulationData  
{
	// ****************************************
	// Construction/Destruction: abstract base class has no ctor or dtor
public:


	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	virtual ISimulation* IISimulation() const = 0;


	// ****************************************
	// Public access functions
public:


	// ****************************************
	// Protected local functions
protected:

	virtual ~ISetSimulationData();

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

};

#endif // !defined(AFX_ISETSIMULATIONDATA_H__FD8FEE8E_8933_4E8A_BBB8_E7C789AC7870__INCLUDED_)

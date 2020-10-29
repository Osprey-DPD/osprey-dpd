// ISecureSimulation.h: interface for the ISecureSimulation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISECURESIMULATION_H__DAE42E45_4EE2_4FBD_89AF_2F25391EA0BB__INCLUDED_)
#define AFX_ISECURESIMULATION_H__DAE42E45_4EE2_4FBD_89AF_2F25391EA0BB__INCLUDED_


// Forward declarations

class ISimulation;


class ISecureSimulation
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

	virtual ~ISecureSimulation();

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:


};

#endif // !defined(AFX_ISECURESIMULATION_H__DAE42E45_4EE2_4FBD_89AF_2F25391EA0BB__INCLUDED_)

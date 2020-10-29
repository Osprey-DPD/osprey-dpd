// ISetProcessData.h: interface for the ISetProcessData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISETPROCESSDATA_H__CD82CE4F_F22E_4CD7_9ADE_245B96FBD8A2__INCLUDED_)
#define AFX_ISETPROCESSDATA_H__CD82CE4F_F22E_4CD7_9ADE_245B96FBD8A2__INCLUDED_


// Forward declarations

class ISimulation;
class xxProcess;


class ISetProcessData  
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


	virtual bool AddProcess(xxProcess* const pProcess) = 0;


	// ****************************************
	// Public access functions
public:


	// ****************************************
	// Protected local functions
protected:

	virtual ~ISetProcessData();

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

};

#endif // !defined(AFX_ISETPROCESSDATA_H__CD82CE4F_F22E_4CD7_9ADE_245B96FBD8A2__INCLUDED_)

// ISecureExperiment.h: interface for the ISecureExperiment class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISECUREEXPERIMENT_H__2D3B7F6D_8CBF_4DA5_B385_587C13A0192D__INCLUDED_)
#define AFX_ISECUREEXPERIMENT_H__2D3B7F6D_8CBF_4DA5_B385_587C13A0192D__INCLUDED_


// Forward declarations

class IExperiment;


class ISecureExperiment
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

	virtual IExperiment* IIExperiment() const = 0;



	// ****************************************
	// Public access functions
public:


	// ****************************************
	// Protected local functions
protected:

	virtual ~ISecureExperiment();

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:

	// ****************************************
	// Data members
private:

};

#endif // !defined(AFX_ISECUREEXPERIMENT_H__2D3B7F6D_8CBF_4DA5_B385_587C13A0192D__INCLUDED_)

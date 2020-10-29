// IExperimentAccessControl.h: interface for the IExperimentAccessControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IEXPERIMENTACCESSCONTROL_H__6A332876_1D88_4C6F_8B40_5BCF3B945C9B__INCLUDED_)
#define AFX_IEXPERIMENTACCESSCONTROL_H__6A332876_1D88_4C6F_8B40_5BCF3B945C9B__INCLUDED_


// Forward declarations

class ISimulation;
class IDocumentHandler;


class IExperimentAccessControl
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

	virtual bool IsExperimentAccessible() const = 0;

	virtual IDocumentHandler* GetIDocumentHandler() = 0;


	// ****************************************
	// Public access functions
public:


	// ****************************************
	// Protected local functions
protected:

	virtual ~IExperimentAccessControl();

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:


};

#endif // !defined(AFX_IEXPERIMENTACCESSCONTROL_H__6A332876_1D88_4C6F_8B40_5BCF3B945C9B__INCLUDED_)

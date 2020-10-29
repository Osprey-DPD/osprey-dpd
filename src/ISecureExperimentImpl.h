// ISecureExperimentImpl.h: interface for the ISecureExperimentImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISECUREEXPERIMENTIMPL_H__0EFA06BA_6AFB_4618_92C4_B737680D56B4__INCLUDED_)
#define AFX_ISECUREEXPERIMENTIMPL_H__0EFA06BA_6AFB_4618_92C4_B737680D56B4__INCLUDED_


// Forward declarations

class CExperiment;


#include "ISecureExperiment.h"

class ISecureExperimentImpl : public ISecureExperiment
{
	// ****************************************
	// Construction/Destruction
public:

	ISecureExperimentImpl(CExperiment& rExpt);

	virtual ~ISecureExperimentImpl();

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

#endif // !defined(AFX_ISECUREEXPERIMENTIMPL_H__0EFA06BA_6AFB_4618_92C4_B737680D56B4__INCLUDED_)

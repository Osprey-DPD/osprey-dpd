// ccCreateNanoparticleSphereImpl.h: interface for the ccCreateNanoparticleSphereImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCCREATENANOPARTICLESPHEREIMPL_H__9fe807fd_1114_4bec_8459_786a874310e2__INCLUDED_)
#define AFX_CCCREATENANOPARTICLESPHEREIMPL_H__9fe807fd_1114_4bec_8459_786a874310e2__INCLUDED_


#include "ISimBoxCmd.h"

class ccCreateNanoparticleSphereImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ccCreateNanoparticleSphereImpl();

	virtual ~ccCreateNanoparticleSphereImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void CreateNanoparticleSphere(const xxCommand* const pCommand);


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

};

#endif // !defined(AFX_CCCREATENANOPARTICLESPHEREIMPL_H__9fe807fd_1114_4bec_8459_786a874310e2__INCLUDED_)

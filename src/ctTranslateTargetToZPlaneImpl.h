// ctTranslateTargetToZPlaneImpl.h: interface for the ctTranslateTargetToZPlaneImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTTRANSLATETARGETTOZPLANEIMPL_H__52ddce20_16ec_43ea_8622_9e14b1d7a03c__INCLUDED_)
#define AFX_CTTRANSLATETARGETTOZPLANEIMPL_H__52ddce20_16ec_43ea_8622_9e14b1d7a03c__INCLUDED_


#include "ISimBoxCmd.h"

class ctTranslateTargetToZPlaneImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ctTranslateTargetToZPlaneImpl();

	virtual ~ctTranslateTargetToZPlaneImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void TranslateTargetToZPlane(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CTTRANSLATETARGETTOZPLANEIMPL_H__52ddce20_16ec_43ea_8622_9e14b1d7a03c__INCLUDED_)

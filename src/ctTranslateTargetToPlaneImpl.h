// ctTranslateTargetToPlaneImpl.h: interface for the ctTranslateTargetToPlaneImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTTRANSLATETARGETTOPLANEIMPL_H__de8f9ab8_be3a_4f30_a822_2435a5387515__INCLUDED_)
#define AFX_CTTRANSLATETARGETTOPLANEIMPL_H__de8f9ab8_be3a_4f30_a822_2435a5387515__INCLUDED_


#include "ISimBoxCmd.h"

class ctTranslateTargetToPlaneImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ctTranslateTargetToPlaneImpl();

	virtual ~ctTranslateTargetToPlaneImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void TranslateTargetToPlane(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CTTRANSLATETARGETTOPLANEIMPL_H__de8f9ab8_be3a_4f30_a822_2435a5387515__INCLUDED_)

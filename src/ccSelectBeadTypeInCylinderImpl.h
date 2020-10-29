// ccSelectBeadTypeInCylinderImpl.h: interface for the ccSelectBeadTypeInCylinderImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSELECTBEADTYPEINCYLINDERIMPL_H__377446dd_cded_425e_b391_30ee2d3f5b65__INCLUDED_)
#define AFX_CCSELECTBEADTYPEINCYLINDERIMPL_H__377446dd_cded_425e_b391_30ee2d3f5b65__INCLUDED_


#include "ISimBoxCmd.h"

class ccSelectBeadTypeInCylinderImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ccSelectBeadTypeInCylinderImpl();

	virtual ~ccSelectBeadTypeInCylinderImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SelectBeadTypeInCylinder(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CCSELECTBEADTYPEINCYLINDERIMPL_H__377446dd_cded_425e_b391_30ee2d3f5b65__INCLUDED_)

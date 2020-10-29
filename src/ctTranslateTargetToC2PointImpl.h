// ctTranslateTargetToC2PointImpl.h: interface for the ctTranslateTargetToC2PointImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTTRANSLATETARGETTOC2POINTIMPL_H__5ef88a9f_4fe8_4f18_9654_b0b38ad9959d__INCLUDED_)
#define AFX_CTTRANSLATETARGETTOC2POINTIMPL_H__5ef88a9f_4fe8_4f18_9654_b0b38ad9959d__INCLUDED_


#include "ISimBoxCmd.h"

class ctTranslateTargetToC2PointImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ctTranslateTargetToC2PointImpl();

	virtual ~ctTranslateTargetToC2PointImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void TranslateTargetToC2Point(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CTTRANSLATETARGETTOC2POINTIMPL_H__5ef88a9f_4fe8_4f18_9654_b0b38ad9959d__INCLUDED_)

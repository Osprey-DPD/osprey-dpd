// cgcSetArgumentsTo3dRectangularLatticeVectorImpl.h: interface for the cgcSetArgumentsTo3dRectangularLatticeVectorImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CGCSETARGUMENTSTO3DRECTANGULARLATTICEVECTORIMPL_H__DB4B6027_BFEC_4C48_BE1F_3CE29DB42537__INCLUDED_)
#define AFX_CGCSETARGUMENTSTO3DRECTANGULARLATTICEVECTORIMPL_H__DB4B6027_BFEC_4C48_BE1F_3CE29DB42537__INCLUDED_



#include "ISimBoxCmd.h"

class cgcSetArgumentsTo3dRectangularLatticeVectorImpl : public virtual ISimBoxCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	cgcSetArgumentsTo3dRectangularLatticeVectorImpl();

	virtual ~cgcSetArgumentsTo3dRectangularLatticeVectorImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SetArgumentsTo3dRectangularLatticeVector(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CGCSETARGUMENTSTO3DRECTANGULARLATTICEVECTORIMPL_H__DB4B6027_BFEC_4C48_BE1F_3CE29DB42537__INCLUDED_)

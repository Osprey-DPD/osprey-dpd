// cgcSetArgumentsTo2dTriangularLatticeVectorImpl.h: interface for the cgcSetArgumentsTo2dTriangularLatticeVectorImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CGCSETARGUMENTSTO2DTRIANGULARLATTICEVECTORIMPL_H__ea7e58da_0b98_409c_8271_983619d50521__INCLUDED_)
#define AFX_CGCSETARGUMENTSTO2DTRIANGULARLATTICEVECTORIMPL_H__ea7e58da_0b98_409c_8271_983619d50521__INCLUDED_



#include "ISimBoxCmd.h"

class cgcSetArgumentsTo2dTriangularLatticeVectorImpl : public virtual ISimBoxCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	cgcSetArgumentsTo2dTriangularLatticeVectorImpl();

	virtual ~cgcSetArgumentsTo2dTriangularLatticeVectorImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SetArgumentsTo2dTriangularLatticeVector(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CGCSETARGUMENTSTO2DTRIANGULARLATTICEVECTORIMPL_H__ea7e58da_0b98_409c_8271_983619d50521__INCLUDED_)

// cgcSetArgumentsTo2dRectangularLatticeVectorImpl.h: interface for the cgcSetArgumentsTo2dRectangularLatticeVectorImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CGCSETARGUMENTSTO2DRECTANGULARLATTICEVECTORIMPL_H__05ABFD2B_CE43_49D7_950C_E052D7C8D7DD__INCLUDED_)
#define AFX_CGCSETARGUMENTSTO2DRECTANGULARLATTICEVECTORIMPL_H__05ABFD2B_CE43_49D7_950C_E052D7C8D7DD__INCLUDED_



#include "ISimBoxCmd.h"

class cgcSetArgumentsTo2dRectangularLatticeVectorImpl : public virtual ISimBoxCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	cgcSetArgumentsTo2dRectangularLatticeVectorImpl();

	virtual ~cgcSetArgumentsTo2dRectangularLatticeVectorImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SetArgumentsTo2dRectangularLatticeVector(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CGCSETARGUMENTSTO2DRECTANGULARLATTICEVECTORIMPL_H__05ABFD2B_CE43_49D7_950C_E052D7C8D7DD__INCLUDED_)

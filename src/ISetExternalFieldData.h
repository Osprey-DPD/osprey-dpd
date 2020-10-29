// ISetExternalFieldData.h: interface for the ISetExternalFieldData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISETEXTERNALFIELDDATA_H__5C18BCAE_9EC9_443D_9AA6_B3FF1D58285C__INCLUDED_)
#define AFX_ISETEXTERNALFIELDDATA_H__5C18BCAE_9EC9_443D_9AA6_B3FF1D58285C__INCLUDED_


// Forward declarations

class ISimulation;


class ISetExternalFieldData  
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


	// ****************************************
	// Public access functions
public:


	// ****************************************
	// Protected local functions
protected:

	virtual ~ISetExternalFieldData();

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

};

#endif // !defined(AFX_ISETEXTERNALFIELDDATA_H__5C18BCAE_9EC9_443D_9AA6_B3FF1D58285C__INCLUDED_)

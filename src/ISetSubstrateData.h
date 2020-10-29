// ISetSubstrateData.h: interface for the ISetSubstrateData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISETSUBSTRATEDATA_H__627CBCEF_791E_423D_AB0B_46C6B6AA9248__INCLUDED_)
#define AFX_ISETSUBSTRATEDATA_H__627CBCEF_791E_423D_AB0B_46C6B6AA9248__INCLUDED_


// Forward declarations

class ISimulation;


class ISetSubstrateData  
{
	// ****************************************
	// Construction/Destruction: no ctor or dtor as this is an interface class
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

	virtual ~ISetSubstrateData();

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

};

#endif // !defined(AFX_ISETSUBSTRATEDATA_H__627CBCEF_791E_423D_AB0B_46C6B6AA9248__INCLUDED_)

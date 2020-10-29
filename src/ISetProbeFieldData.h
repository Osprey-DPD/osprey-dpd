// ISetProbeFieldData.h: interface for the ISetProbeFieldData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISETPROBEFIELDDATA_H__A66780C8_41D3_428A_8190_B9BD4B61D3F6__INCLUDED_)
#define AFX_ISETPROBEFIELDDATA_H__A66780C8_41D3_428A_8190_B9BD4B61D3F6__INCLUDED_


// Forward declarations

class ISimulation;


class ISetProbeFieldData  
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

	virtual ~ISetProbeFieldData();

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

};

#endif // !defined(AFX_ISETPROBEFIELDDATA_H__A66780C8_41D3_428A_8190_B9BD4B61D3F6__INCLUDED_)

// ISetSculptureData.h: interface for the ISetSculptureData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISETSCULPTUREDATA_H__A1CB8AEC_E807_4973_B79D_DBE76713E4C1__INCLUDED_)
#define AFX_ISETSCULPTUREDATA_H__A1CB8AEC_E807_4973_B79D_DBE76713E4C1__INCLUDED_


// Forward declarations

class ISimulation;


class ISetSculptureData  
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

	virtual ~ISetSculptureData();

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

};

#endif // !defined(AFX_ISETSCULPTUREDATA_H__A1CB8AEC_E807_4973_B79D_DBE76713E4C1__INCLUDED_)

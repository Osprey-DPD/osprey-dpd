// DensityField2d.h: interface for the CDensityField2d class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DENSITYFIELD2D_H__0281E09F_8193_4D9A_9ED6_36D9826A8F33__INCLUDED_)
#define AFX_DENSITYFIELD2D_H__0281E09F_8193_4D9A_9ED6_36D9826A8F33__INCLUDED_


class CDensityField2d  
{
	// ****************************************
	// Construction/Destruction
public:

	CDensityField2d();

	virtual ~CDensityField2d();

	// ****************************************
	// Global functions, static member functions and variables
public:

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	void Sample();
	void Normalize();
	bool Serialize();

	// ****************************************
	// Public access functions
public:


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

#endif // !defined(AFX_DENSITYFIELD2D_H__0281E09F_8193_4D9A_9ED6_36D9826A8F33__INCLUDED_)

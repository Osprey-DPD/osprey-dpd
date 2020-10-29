// DensityField3d.h: interface for the CDensityField3d class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DENSITYFIELD3D_H__00C2521F_8257_4C98_9AFA_AEF6B9E08FD3__INCLUDED_)
#define AFX_DENSITYFIELD3D_H__00C2521F_8257_4C98_9AFA_AEF6B9E08FD3__INCLUDED_


class CDensityField3d  
{
	// ****************************************
	// Construction/Destruction
public:

	CDensityField3d();

	virtual ~CDensityField3d();

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

#endif // !defined(AFX_DENSITYFIELD3D_H__00C2521F_8257_4C98_9AFA_AEF6B9E08FD3__INCLUDED_)

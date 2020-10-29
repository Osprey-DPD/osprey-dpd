// IACNAccessControl.h: interface for the IACNAccessControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IACNACCESSCONTROL_H__04330D17_A2F3_4051_AFAD_49045C687519__INCLUDED_)
#define AFX_IACNACCESSCONTROL_H__04330D17_A2F3_4051_AFAD_49045C687519__INCLUDED_


class IACNAccessControl
{
	// ****************************************
	// Construction/Destruction: no ctor as this is an interface class
public:


	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	virtual bool IsAccessControlEnabled() const = 0;



	// ****************************************
	// Public access functions
public:


	// ****************************************
	// Protected local functions
protected:

    	virtual ~IACNAccessControl();

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members: No member variables as this is an interface class
private:


};

#endif // !defined(AFX_IACNACCESSCONTROL_H__04330D17_A2F3_4051_AFAD_49045C687519__INCLUDED_)

// IfActinAccessControl.h: interface for the IfActinAccessControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IFACTINACCESSCONTROL_H__06C6E33B_A2F6_49DF_ADC8_430130B40960__INCLUDED_)
#define AFX_IFACTINACCESSCONTROL_H__06C6E33B_A2F6_49DF_ADC8_430130B40960__INCLUDED_


// Forward declarations

class aeArp23Network;
class aefActinNetwork;
class aeForminNetwork;
class aeReceptorNetwork;

// Include file for the IfActinAccess interface class that is cast to one
// of its base classes for use by the calling ACN

#include "IfActinAccess.h"


#include "IACNAccessControl.h"

class IfActinAccessControl : public IACNAccessControl
{
	// ****************************************
	// Construction/Destruction: 
public:

	IfActinAccessControl(aefActinNetwork* const pNetwork);

	virtual ~IfActinAccessControl();


	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs
public:

	virtual bool IsAccessControlEnabled() const;


	// ****************************************
	// Public access functions
public:

	// The following functions return pointers to the interface classes 
	// that allow the calling networks to modify this network

	IfActinAccessFromArp23*    IsAccessAllowed(const aeArp23Network* const pACN);
	IfActinAccessFromFormin*   IsAccessAllowed(const aeForminNetwork* const pACN);
	IfActinAccessFromReceptor* IsAccessAllowed(const aeReceptorNetwork* const pACN);



	// ****************************************
	// Protected local functions
protected:

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members:
private:

	IfActinAccess* const m_pIAccess;	// Interface object controlling access to fActin network

};

#endif // !defined(AFX_IFACTINACCESSCONTROL_H__06C6E33B_A2F6_49DF_ADC8_430130B40960__INCLUDED_)

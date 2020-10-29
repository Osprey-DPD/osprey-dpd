// IActiveSimBox.h: interface for the IActiveSimBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IACTIVESIMBOX_H__84733702_0BD7_4357_A6BF_AFB8082E8C9F__INCLUDED_)
#define AFX_IACTIVESIMBOX_H__84733702_0BD7_4357_A6BF_AFB8082E8C9F__INCLUDED_


#include "SimACNFlags.h"

// Forward declarations

#if EnableShadowSimBox == SimACNEnabled
class IModifyActiveCellNetwork;
class IACNAccessControl;
class IActivePolymerForms;
class IActivePolymerFormsBondTriple;
class IActivePolymerDissolves;
class IActiveBondBindsToPolymer;
class IActiveBondUnbindsFromPolymer;
class IActiveBondHydrolysesATP;
class IActiveBondReleasesPi;
class IActiveBondPhosphorylation;
#endif


#include "xxBase.h"

class IActiveSimBox : public xxBase
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

	// Access functions to the original SimBox data

	virtual const zString GetRunId()  const = 0;
	virtual long   GetCurrentTime()   const = 0;
	virtual double GetSimBoxXLength() const = 0;
	virtual double GetSimBoxYLength() const = 0;
	virtual double GetSimBoxZLength() const = 0;
	virtual double GetHalfSimBoxXLength() const = 0;
	virtual double GetHalfSimBoxYLength() const = 0;
	virtual double GetHalfSimBoxZLength() const = 0;


#if EnableShadowSimBox == SimACNEnabled

    // Function to show if an ACN exists

    virtual bool IsACNPresent(const zString acnType) const = 0;

    // Function to return a pointer to a given ACN

	virtual aeActiveCellNetwork* GetACNFromType(const zString acnType) const = 0;

	// Function to return a pointer to an ACN's command interface given its type

	virtual IModifyActiveCellNetwork* GetIModifyActiveCellNetworkFromType(const zString type) = 0;

	// Function to return a pointer to an ACN's access control interface given its type

	virtual IACNAccessControl* GetIACNAccessControlFromType(const zString type) = 0;

	// Active Event Interfaces

	virtual IActivePolymerForms* GetIActivePolymerForms() = 0;
	virtual IActivePolymerFormsBondTriple* GetIActivePolymerFormsBondTriple() = 0;
	virtual IActivePolymerDissolves* GetIActivePolymerDissolves() = 0;
	virtual IActiveBondBindsToPolymer* GetIActiveBondBindsToPolymer() = 0;
	virtual IActiveBondUnbindsFromPolymer* GetIActiveBondUnbindsFromPolymer() = 0;
	virtual IActiveBondHydrolysesATP* GetIActiveBondHydrolysesATP() = 0;
	virtual IActiveBondReleasesPi* GetIActiveBondReleasesPi() = 0;
	virtual IActiveBondPhosphorylation* GetIActiveBondPhosphorylation() = 0;
#endif

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
	// Data members: No member variables as this is an interface class
private:


};

#endif // !defined(AFX_IACTIVESIMBOX_H__84733702_0BD7_4357_A6BF_AFB8082E8C9F__INCLUDED_)

// IModifyBLMVesicleFusion.h: interface for the IModifyBLMVesicleFusion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMODIFYBLMVESICLEFUSION_H__838B6528_D526_4499_8F3B_48FB1922D40D__INCLUDED_)
#define AFX_IMODIFYBLMVESICLEFUSION_H__838B6528_D526_4499_8F3B_48FB1922D40D__INCLUDED_


#include "IModifyProcess.h"

class IModifyBLMVesicleFusion : public IModifyProcess
{
	// ****************************************
	// Construction/Destruction: protected constructor declared below
public:

	virtual ~IModifyBLMVesicleFusion();

	// ****************************************
	// Global functions, static member functions and variables
public:

	static IModifyBLMVesicleFusion* GetInterfacePointer(IModifyProcess* pIMP);

	// ****************************************
	// PVFs that must be overridden by the derived process class
public:

	virtual void        SetMaxCMApproach(const xxCommand* const pCommand) = 0;
	virtual void       SetMaxDistalTouch(const xxCommand* const pCommand) = 0;
	virtual void      SetMinMixThreshold(const xxCommand* const pCommand) = 0;
	virtual void          SetMinPoreSize(const xxCommand* const pCommand) = 0;
	virtual void    SetMinFusionPoreSize(const xxCommand* const pCommand) = 0;
	virtual void        SetCondenseDelay(const xxCommand* const pCommand) = 0;
	virtual void     SetCondenseDuration(const xxCommand* const pCommand) = 0;
	virtual void         SetStretchDelay(const xxCommand* const pCommand) = 0;
	virtual void      SetStretchDuration(const xxCommand* const pCommand) = 0;
	virtual void          SetInnerRadius(const xxCommand* const pCommand) = 0;
	virtual void          SetOuterRadius(const xxCommand* const pCommand) = 0;
	virtual void           SetHalfLength(const xxCommand* const pCommand) = 0;
	virtual void SetRadialForceMagnitude(const xxCommand* const pCommand) = 0;

	// ****************************************
	// Public access functions
public:

	// ****************************************
	// Protected local functions
protected:

	IModifyBLMVesicleFusion();

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members: No member variables as this is an interface class
private:

};

#endif // !defined(AFX_IMODIFYBLMVESICLEFUSION_H__838B6528_D526_4499_8F3B_48FB1922D40D__INCLUDED_)

// mcPanCurrentStateCameraImpl.h: interface for the mcPanCurrentStateCameraImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCPANCURRENTSTATECAMERAIMPL_H__32086E1F_64F8_484B_B259_7CE3494E7CA5__INCLUDED_)
#define AFX_MCPANCURRENTSTATECAMERAIMPL_H__32086E1F_64F8_484B_B259_7CE3494E7CA5__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcPanCurrentStateCameraImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcPanCurrentStateCameraImpl();

	virtual ~mcPanCurrentStateCameraImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void PanCurrentStateCamera(const xxCommand* const pCommand);


	// ****************************************
	// Protected local functions
protected:

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:

	void CalculateNRotationMatrix();

	// ****************************************
	// Data members
private:

	double m_OriginalCamera[3];	// Pre-command position of camera
	double m_OriginalTarget[3];	// Pre-command position of target (look-at point)
	double m_Normal[3];			// Normal to pan
	double m_RNormal[3][3];		// Rotation matrix for normal direction
	double m_RPan[3][3];		// Rotation matrix for each snapshot in the pan

	double m_Theta;				// Polar angle of normal vector
	double m_Phi;				// Azimuthal angle of normal vector


};

#endif // !defined(AFX_MCPANCURRENTSTATECAMERAIMPL_H__32086E1F_64F8_484B_B259_7CE3494E7CA5__INCLUDED_)

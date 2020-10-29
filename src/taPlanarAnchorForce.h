// taPlanarAnchorForce.h: interface for the taPlanarAnchorForce class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TAPLANARANCHORFORCE_H__0280ea7c_6c0f_4cf9_b72a_66588172be29__INCLUDED_)
#define AFX_TAPLANARANCHORFORCE_H__0280ea7c_6c0f_4cf9_b72a_66588172be29__INCLUDED_



#include "taForceDecorator.h"

class taPlanarAnchorForce : public taForceDecorator
{
public:
	// ****************************************
	// Construction/Destruction
public:

	taPlanarAnchorForce(const zString label, CCommandTargetNode* const pDec,
				  long xn, long yn, long zn, 
			  	  double xc, double yc, double zc, double mag);

	taPlanarAnchorForce(const zString label);

    virtual ~taPlanarAnchorForce();

    // ****************************************
	// Global functions, static member functions and variables
public:

    static const zString GetType();		// return the target's type

private:

	static const zString m_Type;

	// ****************************************
	// PVFs that must be implemented by all instantiated derived classes 
public:

    const zString GetTargetType() const;    // return the target's type

    // Implementation of the ISerialiseInclusiveRestartState interface
    // to allow this class to read/write data that can be modified
    // for restarts.

    virtual zInStream& Read(zInStream& is);
    virtual zOutStream& Write(zOutStream& is) const;

	virtual void Execute(long simTime);

	virtual double GetXForce(long simTime);
	virtual double GetYForce(long simTime);
	virtual double GetZForce(long simTime);

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

protected:


private:

	long m_X;
	long m_Y;	// Integer components of plane normal
	long m_Z;

	double	m_XOrigin;
	double	m_YOrigin;		// Coordinates of anchor point in units of bead diameter
	double	m_ZOrigin;

	double m_keff;	        // Spring constant of the Hookean restoring force

	double m_FX;
	double m_FY;		    // Force components
	double m_FZ;

	// Local data

	double	m_XNormal;
	double	m_YNormal;		// Normal vector to plane
	double	m_ZNormal;

	double m_RelOX;
	double m_RelOY;			// Bead position vector relative to anchor point
	double m_RelOZ;
	double m_RelDotN;		// Scalar product of rel. pos. vector and axis direction
};

#endif // !defined(AFX_TAPLANARANCHORFORCE_H__0280ea7c_6c0f_4cf9_b72a_66588172be29__INCLUDED_)

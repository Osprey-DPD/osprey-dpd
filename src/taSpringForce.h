// taSpringForce.h: interface for the taSpringForce class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASPRINGFORCE_H__6D69F6C9_AAB9_49FC_90F7_F507FCD866F6__INCLUDED_)
#define AFX_TASPRINGFORCE_H__6D69F6C9_AAB9_49FC_90F7_F507FCD866F6__INCLUDED_



#include "taForceDecorator.h"

class taSpringForce : public taForceDecorator
{
public:
	// ****************************************
	// Construction/Destruction
public:

	taSpringForce(const zString label, CCommandTargetNode* const pDec, 
			  	  double xc, double yc, double zc, double mag);

    taSpringForce(const zString label);

	virtual ~taSpringForce();

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

	double	m_XOrigin;
	double	m_YOrigin;		// Coordinates of restraint origin in units of bead diameter
	double	m_ZOrigin;

	double m_keff;	        // Coefficient of the spring force (= 2*k for harmonic potential)

	double m_FX;
	double m_FY;		    // Force components
	double m_FZ;

	// Local data

	double m_RelOX;
	double m_RelOY;			// Bead position vector relative to force origin
	double m_RelOZ;

};

#endif // !defined(AFX_TASPRINGFORCE_H__6D69F6C9_AAB9_49FC_90F7_F507FCD866F6__INCLUDED_)

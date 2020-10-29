// taSineForce.h: interface for the taSineForce class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASINEFORCE_H__745D9D25_6166_4FEE_B570_7EF5D26937BD__INCLUDED_)
#define AFX_TASINEFORCE_H__745D9D25_6166_4FEE_B570_7EF5D26937BD__INCLUDED_


#include "taForceDecorator.h"

class taSineForce : public taForceDecorator
{
public:
	// ****************************************
	// Construction/Destruction
public:

	taSineForce(const zString label, CCommandTargetNode* const pDec,
				long start, long xn, long yn, long zn, 
				double amp, double period);

	taSineForce(const zString label);

    virtual ~taSineForce();

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

	long	m_StartTime;	// Timestep at which force increases from zero
	long    m_X;
	long    m_Y;			// Integer components of force direction
	long    m_Z;
	double	m_Amplitude;	// Amplitude of sinusoidal force
	double	m_Period;		// Period of force in time steps
	double	m_Frequency;	// Frequency of sinusoidal force

	double m_AmpX;			// Component of force in X direction
	double m_AmpY;			// Component of force in Y direction
	double m_AmpZ;			// Component of force in Z direction

	double m_FX;
	double m_FY;			// Force components
	double m_FZ;

};

#endif // !defined(AFX_TASINEFORCE_H__745D9D25_6166_4FEE_B570_7EF5D26937BD__INCLUDED_)

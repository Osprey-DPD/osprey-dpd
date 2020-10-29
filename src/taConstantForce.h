// taConstantForce.h: interface for the taConstantForce class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TACONSTANTFORCE_H__E784EE76_006C_44EC_B1FD_E53942DAC305__INCLUDED_)
#define AFX_TACONSTANTFORCE_H__E784EE76_006C_44EC_B1FD_E53942DAC305__INCLUDED_


#include "taForceDecorator.h"

class taConstantForce : public taForceDecorator
{
public:
	// ****************************************
	// Construction/Destruction
public:

	taConstantForce(const zString label, CCommandTargetNode* const pDec,
					long xn, long yn, long zn, double mag);

	taConstantForce(const zString label);

	virtual ~taConstantForce();

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

	virtual double GetXForce(long simTime)	{return m_FX;}
	virtual double GetYForce(long simTime)	{return m_FY;}
	virtual double GetZForce(long simTime)	{return m_FZ;}


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
	long m_Y;		    // Integer components of force direction
	long m_Z;
	double m_Magnitude;	// Magnitude of the force

	double m_FX;
	double m_FY;		// Force components
	double m_FZ;

};

#endif // !defined(AFX_TACONSTANTFORCE_H__E784EE76_006C_44EC_B1FD_E53942DAC305__INCLUDED_)

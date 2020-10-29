// taLinearForce.h: interface for the taLinearForce class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TALINEARFORCE_H__F76287D7_35F8_4F11_BE63_D12F3FC2A77A__INCLUDED_)
#define AFX_TALINEARFORCE_H__F76287D7_35F8_4F11_BE63_D12F3FC2A77A__INCLUDED_


#include "taForceDecorator.h"

class taLinearForce : public taForceDecorator
{
public:
	// ****************************************
	// Construction/Destruction
public:

	taLinearForce(const zString label, CCommandTargetNode* const pDec,
				  long start, long xn, long yn, long zn, double rate);

	taLinearForce(const zString label);
    
    virtual ~taLinearForce();

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

	long m_StartTime;	// Timestep at which force increases from zero
	long m_X;
	long m_Y;	        // Integer components of force direction
	long m_Z;
	double m_Rate;	    // Rate at which force grows in time

    // Local data

	double m_RateX;		// Time-independent part of X force
	double m_RateY;		// Time-independent part of Y force
	double m_RateZ;		// Time-independent part of Z force

	double m_FX;
	double m_FY;		// Force components
	double m_FZ;

};

#endif // !defined(AFX_TALINEARFORCE_H__F76287D7_35F8_4F11_BE63_D12F3FC2A77A__INCLUDED_)

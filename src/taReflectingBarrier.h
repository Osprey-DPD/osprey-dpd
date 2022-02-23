// taReflectingBarrier.h: interface for the taReflectingBarrier class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TAREFLECTINGBARRIER_H__277c8f4e_c95b_4f6c_a545_ba01c587d997__INCLUDED_)
#define AFX_TAREFLECTINGBARRIER_H__277c8f4e_c95b_4f6c_a545_ba01c587d997__INCLUDED_



#include "taForceDecorator.h"

class taReflectingBarrier : public taForceDecorator
{
public:
	// ****************************************
	// Construction/Destruction
public:

	taReflectingBarrier(const zString label, CCommandTargetNode* const pDec,
				  long xn, long yn, long zn, double mag);

	taReflectingBarrier(const zString label);

    virtual ~taReflectingBarrier();

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
	long m_Y;	// Integer components of barrier normal
	long m_Z;

    double m_mag;            // Distance of barrier along axis as fraction of box side length

	// Local data

    double m_XOrigin;
    double m_YOrigin;        // Coordinates of barrier along axis in units of bead diameter
    double m_ZOrigin;

    double m_FX;
    double m_FY;            // Force components
    double m_FZ;

    
};

#endif // !defined(AFX_TAREFLECTINGBARRIER_H__277c8f4e_c95b_4f6c_a545_ba01c587d997__INCLUDED_)

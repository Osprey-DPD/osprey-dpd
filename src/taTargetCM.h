// taTargetCM.h: interface for the taTargetCM class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TATARGETCM_H__AC5439C5_E29C_4F23_A91E_7DBB8048D77B__INCLUDED_)
#define AFX_TATARGETCM_H__AC5439C5_E29C_4F23_A91E_7DBB8048D77B__INCLUDED_



#include "taCumulateDecorator.h"

class taTargetCM : public taCumulateDecorator
{
public:
	// ****************************************
	// Construction/Destruction
public:

	taTargetCM(const zString label, CCommandTargetNode* const pDec,
					              long start, long end);

	virtual ~taTargetCM();

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

    double m_BeadTotal;    // Number of beads in target contributing to CM calculation
	double m_XCM;		   // X component of target's CM
	double m_YCM;		   // Y component of target's CM
	double m_ZCM;		   // Z component of target's CM

    BeadVector  m_vBeads;  // Vector of beads in target

};

#endif // !defined(AFX_TATARGETCM_H__AC5439C5_E29C_4F23_A91E_7DBB8048D77B__INCLUDED_)

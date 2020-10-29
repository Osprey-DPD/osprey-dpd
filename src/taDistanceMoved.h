// taDistanceMoved.h: interface for the taDistanceMoved class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TADISTANCEMOVED_H__F7B7837E_C14E_41CF_919F_ED280FA7DD85__INCLUDED_)
#define AFX_TADISTANCEMOVED_H__F7B7837E_C14E_41CF_919F_ED280FA7DD85__INCLUDED_


// Forward declarations

class taForceDecorator;


#include "taCumulateDecorator.h"

class taDistanceMoved : public taCumulateDecorator
{
public:
	// ****************************************
	// Construction/Destruction
public:

	taDistanceMoved(const zString label, CCommandTargetNode* const pDec,
					taForceDecorator* const pForceDec, long start, long end);

	virtual ~taDistanceMoved();

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

	taForceDecorator* const m_pForceDec;	// External force whose effect is being measured

	double m_Distance;						// Accumulated distance moved by target beads
	double m_DistancePerBead;				// Average distance moved by target beads

};

#endif // !defined(AFX_TADISTANCEMOVED_H__F7B7837E_C14E_41CF_919F_ED280FA7DD85__INCLUDED_)

// taRgBead.h: interface for the taRgBead class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TARGBEAD_H__e48be13a_9aa9_48ef_9e00_98a23294b566__INCLUDED_)
#define AFX_TARGBEAD_H__e48be13a_9aa9_48ef_9e00_98a23294b566__INCLUDED_



// Forward declarations

class CCommandTarget;


#include "taCumulateDecorator.h"

class taRgBead : public taCumulateDecorator
{
public:
	// ****************************************
	// Construction/Destruction
public:

	taRgBead(const zString label, CCommandTargetNode* const pDec, long start, long end);

	virtual ~taRgBead();

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

	double m_CM[3];			// Target beads' CM (for all polymers even if target is composite)
    double m_Rg;            // Target beads' radius of gyration

    CCommandTarget*  m_pBeadTarget; // Pointer to actual bead target

    BeadVector  m_vBeads;
    long        m_Total;
};

#endif // !defined(AFX_TARGBEAD_H__e48be13a_9aa9_48ef_9e00_98a23294b566__INCLUDED_)

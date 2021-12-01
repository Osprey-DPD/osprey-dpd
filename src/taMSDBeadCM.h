// taMSDBeadCM.h: interface for the taMSDBeadCM class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TAMSDBEADCM_H__ca65a1fc_578c_40c8_b5ea_92c81971c460__INCLUDED_)
#define AFX_TAMSDBEADCM_H__ca65a1fc_578c_40c8_b5ea_92c81971c460__INCLUDED_



// Forward declarations

class CCommandTarget;


#include "taCumulateDecorator.h"

class taMSDBeadCM : public taCumulateDecorator
{
public:
	// ****************************************
	// Construction/Destruction
public:

	taMSDBeadCM(const zString label, CCommandTargetNode* const pDec,
				   long start, long end);

	virtual ~taMSDBeadCM();

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

    zDoubleVector m_vXInitial;      // Initial coordinates of Beads' CM
    zDoubleVector m_vYInitial;
    zDoubleVector m_vZInitial;

	zDoubleVector m_vMSD;			// Target Beads' CM mean-square displacement
                                    // (for all Beads even if target is composite)

    CCommandTarget*  m_pBeadTarget; // Pointer to actual Bead target

    BeadVector  m_vBeads;
    long           m_BeadTotal;
};

#endif // !defined(AFX_TAMSDBEADCM_H__ca65a1fc_578c_40c8_b5ea_92c81971c460__INCLUDED_)

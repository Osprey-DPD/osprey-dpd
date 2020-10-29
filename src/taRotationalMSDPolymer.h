// taRotationalMSDPolymer.h: interface for the taRotationalMSDPolymer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TAROTATIONALMSDPOLYMER_H__27D2FB3C_0314_41BD_9B40_0335D95CF88E__INCLUDED_)
#define AFX_TAROTATIONALMSDPOLYMER_H__27D2FB3C_0314_41BD_9B40_0335D95CF88E__INCLUDED_



// Forward declarations

class CCommandTargetPolymer;


#include "taCumulateDecorator.h"

class taRotationalMSDPolymer : public taCumulateDecorator
{
public:
	// ****************************************
	// Construction/Destruction
public:

	taRotationalMSDPolymer(const zString label, CCommandTargetNode* const pDec,
				   long start, long end);

	virtual ~taRotationalMSDPolymer();

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

    zDoubleVector m_vXInitial;      // Initial coordinates of polymers' CM
    zDoubleVector m_vYInitial;
    zDoubleVector m_vZInitial;

	zDoubleVector m_vMSD;			// Target polymers' CM mean-square displacement
                                    // (for all polymers even if target is composite)

    CCommandTargetPolymer*  m_pPolymerTarget; // Pointer to actual polymer target

    PolymerVector  m_vPolymers;
    long           m_PolymerTotal;
};

#endif // !defined(AFX_TAROTATIONALMSDPOLYMER_H__27D2FB3C_0314_41BD_9B40_0335D95CF88E__INCLUDED_)

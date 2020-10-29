// taMSDPolymerCM.h: interface for the taMSDPolymerCM class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TAMSDPOLYMERCM_H__E8647596_09CC_4B25_86A8_83AAF3BAD890__INCLUDED_)
#define AFX_TAMSDPOLYMERCM_H__E8647596_09CC_4B25_86A8_83AAF3BAD890__INCLUDED_



// Forward declarations

class CCommandTargetPolymer;


#include "taCumulateDecorator.h"

class taMSDPolymerCM : public taCumulateDecorator
{
public:
	// ****************************************
	// Construction/Destruction
public:

	taMSDPolymerCM(const zString label, CCommandTargetNode* const pDec,
				   long start, long end);

	virtual ~taMSDPolymerCM();

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

#endif // !defined(AFX_TAMSDPOLYMERCM_H__E8647596_09CC_4B25_86A8_83AAF3BAD890__INCLUDED_)

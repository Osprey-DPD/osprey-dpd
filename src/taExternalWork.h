// taExternalWork.h: interface for the taExternalWork class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TAEXTERNALWORK_H__D46EDE64_6AB9_40AA_A33D_D9591EB93A05__INCLUDED_)
#define AFX_TAEXTERNALWORK_H__D46EDE64_6AB9_40AA_A33D_D9591EB93A05__INCLUDED_


// Forward declarations

class taForceDecorator;


#include "taCumulateDecorator.h"
#include "xxFile.h"

class taExternalWork : public taCumulateDecorator, public xxFile
{
public:
	// ****************************************
	// Construction/Destruction
public:

	taExternalWork(const zString label, CCommandTargetNode* const pDec,
				   taForceDecorator* const pForceDec, long start, long end);

	virtual ~taExternalWork();

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

    virtual bool Serialize();


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

	taForceDecorator* const m_pForceDec;	// External force whose work is being measured

	double m_Work;							// Accumulated work done on target beads

};

#endif // !defined(AFX_TAEXTERNALWORK_H__D46EDE64_6AB9_40AA_A33D_D9591EB93A05__INCLUDED_)

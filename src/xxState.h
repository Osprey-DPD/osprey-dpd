// xxState.h: interface for the xxState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXSTATE_H__83457701_3C59_11D3_820E_0060088AD300__INCLUDED_)
#define AFX_XXSTATE_H__83457701_3C59_11D3_820E_0060088AD300__INCLUDED_


#include "xxFile.h"

class xxState : public xxFile
{
	// ****************************************
	// Construction/Destruction: protected constructor declared below
public:

	virtual ~xxState();

	// ****************************************
	// Global functions, static member functions and variables
public:

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:


	// ****************************************
	// Public access functions
public:

	inline const zString GetRunId() const {return m_RunId;}
    inline long GetCurrentTime()    const {return m_CurrentTime;}

	// ****************************************
	// Protected local functions
protected:

	// Protected constructor ensures that only subclasses can be instantiated

	xxState(zString FileName, bool IOFlag, long currentTime, const zString runId);

	xxState(zString FileName, bool IOFlag, long currentTime);
	
	// ****************************************
	// Implementation

protected:

	long m_CurrentTime;

	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

	const zString m_RunId;		// RunId cannot change during the run

};

#endif // !defined(AFX_XXSTATE_H__83457701_3C59_11D3_820E_0060088AD300__INCLUDED_)

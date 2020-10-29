// LogCNTBeadError.h: interface for the CLogCNTBeadError class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCNTBEADERROR_H__55BD6044_634D_46C7_816D_7D3FE283E69D__INCLUDED_)
#define AFX_LOGCNTBEADERROR_H__55BD6044_634D_46C7_816D_7D3FE283E69D__INCLUDED_


// Forward declarations

class CAbstractBead;


#include "LogWarningMessage.h"

class CLogCNTBeadError : public CLogWarningMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogCNTBeadError(long time, long ix, long iy, long iz, 
					 long expectIndex, long actualIndex, const CAbstractBead* const pBead);

	virtual ~CLogCNTBeadError();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogCNTBeadError& rMsg);

	// ****************************************
	// Public access functions
public:

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	virtual	void Serialize(zOutStream& os) const;

	// ****************************************
	// Protected local functions
protected:

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:
	
	// Explicitly disallow the copy constructor and assignment operators
	// by declaring them private and providing NO definitions.

	CLogCNTBeadError(const CLogCNTBeadError& oldMessage);
	CLogCNTBeadError& operator=(const CLogCNTBeadError& rhs);

	// ****************************************
	// Data members
private:

	long	m_CellXNo;		// x,y,z indices of CNT cell
	long	m_CellYNo;
	long	m_CellZNo;
	long	m_ExpectIndex;	// Index of CNT cell using bead coordinates
	long	m_ActualIndex;	// Actual CNT cell index containing bead

	const CAbstractBead* const m_pBead;

};

#endif // !defined(AFX_LOGCNTBEADERROR_H__55BD6044_634D_46C7_816D_7D3FE283E69D__INCLUDED_)


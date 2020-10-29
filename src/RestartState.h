// RestartState.h: interface for the CRestartState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESTARTSTATE_H__4EFD3500_5ECD_11D3_820E_0060088AD300__INCLUDED_)
#define AFX_RESTARTSTATE_H__4EFD3500_5ECD_11D3_820E_0060088AD300__INCLUDED_


// Forward declarations

class CInitialState;

#include "xxState.h"

class CRestartState : public xxState  
{
	// ****************************************
	// Construction/Destruction
public:

    CRestartState(long currentTime, zString runId, const CInitialState& riState);
	CRestartState(long currentTime, zString runId, bool IOFlag, bool bLogWarnings, const CInitialState& riState);

	virtual ~CRestartState();

	// ****************************************
	// Global functions, static member functions and variables
public:

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

    bool Serialize();

    bool AreWarningMessagesLogged() const;

    inline bool IsUnexpectedBeadTypeFound() const {return m_bUnexpectedBeadTypeFound;}
    inline long GetLargestBeadTypeFound()   const {return m_LargestBeadTypeFound;}

	// ****************************************
	// Protected local functions
protected:

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:

	bool CheckBeadWithinBox(long id, long type, double& xp, double& yp, double& zp);

	// ****************************************
	// Data members
private:

	const CInitialState& m_riState;

    const bool m_bLogWarningMessages;  // Flag showing if warning messages are logged

    const double m_CoordErrorLimit;	   // epsilon used to distinguish coords from boundaries

	// Local storage to avoid accessing CInitialState SimBox data while writing
	// a restart file.

	const double m_SimBoxXLength;
	const double m_SimBoxYLength;
	const double m_SimBoxZLength;

    const double m_SimBoxXMinusEpsilon; // Used to reduce number of flops per test
    const double m_SimBoxYMinusEpsilon; // Used to reduce number of flops per test
    const double m_SimBoxZMinusEpsilon; // Used to reduce number of flops per test

    bool  m_bUnexpectedBeadTypeFound;   // Flag showing if a bead has a larger type than expected
    long  m_ExpectedLargestBeadType;    // Largest bead type expected from CDF
    long  m_LargestBeadTypeFound;       // Largest bead type found

};

#endif // !defined(AFX_RESTARTSTATE_H__4EFD3500_5ECD_11D3_820E_0060088AD300__INCLUDED_)

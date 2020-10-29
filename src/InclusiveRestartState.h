// InclusiveRestartState.h: interface for the CInclusiveRestartState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INCLUSIVERESTARTSTATE_H__E6D0FDCF_7AD7_4B21_914D_53C26C38EB6F__INCLUDED_)
#define AFX_INCLUSIVERESTARTSTATE_H__E6D0FDCF_7AD7_4B21_914D_53C26C38EB6F__INCLUDED_


// Forward declarations

class ISimBox;
class CAnalysisState;
class CCurrentState;
class CInitialState;

#include "xxState.h"

class CInclusiveRestartState : public xxState  
{
	// ****************************************
	// Construction/Destruction
public:

    // Constructor for use when reading a restart state from file

	CInclusiveRestartState(long currentTime, zString runId, CInitialState& riState);

    // Constructor for use when writing a restart state to file

	CInclusiveRestartState(long currentTime, zString runId, bool IOFlag, bool bLogWarnings, 
                           const ISimBox* const pISimBox, CAnalysisState* paState, CInitialState& riState);

    // Constructor for use when writing a non-bead restart state to file:
    // the first boolean flag indicates that this is a non-bead restart state.
    // It is automatically false for the previous constructors.

	CInclusiveRestartState(long currentTime, zString runId, bool bNonBeadState, bool IOFlag, bool bLogWarnings, 
                           const ISimBox* const pISimBox, CAnalysisState* paState, CInitialState& riState);

	virtual ~CInclusiveRestartState();

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
private:

    const ISimBox* const m_pISimBox;
    CAnalysisState* const m_paState;
    CInitialState&  m_riState;

    const bool m_bNonBeadRestartState; // Flag showing if bead coordinates are to be serialised
    const bool m_bLogWarningMessages;  // Flag showing if warning messages are logged

};

#endif // !defined(AFX_INCLUSIVERESTARTSTATE_H__E6D0FDCF_7AD7_4B21_914D_53C26C38EB6F__INCLUDED_)

// IInclusiveRestartState.h: interface for the IInclusiveRestartState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IINCLUSIVERESTARTSTATE_H__28FA1A5E_D8D2_4D77_A6A8_246DEC2A94B7__INCLUDED_)
#define AFX_IINCLUSIVERESTARTSTATE_H__28FA1A5E_D8D2_4D77_A6A8_246DEC2A94B7__INCLUDED_


// Forward declarations

class CSimState;

// Include files

#include "xxBase.h"


class IInclusiveRestartState  
{
	// ****************************************
	// Construction/Destruction
public:

	IInclusiveRestartState(CSimState* pSimState);

	virtual ~IInclusiveRestartState();

	// ****************************************
	// Global functions, static member functions and variables
public:

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:


	// ****************************************
	// Public access functions
public:

    // Access functions to other class' data

	const zString GetBeadNameFromType(long type) const;
	const zString GetBondNameFromType(long type) const;
	const zString GetBondPairNameFromType(long type) const;
    const zString GetPolymerNameFromType(long type) const;

    // Functions to build up the new initial state

	bool AddBeadType(long type, const zString name);
	bool AddBondType();
	bool AddBondPairType();

    // BD interaction parameters - not implemented yet

	void SetBDBeadIntArrays();

    // DPD bead-bead interaction parameters

	void SetDPDBeadIntArrays(const zArray2dDouble* pvvConsInt, const zArray2dDouble* pvvDissInt);

    // MD interaction parameters - not implemented yet

	void SetMDBeadIntArrays();

    // Following functions not implemented yet
	bool AddPolymerType();

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

    CSimState* const m_pSimState;
};

#endif // !defined(AFX_IINCLUSIVERESTARTSTATE_H__28FA1A5E_D8D2_4D77_A6A8_246DEC2A94B7__INCLUDED_)

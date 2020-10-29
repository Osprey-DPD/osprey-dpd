// prFormin.h: interface for the prFormin class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRFORMIN_H__1D414E9A_6C63_446F_87BF_300CF5B3A2EC__INCLUDED_)
#define AFX_PRFORMIN_H__1D414E9A_6C63_446F_87BF_300CF5B3A2EC__INCLUDED_


// Forward declarations

class aeForminNetwork;



#include "IModifyForminProcess.h"

class prFormin : public IModifyForminProcess
{
	// ****************************************
	// Construction/Destruction
public:

	prFormin();

	// Constructor for use by the shadow SimBox to attach an ACN to a process

	prFormin(const aeForminNetwork* pACN, long start, long end);

	virtual ~prFormin();

	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	// Return the process name

private:

	static const zString m_Type;	// Identifier used in control data file

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an xxProcess pointer,
	// and respond to the SimBox's request to sample their data.

	virtual zOutStream& put(zOutStream& os) const;
	virtual zInStream&  get(zInStream& is);

	// Function to allow the process to monitor aggregates and check for events

	virtual void UpdateState(CSimState& rSimState, const ISimBox* const pISimBox);

	virtual xxProcess* GetProcess()  const;

	// Non-static member function to return the (static) process name that
	// must be provided by each derived class. A static GetType() function
	// must also be provided.

	virtual const zString GetProcessType() const;

	// Function to allow the CInputData object to check all processes' data

	virtual bool ValidateData(const CInputData &riData);

	// Function to allow process validation when it is created internally

	virtual bool InternalValidateData(const ISimState* const pISimState);

// ****************************************
	// Implementation of the IModifyProcess interface

	virtual bool IsModifiable() const;
	virtual void SetModifiable(const xxCommand* const pCommand);
	virtual void SetUnmodifiable(const xxCommand* const pCommand);
	virtual void SetIrreversibleUnmodifiable(const xxCommand* const pCommand);

// ****************************************
	// Implementation of the IModifyForminProcess interface

	virtual void SetCondenseDelay(const xxCommand* const pCommand);

	// ****************************************
	// Public access functions
public:

	void SetACN(const aeForminNetwork* pACN);


	// ****************************************
	// Protected local functions
protected:


	// ****************************************
	// Implementation


	// ****************************************
	// Private functions

	// ****************************************
	// Data members

private:								// Data specific to the process

	const aeForminNetwork* m_pACN;		// ACN that supplies data to this process

	// Data required to implement the IModifyProcess interface 

	bool m_bIsModifiable;			// Flag showing if the process is currently modifiable
	bool m_bNeverModifiable;		// Flag showing if the process is not modifiable

	// Local data for the process

	long	m_MonomerTotal;			// Number of free and bound monomers
	long	m_FreeMonomerTotal;		// Number of free monomers
	long	m_BoundMonomerTotal;	// Number of bound monomers
	long	m_FilamentTotal;		// Number of filaments

	double  m_FreeMonomerFraction;	// Fraction of monomers free in solution
	double  m_BoundMonomerFraction;	// Fraction of monomers in filaments
	double  m_MeanFilamentSize;		// Mean number of monomers/filament (not the end-to-end length)
	double  m_SDevFilamentSize;		// Std. Dev. of monomers/filament 
	double  m_MeanFilamentLength;	// Mean filament end-to-end length
	double  m_SDevFilamentLength;	// Std. Dev. of filament end-to-end length

	long m_CondenseDelay;			// Sample modifiable property

};

#endif // !defined(AFX_PRFORMIN_H__1D414E9A_6C63_446F_87BF_300CF5B3A2EC__INCLUDED_)

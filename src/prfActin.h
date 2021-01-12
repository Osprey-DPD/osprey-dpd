// prfActin.h: interface for the prfActin class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRFACTIN_H__353AD3DB_CEC3_4F3C_A4FB_D83672678956__INCLUDED_)
#define AFX_PRFACTIN_H__353AD3DB_CEC3_4F3C_A4FB_D83672678956__INCLUDED_


// Forward declarations

class aefActinNetwork;



#include "IModifyfActinProcess.h"

class prfActin : public IModifyfActinProcess
{
	// ****************************************
	// Construction/Destruction
public:

	prfActin();

	// Constructor for use by the shadow SimBox to attach an ACN to a process

	prfActin(const aefActinNetwork* pACN, long start, long end);

	virtual ~prfActin();

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
	// Implementation of the IModifyfActinProcess interface

	virtual void SetCondenseDelay(const xxCommand* const pCommand);

	// ****************************************
	// Public access functions
public:

	void SetACN(const aefActinNetwork* pACN);

	// ****************************************
	// Protected local functions
protected:


	// ****************************************
	// Implementation


	// ****************************************
	// Private functions

	// ****************************************
	// Data members

private:					// Data specific to the process

	const aefActinNetwork* m_pACN;	// ACN that supplies data to this process

	// Local data for the process

	long	m_MonomerTotal;		// Number of free and bound monomers
	long	m_FreeMonomerTotal;		// Number of free monomers
	long	m_BoundMonomerTotal;	        // Number of bound monomers
	long	m_FilamentTotal;		// Number of filaments

	double  m_FreeMonomerFraction;	// Fraction of monomers free in solution
	double  m_BoundMonomerFraction;	// Fraction of monomers in filaments
	double  m_MeanFilamentSize;		// Mean number of monomers/filament (not the end-to-end length)
	double  m_SDevFilamentSize;		// Std. Dev. of monomers/filament 
	double  m_MeanFilamentLength;	       // Mean filament end-to-end length
	double  m_SDevFilamentLength;	       // Std. Dev. of filament end-to-end length

	double  m_MeanFilamentLatSqDisp;  // Mean square lateral displacement of monomers
	double  m_VarFilamentLatSqDisp;   // Variance of the lateral displacement of monomers
 
	double  m_MeanFilamentSqDisp[3];  // Mean square displacement of monomers in all filaments
        double  m_VarFilamentSqDisp[3];   // Variance of the displacement of monomers in all filaments

	double  m_MeanFilamentCM[3];	  // Mean CM coordinates of all filaments
	double  m_MeanFilamentHead[3];    // Mean coordinates of filament head monomers
	double  m_MeanFilamentTail[3];    //                                     "
	double  m_VarFilamentCM[3];       // Variance of CM coordinates of all filaments
	double  m_VarFilamentHead[3];     // Variance of coordinates of filament head monomers
	double  m_VarFilamentTail[3];     //                                      "


	long m_CondenseDelay;			// Sample modifiable property

};

#endif // !defined(AFX_PRFACTIN_H__353AD3DB_CEC3_4F3C_A4FB_D83672678956__INCLUDED_)

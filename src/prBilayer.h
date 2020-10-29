// prBilayer.h: interface for the prBilayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRBILAYER_H__8C320400_A4AA_42CD_B96B_CC4A9BCD573A__INCLUDED_)
#define AFX_PRBILAYER_H__8C320400_A4AA_42CD_B96B_CC4A9BCD573A__INCLUDED_


// Include files and forward declarations for template adaptor classes



#include "xxProcess.h"

class prBilayer : public xxProcess
{
	// ****************************************
	// Construction/Destruction
public:

	prBilayer();
	virtual ~prBilayer();

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

	zOutStream& put(zOutStream& os) const;
	zInStream&  get(zInStream& is);

	// Function to allow the process to monitor aggregates and check for events

	virtual void UpdateState(CSimState& rSimState, const ISimBox* const pISimBox);

	virtual xxProcess* GetProcess() const;

	// Non-static member function to return the (static) process name that
	// must be provided by each derived class. A static GetType() function
	// must also be provided.

	virtual const zString GetProcessType() const;	// Return the process name

	// Function to allow the CInputData object to check all processes' data
	
	virtual bool ValidateData(const CInputData &riData);

	// ****************************************
	// Public access functions
public:

	// ****************************************
	// Protected local functions
protected:

	// Functions to allow the xxProcess to store pointers to its 
	// required CAnalysis and xxEvents-derived class(es)
	// rather than doing dynamic_cast every time we want to access them.
	// We cannot provide such a function in the xxProcess base class
	// because different processes may need different numbers of aggregates
	// and events.

	void SetAggregate(const CAnalysis* pAggregate1);

	void SetEvent();

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions

	// ****************************************
	// Data members

private:							// Data specific to the process



	// Local data needed by the process


};

#endif // !defined(AFX_PRBILAYER_H__8C320400_A4AA_42CD_B96B_CC4A9BCD573A__INCLUDED_)

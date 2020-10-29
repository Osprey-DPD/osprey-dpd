// acSetATPHydrolysisProbability.h: interface for the acSetATPHydrolysisProbability class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACSETATPHYDROLYSISPROBABILITY_H__53F44E2B_33F1_411A_A7A9_15C1C0D835BB__INCLUDED_)
#define AFX_ACSETATPHYDROLYSISPROBABILITY_H__53F44E2B_33F1_411A_A7A9_15C1C0D835BB__INCLUDED_


#include "xxCommand.h" 

class acSetATPHydrolysisProbability : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	acSetATPHydrolysisProbability(long executionTime);
	acSetATPHydrolysisProbability(const acSetATPHydrolysisProbability& oldCommand);

	virtual ~acSetATPHydrolysisProbability();
	
	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	// Return the type of command

	
private:

	static const zString m_Type;	// Identifier used in control data file for command


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	zOutStream& put(zOutStream& os) const;
	zInStream&  get(zInStream& is);

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an xxCommand pointer,
	// respond to the SimBox's request to execute and return the name of the command.

	virtual bool Execute(long simTime, ISimCmd* const pISimCmd) const;

	virtual const xxCommand* GetCommand() const;

	virtual bool IsDataValid(const CInputData& riData) const;

	// ****************************************
	// Public access functions
public:

	inline const zString GetACNType() const {return m_ACNType;}

	inline double GetRate() const {return m_Rate;}

	// ****************************************
	// Protected local functions
protected:

	virtual const zString GetCommandType() const;

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

	zString m_ACNType; // Name of ACN to modify

	double m_Rate;	   // Probability per time-step for bond to detach via its head

};

#endif // !defined(AFX_ACSETATPHYDROLYSISPROBABILITY_H__53F44E2B_33F1_411A_A7A9_15C1C0D835BB__INCLUDED_)

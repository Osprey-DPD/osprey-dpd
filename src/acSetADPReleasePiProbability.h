// acSetADPReleasePiProbability.h: interface for the acSetADPReleasePiProbability class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACSETADPRELEASEPIPROBABILITY_H__E456CC58_3F06_40DE_A098_729442104BE4__INCLUDED_)
#define AFX_ACSETADPRELEASEPIPROBABILITY_H__E456CC58_3F06_40DE_A098_729442104BE4__INCLUDED_


#include "xxCommand.h" 

class acSetADPReleasePiProbability : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	acSetADPReleasePiProbability(long executionTime);
	acSetADPReleasePiProbability(const acSetADPReleasePiProbability& oldCommand);

	virtual ~acSetADPReleasePiProbability();
	
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

#endif // !defined(AFX_ACSETADPRELEASEPIPROBABILITY_H__E456CC58_3F06_40DE_A098_729442104BE4__INCLUDED_)

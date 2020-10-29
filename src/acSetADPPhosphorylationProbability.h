// acSetADPPhosphorylationProbability.h: interface for the acSetADPPhosphorylationProbability class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACSETADPPHOSPHORYLATIONPROBABILITY_H__3BA10F40_34B6_4F5C_AC41_75C0990DC730__INCLUDED_)
#define AFX_ACSETADPPHOSPHORYLATIONPROBABILITY_H__3BA10F40_34B6_4F5C_AC41_75C0990DC730__INCLUDED_


#include "xxCommand.h" 

class acSetADPPhosphorylationProbability : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	acSetADPPhosphorylationProbability(long executionTime);
	acSetADPPhosphorylationProbability(const acSetADPPhosphorylationProbability& oldCommand);

	virtual ~acSetADPPhosphorylationProbability();
	
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

#endif // !defined(AFX_ACSETADPPHOSPHORYLATIONPROBABILITY_H__3BA10F40_34B6_4F5C_AC41_75C0990DC730__INCLUDED_)

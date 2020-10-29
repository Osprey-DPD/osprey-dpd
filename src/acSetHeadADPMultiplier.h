// acSetHeadADPMultiplier.h: interface for the acSetHeadADPMultiplier class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACSETHEADADPMULTIPLIER_H__349646D1_08B6_4B1B_BDD0_CB82089A336D__INCLUDED_)
#define AFX_ACSETHEADADPMULTIPLIER_H__349646D1_08B6_4B1B_BDD0_CB82089A336D__INCLUDED_


#include "xxCommand.h" 

class acSetHeadADPMultiplier : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	acSetHeadADPMultiplier(long executionTime);
	acSetHeadADPMultiplier(const acSetHeadADPMultiplier& oldCommand);

	virtual ~acSetHeadADPMultiplier();
	
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

	inline double GetMultiplier() const {return m_Multiplier;}

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

	double m_Multiplier;    // Multiplier of the bond's basal transition rate

};

#endif // !defined(AFX_ACSETHEADADPMULTIPLIER_H__349646D1_08B6_4B1B_BDD0_CB82089A336D__INCLUDED_)

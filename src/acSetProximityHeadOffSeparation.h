// acSetProximityHeadOffSeparation.h: interface for the acSetProximityHeadOffSeparation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACSETPROXIMITYHEADOFFSEPARATION_H__D0C98086_F6C1_4D09_A316_8071A15CEE10__INCLUDED_)
#define AFX_ACSETPROXIMITYHEADOFFSEPARATION_H__D0C98086_F6C1_4D09_A316_8071A15CEE10__INCLUDED_


#include "xxCommand.h" 

class acSetProximityHeadOffSeparation : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	acSetProximityHeadOffSeparation(long executionTime);
	acSetProximityHeadOffSeparation(const acSetProximityHeadOffSeparation& oldCommand);

	virtual ~acSetProximityHeadOffSeparation();
	
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

	inline double GetSeparation() const {return m_MaxSeparation;}

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

	zString m_ACNType;		 // Name of ACN to modify

	double m_MaxSeparation;	 // Maximum distance at which bonds will dissociate

};

#endif // !defined(AFX_ACSETPROXIMITYHEADOFFSEPARATION_H__D0C98086_F6C1_4D09_A316_8071A15CEE10__INCLUDED_)

// acSetProximityHeadOnSeparation.h: interface for the acSetProximityHeadOnSeparation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACSETPROXIMITYHEADONSEPARATION_H__5968DEDB_D2B1_4CE8_84F0_F9C7ED6D1201__INCLUDED_)
#define AFX_ACSETPROXIMITYHEADONSEPARATION_H__5968DEDB_D2B1_4CE8_84F0_F9C7ED6D1201__INCLUDED_


#include "xxCommand.h" 

class acSetProximityHeadOnSeparation : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	acSetProximityHeadOnSeparation(long executionTime);
	acSetProximityHeadOnSeparation(const acSetProximityHeadOnSeparation& oldCommand);

	virtual ~acSetProximityHeadOnSeparation();
	
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

	double m_MaxSeparation;	 // Maximum distance at which bonds can associate

};

#endif // !defined(AFX_ACSETPROXIMITYHEADONSEPARATION_H__5968DEDB_D2B1_4CE8_84F0_F9C7ED6D1201__INCLUDED_)

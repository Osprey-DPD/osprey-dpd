// acSetProximityTailOffSeparation.h: interface for the acSetProximityTailOffSeparation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACSETPROXIMITYTAILOFFSEPARATION_H__CDE5471F_BFF0_409E_90A6_D5F9D5090A1D__INCLUDED_)
#define AFX_ACSETPROXIMITYTAILOFFSEPARATION_H__CDE5471F_BFF0_409E_90A6_D5F9D5090A1D__INCLUDED_


#include "xxCommand.h" 

class acSetProximityTailOffSeparation : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	acSetProximityTailOffSeparation(long executionTime);
	acSetProximityTailOffSeparation(const acSetProximityTailOffSeparation& oldCommand);

	virtual ~acSetProximityTailOffSeparation();
	
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

#endif // !defined(AFX_ACSETPROXIMITYTAILOFFSEPARATION_H__CDE5471F_BFF0_409E_90A6_D5F9D5090A1D__INCLUDED_)

// acSetBondUnbindsFromPolymerHeadEvent.h: interface for the acSetBondUnbindsFromPolymerHeadEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACSETBONDUNBINDSFROMPOLYMERHEADEVENT_H__1CFC53A9_6871_4801_BFA2_9137D0D17D4A__INCLUDED_)
#define AFX_ACSETBONDUNBINDSFROMPOLYMERHEADEVENT_H__1CFC53A9_6871_4801_BFA2_9137D0D17D4A__INCLUDED_


#include "xxCommand.h" 

class acSetBondUnbindsFromPolymerHeadEvent : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	acSetBondUnbindsFromPolymerHeadEvent(long executionTime);
	acSetBondUnbindsFromPolymerHeadEvent(const acSetBondUnbindsFromPolymerHeadEvent& oldCommand);

	virtual ~acSetBondUnbindsFromPolymerHeadEvent();
	
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

	inline long   GetDuration()       const {return m_Duration;}
	inline double GetSpringConstant() const {return m_SpringConstant;}
	inline double GetLength()         const {return m_Length;}


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

	zString m_ACNType;		// Name of ACN to modify

	long    m_Duration;			// No of steps needed for complete binding
	double  m_SpringConstant;	// Final value of the transient bond spring constant
	double  m_Length;			// Final value of bond's unstretched length

};

#endif // !defined(AFX_ACSETBONDUNBINDSFROMPOLYMERHEADEVENT_H__1CFC53A9_6871_4801_BFA2_9137D0D17D4A__INCLUDED_)

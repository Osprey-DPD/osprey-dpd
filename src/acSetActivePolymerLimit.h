// acSetActivePolymerLimit.h: interface for the acSetActivePolymerLimit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACSETACTIVEPOLYMERLIMIT_H__5BA47AF3_A89D_4ED3_A855_D54B0717DAD0__INCLUDED_)
#define AFX_ACSETACTIVEPOLYMERLIMIT_H__5BA47AF3_A89D_4ED3_A855_D54B0717DAD0__INCLUDED_


#include "xxCommand.h" 

class acSetActivePolymerLimit : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	acSetActivePolymerLimit(long executionTime);
	acSetActivePolymerLimit(const acSetActivePolymerLimit& oldCommand);

	virtual ~acSetActivePolymerLimit();
	
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

	inline long GetMaxPolymers() const {return m_MaxPolymers;}


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

	long m_MaxPolymers;	 // Maximum number of polymers in ACN

};

#endif // !defined(AFX_ACSETACTIVEPOLYMERLIMIT_H__5BA47AF3_A89D_4ED3_A855_D54B0717DAD0__INCLUDED_)

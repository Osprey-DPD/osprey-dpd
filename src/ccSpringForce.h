// ccSpringForce.h: interface for the ccSpringForce class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSPRINGFORCE_H__24914BF3_30AF_11D4_BF39_004095086186__INCLUDED_)
#define AFX_CCSPRINGFORCE_H__24914BF3_30AF_11D4_BF39_004095086186__INCLUDED_


#include "ccApplyForce.h"

class ccSpringForce : public ccApplyForce  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ccSpringForce(long executionTime);
	ccSpringForce(const ccSpringForce& oldCommand);

	virtual ~ccSpringForce();

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

	virtual bool Execute(long simTime, ISimCmd* const pISimCmd) const;

	virtual const xxCommand* GetCommand() const;

	virtual bool IsDataValid(const CInputData& riData) const;

	// ****************************************
	// Public access functions
public:

	// Functions to return the details for the given force law

	inline double GetSpringConst()    const {return m_SpringConst;}
	inline double GetSpringLength()   const {return m_SpringLength;}
	inline double GetSpringVelocity() const {return m_Velocity;}

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

	double m_SpringConst;			// Transducer parameters
	double m_SpringLength;
	double m_Velocity;				// Transducer velocity

};

#endif // !defined(AFX_CCSPRINGFORCE_H__24914BF3_30AF_11D4_BF39_004095086186__INCLUDED_)

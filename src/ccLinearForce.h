// ccLinearForce.h: interface for the ccLinearForce class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCLINEARFORCE_H__488D9847_0C5C_11D4_BF30_004095086186__INCLUDED_)
#define AFX_CCLINEARFORCE_H__488D9847_0C5C_11D4_BF30_004095086186__INCLUDED_


#include "ccApplyForce.h"

class ccLinearForce : public ccApplyForce  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ccLinearForce(long executionTime);
	ccLinearForce(const ccLinearForce& oldCommand);

	virtual ~ccLinearForce();

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

	double m_Rate;

};

#endif // !defined(AFX_CCLINEARFORCE_H__488D9847_0C5C_11D4_BF30_004095086186__INCLUDED_)

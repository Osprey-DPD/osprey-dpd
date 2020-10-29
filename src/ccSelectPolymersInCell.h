// ccSelectPolymersInCell.h: interface for the ccSelectPolymersInCell class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSELECTPOLYMERSINCELL_H__1A88D2F6_0D4A_11D4_BF31_004095086186__INCLUDED_)
#define AFX_CCSELECTPOLYMERSINCELL_H__1A88D2F6_0D4A_11D4_BF31_004095086186__INCLUDED_


#include "ccSelection.h"

class ccSelectPolymersInCell : public ccSelection  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ccSelectPolymersInCell(long executionTime);
	ccSelectPolymersInCell(const ccSelectPolymersInCell& oldCommand);

	virtual ~ccSelectPolymersInCell();

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

};

#endif // !defined(AFX_CCSELECTPOLYMERSINCELL_H__1A88D2F6_0D4A_11D4_BF31_004095086186__INCLUDED_)

// mc0B12400927234167B12C1739B865B671.h: interface for the mc0B12400927234167B12C1739B865B671 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MC0B12400927234167B12C1739B865B671_H__BDF89584_019D_4914_8017_E26D3650F44C__INCLUDED_)
#define AFX_MC0B12400927234167B12C1739B865B671_H__BDF89584_019D_4914_8017_E26D3650F44C__INCLUDED_


#include "xxCommand.h"

class mc0B12400927234167B12C1739B865B671 : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	mc0B12400927234167B12C1739B865B671(long executionTime);
	mc0B12400927234167B12C1739B865B671(const mc0B12400927234167B12C1739B865B671& oldCommand);

	virtual ~mc0B12400927234167B12C1739B865B671();
	
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

#endif // !defined(AFX_MC0B12400927234167B12C1739B865B671_H__BDF89584_019D_4914_8017_E26D3650F44C__INCLUDED_)

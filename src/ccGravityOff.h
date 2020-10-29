// ccGravityOff.h: interface for the ccGravityOff class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCGRAVITYOFF_H__B75BC545_E47F_11D3_BF23_004095086186__INCLUDED_)
#define AFX_CCGRAVITYOFF_H__B75BC545_E47F_11D3_BF23_004095086186__INCLUDED_


// Forward declarations

class ISimCmd;


#include "xxCommand.h"
#include "xxBase.h"	// Added by ClassView

class ccGravityOff : public xxCommand  
{
public:
	ccGravityOff(long executionTime);
	ccGravityOff(const ccGravityOff& oldCommand);
	virtual ~ccGravityOff();

	zOutStream& put(zOutStream& os) const;
	zInStream&  get(zInStream& is);

	virtual bool Execute(long simTime, ISimCmd* const pISimCmd) const;

	static const zString GetType();	// Return the type of command

	virtual const xxCommand* GetCommand() const;

	virtual bool IsDataValid(const CInputData& riData) const;

protected:
	virtual const zString GetCommandType() const;

private:
	static const zString m_Type;	// Identifier used in control data file for command

};

#endif // !defined(AFX_CCGRAVITYOFF_H__B75BC545_E47F_11D3_BF23_004095086186__INCLUDED_)

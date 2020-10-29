// ccZeroTotalMom.h: interface for the ccZeroTotalMom class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCZEROTOTALMOM_H__5AA90E63_047E_11D4_BF30_004095086186__INCLUDED_)
#define AFX_CCZEROTOTALMOM_H__5AA90E63_047E_11D4_BF30_004095086186__INCLUDED_


#include "xxCommand.h"

class ccZeroTotalMom : public xxCommand  
{
public:
	ccZeroTotalMom(long executionTime);
	ccZeroTotalMom(const ccZeroTotalMom& oldCommand);

	virtual ~ccZeroTotalMom();

	zOutStream& put(zOutStream& os) const;
	zInStream&  get(zInStream& is);

	virtual bool Execute(long simTime, ISimCmd* const pISimCmd) const;

	static const zString GetType();	// Return the type of command

	virtual const xxCommand* GetCommand() const;	// Return a copy of the command

	virtual bool IsDataValid(const CInputData& riData) const;

protected:
	virtual const zString GetCommandType() const;

private:
	static const zString m_Type;	// Identifier used in control data file for command

};

#endif // !defined(AFX_CCZEROTOTALMOM_H__5AA90E63_047E_11D4_BF30_004095086186__INCLUDED_)

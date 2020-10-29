// ccRenormaliseMomenta.h: interface for the ccRenormaliseMomenta class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCRENORMALISEMOMENTA_H__584118D3_5665_11D4_BF41_004095086186__INCLUDED_)
#define AFX_CCRENORMALISEMOMENTA_H__584118D3_5665_11D4_BF41_004095086186__INCLUDED_


#include "xxCommand.h"

class ccRenormaliseMomenta : public xxCommand  
{
public:
	ccRenormaliseMomenta(long executionTime);
	ccRenormaliseMomenta(const ccRenormaliseMomenta& oldCommand);

	virtual ~ccRenormaliseMomenta();

	zOutStream& put(zOutStream& os) const;
	zInStream&  get(zInStream& is);

	virtual bool Execute(long simTime, ISimCmd* const pISimCmd) const;

	static const zString GetType();	// Return the type of command

	virtual const xxCommand* GetCommand() const;

	virtual bool IsDataValid(const CInputData& riData) const;

	bool ApplyRenormalisation() const;
	long GetPeriod() const;

protected:
	virtual const zString GetCommandType() const;

private:
	static const zString m_Type;	// Identifier used in control data file for command

	long m_Period;					// Renormalisation period
	bool m_bRenormalise;			// Flag showing if renormalisation is on or off
};

#endif // !defined(AFX_CCRENORMALISEMOMENTA_H__584118D3_5665_11D4_BF41_004095086186__INCLUDED_)


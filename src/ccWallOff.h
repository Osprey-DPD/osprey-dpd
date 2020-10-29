// ccWallOff.h: interface for the ccWallOff class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCWALLOFF_H__B75BC544_E47F_11D3_BF23_004095086186__INCLUDED_)
#define AFX_CCWALLOFF_H__B75BC544_E47F_11D3_BF23_004095086186__INCLUDED_


#include "xxCommand.h"

class ccWallOff : public xxCommand  
{
public:
	ccWallOff(long executionTime);
	ccWallOff(long executionTime, bool bZeroTotalMom);
	ccWallOff(const ccWallOff& oldCommand);
	virtual ~ccWallOff();

	// Access functions to the command's data

	bool ZeroTotalMom() const;		// Flag showing whether to zero bead CM motion

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
	bool m_bZeroTotalMom;			// Flag showing whether to zero total bead momentum

};

#endif // !defined(AFX_CCWALLOFF_H__B75BC544_E47F_11D3_BF23_004095086186__INCLUDED_)

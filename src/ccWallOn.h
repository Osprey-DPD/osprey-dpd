// ccWallOn.h: interface for the ccWallOn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCWALLON_H__EBF46FF3_F98F_11D3_BF2D_004095086186__INCLUDED_)
#define AFX_CCWALLON_H__EBF46FF3_F98F_11D3_BF2D_004095086186__INCLUDED_


#include "xxCommand.h"

class ccWallOn : public xxCommand  
{
public:
	ccWallOn(long executionTime);
	ccWallOn(long executionTime, bool bZeroTotalMom);
	ccWallOn(const ccWallOn& oldCommand);
	virtual ~ccWallOn();

	zOutStream& put(zOutStream& os) const;
	zInStream&  get(zInStream& is);

	virtual bool Execute(long simTime, ISimCmd* const pISimCmd) const;

	static const zString GetType();	// Return the type of command

	virtual const xxCommand* GetCommand() const;

	virtual bool IsDataValid(const CInputData& riData) const;

	// Access functions to the command's data

	bool ZeroTotalMom() const;		// Flag showing whether to zero bead CM motion

protected:
	virtual const zString GetCommandType() const;

private:
	static const zString m_Type;	// Identifier used in control data file for command
	bool m_bZeroTotalMom;			// Flag showing whether to zero total bead momentum
};

#endif // !defined(AFX_CCWALLON_H__EBF46FF3_F98F_11D3_BF2D_004095086186__INCLUDED_)

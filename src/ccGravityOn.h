// ccGravityOn.h: interface for the ccGravityOn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCGRAVITYON_H__EBF46FF9_F98F_11D3_BF2D_004095086186__INCLUDED_)
#define AFX_CCGRAVITYON_H__EBF46FF9_F98F_11D3_BF2D_004095086186__INCLUDED_


#include "xxCommand.h"
#include "xxBase.h"	// Added by ClassView

class ccGravityOn : public xxCommand  
{
public:
	ccGravityOn(long executionTime);
	ccGravityOn(const ccGravityOn& oldCommand);
	virtual ~ccGravityOn();

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

#endif // !defined(AFX_CCGRAVITYON_H__EBF46FF9_F98F_11D3_BF2D_004095086186__INCLUDED_)

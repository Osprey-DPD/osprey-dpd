// ccAddPolymersInCell.h: interface for the ccAddPolymersInCell class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCADDPOLYMERSINCELL_H__3FE65EB9_0EB5_11D4_BF32_004095086186__INCLUDED_)
#define AFX_CCADDPOLYMERSINCELL_H__3FE65EB9_0EB5_11D4_BF32_004095086186__INCLUDED_


#include "ccSelection.h"

class ccAddPolymersInCell : public ccSelection  
{
public:
	ccAddPolymersInCell(long executionTime);
	ccAddPolymersInCell(const ccAddPolymersInCell& oldCommand);

	virtual ~ccAddPolymersInCell();

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

#endif // !defined(AFX_CCADDPOLYMERSINCELL_H__3FE65EB9_0EB5_11D4_BF32_004095086186__INCLUDED_)

// ccToggleBondPairStressContribution.h: interface for the ccToggleBondPairStressContribution class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCTOGGLEBONDPAIRSTRESSCONTRIBUTION_H__D9892EB0_000F_47A1_BFC0_1FFE25064E76__INCLUDED_)
#define AFX_CCTOGGLEBONDPAIRSTRESSCONTRIBUTION_H__D9892EB0_000F_47A1_BFC0_1FFE25064E76__INCLUDED_


#include "xxCommand.h"

class ccToggleBondPairStressContribution : public xxCommand  
{
public:
	ccToggleBondPairStressContribution(long executionTime);
	ccToggleBondPairStressContribution(const ccToggleBondPairStressContribution& oldCommand);

	virtual ~ccToggleBondPairStressContribution();


	// xxCommand functions
	
	zOutStream& put(zOutStream& os) const;
	zInStream&  get(zInStream& is);

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an xxCommand pointer,
	// respond to the SimBox's request to execute and return the name of the command.

	virtual bool Execute(long simTime, ISimCmd* const pISimCmd) const;

	static const zString GetType();	// Return the type of command

	virtual const xxCommand* GetCommand() const;

	virtual bool IsDataValid(const CInputData& riData) const;

	// Access functions to command's data

	// Local data access functions


protected:
	virtual const zString GetCommandType() const;

private:
	static const zString m_Type;	// Identifier used in control data file for command


};


#endif // !defined(AFX_CCTOGGLEBONDPAIRSTRESSCONTRIBUTION_H__D9892EB0_000F_47A1_BFC0_1FFE25064E76__INCLUDED_)

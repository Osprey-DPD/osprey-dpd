// ccToggleBondStressContribution.h: interface for the ccToggleBondStressContribution class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCTOGGLEBONDSTRESSCONTRIBUTION_H__FA4682AB_9B27_4B3C_8D20_DF1FAD8535D7__INCLUDED_)
#define AFX_CCTOGGLEBONDSTRESSCONTRIBUTION_H__FA4682AB_9B27_4B3C_8D20_DF1FAD8535D7__INCLUDED_


#include "xxCommand.h"

class ccToggleBondStressContribution : public xxCommand  
{
public:
	ccToggleBondStressContribution(long executionTime);
	ccToggleBondStressContribution(const ccToggleBondStressContribution& oldCommand);

	virtual ~ccToggleBondStressContribution();


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

#endif // !defined(AFX_CCTOGGLEBONDSTRESSCONTRIBUTION_H__FA4682AB_9B27_4B3C_8D20_DF1FAD8535D7__INCLUDED_)

// ccToggleBeadStressContribution.h: interface for the ccToggleBeadStressContribution class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCTOGGLEBEADSTRESSCONTRIBUTION_H__4441C917_AE4A_47CF_9B90_0F59104D90A5__INCLUDED_)
#define AFX_CCTOGGLEBEADSTRESSCONTRIBUTION_H__4441C917_AE4A_47CF_9B90_0F59104D90A5__INCLUDED_


#include "xxCommand.h"

class ccToggleBeadStressContribution : public xxCommand  
{
public:
	ccToggleBeadStressContribution(long executionTime);
	ccToggleBeadStressContribution(const ccToggleBeadStressContribution& oldCommand);

	virtual ~ccToggleBeadStressContribution();


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

#endif // !defined(AFX_CCTOGGLEBEADSTRESSCONTRIBUTION_H__4441C917_AE4A_47CF_9B90_0F59104D90A5__INCLUDED_)


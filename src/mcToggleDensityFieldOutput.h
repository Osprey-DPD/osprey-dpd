// mcToggleDensityFieldOutput.h: interface for the mcToggleDensityFieldOutput class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCTOGGLEDENSITYFIELDOUTPUT_H__7F288133_9B81_11D4_BF4C_004095086186__INCLUDED_)
#define AFX_MCTOGGLEDENSITYFIELDOUTPUT_H__7F288133_9B81_11D4_BF4C_004095086186__INCLUDED_


// Forward declarations

class ISimCmd;


#include "xxCommand.h"

class mcToggleDensityFieldOutput : public xxCommand  
{
public:
	mcToggleDensityFieldOutput(long executionTime);
	mcToggleDensityFieldOutput(const mcToggleDensityFieldOutput& oldCommand);

	virtual ~mcToggleDensityFieldOutput();

	zOutStream& put(zOutStream& os) const;
	zInStream&  get(zInStream& is);

	virtual bool Execute(long simTime, ISimCmd* const pISimCmd) const;

	static const zString GetType();	// Return the type of command

	virtual const xxCommand* GetCommand() const;

	virtual bool IsDataValid(const CInputData& riData) const;

	// Access function to command data


protected:
	virtual const zString GetCommandType() const;

private:
	static const zString m_Type;	// Identifier used in control data file for command

};

#endif // !defined(AFX_MCTOGGLEDENSITYFIELDOUTPUT_H__7F288133_9B81_11D4_BF4C_004095086186__INCLUDED_)

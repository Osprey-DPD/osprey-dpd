// ccUnchargeBeadType.h: interface for the ccUnchargeBeadType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCUNCHARGEBEADTYPE_H__C610C072_71A5_4537_8B05_FD4A2AD83F0C__INCLUDED_)
#define AFX_CCUNCHARGEBEADTYPE_H__C610C072_71A5_4537_8B05_FD4A2AD83F0C__INCLUDED_


#include "xxCommand.h"

class ccUnchargeBeadType : public xxCommand  
{
public:
	ccUnchargeBeadType(long executionTime);
	ccUnchargeBeadType(const ccUnchargeBeadType& oldCommand);

	virtual ~ccUnchargeBeadType();
	
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

	inline const zString GetName()		const {return m_Name;};

protected:
	virtual const zString GetCommandType() const;

private:
	static const zString m_Type;	// Identifier used in control data file for command

	zString m_Name;					// Bead type name
};

#endif // !defined(AFX_CCUNCHARGEBEADTYPE_H__C610C072_71A5_4537_8B05_FD4A2AD83F0C__INCLUDED_)

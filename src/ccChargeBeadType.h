// ccChargeBeadType.h: interface for the ccChargeBeadType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCCHARGEBEADTYPE_H__7E881820_AC0C_428D_BD20_9CF9EAD9F47F__INCLUDED_)
#define AFX_CCCHARGEBEADTYPE_H__7E881820_AC0C_428D_BD20_9CF9EAD9F47F__INCLUDED_


#include "xxCommand.h"

class ccChargeBeadType : public xxCommand  
{
public:
	ccChargeBeadType(long executionTime);
	ccChargeBeadType(const ccChargeBeadType& oldCommand);

	virtual ~ccChargeBeadType();
	
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
	inline		 double  GetStrength()	const {return m_Strength;}
	inline		 double	 GetRange()		const {return m_Range;}

protected:
	virtual const zString GetCommandType() const;

private:
	static const zString m_Type;	// Identifier used in control data file for command

	zString m_Name;					// Bead type name
	double  m_Strength;				// Strength of "        "        "
	double  m_Range;				// Range of screened charge interaction

};

#endif // !defined(AFX_CCCHARGEBEADTYPE_H__7E881820_AC0C_428D_BD20_9CF9EAD9F47F__INCLUDED_)

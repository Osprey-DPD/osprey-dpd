// mcSetCurrentStateDefaultFormat.h: interface for the mcSetCurrentStateDefaultFormat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSETCURRENTSTATEDEFAULTFORMAT_H__2D6DBD0A_DA88_4AC8_99B7_34E9FC272B07__INCLUDED_)
#define AFX_MCSETCURRENTSTATEDEFAULTFORMAT_H__2D6DBD0A_DA88_4AC8_99B7_34E9FC272B07__INCLUDED_


// Forward declarations

class ISimCmd;


#include "xxCommand.h"

class mcSetCurrentStateDefaultFormat : public xxCommand  
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcSetCurrentStateDefaultFormat(long executionTime);
	mcSetCurrentStateDefaultFormat(const mcSetCurrentStateDefaultFormat& oldCommand);

	mcSetCurrentStateDefaultFormat(long executionTime, const zString newFormat);

	virtual ~mcSetCurrentStateDefaultFormat();

	// ****************************************
	// Global functions, static member functions and variables


	// ****************************************
	// Public access functions
public:

	inline const zString GetFormat()	const {return m_Format;}

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	zOutStream& put(zOutStream& os) const;
	zInStream&  get(zInStream& is);

	virtual bool Execute(long simTime, ISimCmd* const pISimCmd) const;

	virtual const zString GetCommandType() const;

	static const zString GetType();	// Return the type of command

	virtual const xxCommand* GetCommand() const;

	virtual bool IsDataValid(const CInputData& riData) const;

	// ****************************************
	// Protected local functions


	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

	static const zString m_Type;	// Identifier used in control data file for command

	zString m_Format;				// Name of format to use as the default 

};

#endif // !defined(AFX_MCSETCURRENTSTATEDEFAULTFORMAT_H__2D6DBD0A_DA88_4AC8_99B7_34E9FC272B07__INCLUDED_)

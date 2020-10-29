// ccChargeBeadByType.h: interface for the ccChargeBeadByType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCCHARGEBEADBYTYPE_H__2A18299F_0006_43C4_A280_0DD0D6BF3541__INCLUDED_)
#define AFX_CCCHARGEBEADBYTYPE_H__2A18299F_0006_43C4_A280_0DD0D6BF3541__INCLUDED_


#include "xxCommand.h"

class ccChargeBeadByType : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ccChargeBeadByType(long executionTime);
	ccChargeBeadByType(const ccChargeBeadByType& oldCommand);

	virtual ~ccChargeBeadByType();
	
	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	// Return the type of command

private:

	static const zString m_Type;	// Identifier used in control data file for command

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	zOutStream& put(zOutStream& os) const;
	zInStream&  get(zInStream& is);

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an xxCommand pointer,
	// respond to the SimBox's request to execute and return the name of the command.

	virtual bool Execute(long simTime, ISimCmd* const pISimCmd) const;

	virtual const xxCommand* GetCommand() const;

	virtual bool IsDataValid(const CInputData& riData) const;

	// ****************************************
	// Public access functions
public:

	inline long   GetBeadType()	const {return m_BeadType;}
	inline double GetStrength()	const {return m_Strength;}
	inline double GetRange()	const {return m_Range;}

	// ****************************************
	// Protected local functions
protected:

	virtual const zString GetCommandType() const;

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

	long	m_BeadType;		// Bead's numeric identifer
	double  m_Strength;		// Strength of bead-bead interaction
	double  m_Range;		// Range of screened charge interaction

};

#endif // !defined(AFX_CCCHARGEBEADBYTYPE_H__2A18299F_0006_43C4_A280_0DD0D6BF3541__INCLUDED_)

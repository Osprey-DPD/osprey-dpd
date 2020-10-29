// ccUnchargeBeadByType.h: interface for the ccUnchargeBeadByType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCUNCHARGEBEADBYTYPE_H__9B2A0DB2_3626_4D97_ABD8_A1B481DBBEEC__INCLUDED_)
#define AFX_CCUNCHARGEBEADBYTYPE_H__9B2A0DB2_3626_4D97_ABD8_A1B481DBBEEC__INCLUDED_


#include "xxCommand.h"

class ccUnchargeBeadByType : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ccUnchargeBeadByType(long executionTime);
	ccUnchargeBeadByType(const ccUnchargeBeadByType& oldCommand);

	virtual ~ccUnchargeBeadByType();
	
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

};

#endif // !defined(AFX_CCUNCHARGEBEADBYTYPE_H__9B2A0DB2_3626_4D97_ABD8_A1B481DBBEEC__INCLUDED_)

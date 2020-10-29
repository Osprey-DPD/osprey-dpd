// ccSetDPDBeadConsIntByType.h: interface for the ccSetDPDBeadConsIntByType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSETDPDBEADCONSINTBYTYPE_H__935C037D_45A2_4C18_8AA2_9139BB3BBC16__INCLUDED_)
#define AFX_CCSETDPDBEADCONSINTBYTYPE_H__935C037D_45A2_4C18_8AA2_9139BB3BBC16__INCLUDED_


#include "xxCommand.h"

class ccSetDPDBeadConsIntByType : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ccSetDPDBeadConsIntByType(long executionTime);
	ccSetDPDBeadConsIntByType(const ccSetDPDBeadConsIntByType& oldCommand);

	ccSetDPDBeadConsIntByType(long executionTime, long firstType, long secondType, double consInt);

	virtual ~ccSetDPDBeadConsIntByType();
	
	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	 // Return the type of command
    static long  GetArgumentTotal(); // Return the number of arguments 

private:

	static const zString m_Type;	// Identifier used in control data file for command
    static long  m_ArgumentTotal;   // Number of arguments required not including the execution time

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

    virtual long GetCommandArgumentTotal() const {return m_ArgumentTotal;}

	virtual bool Pack(const tguArgumentSequence& vArguments);


	// ****************************************
	// Public access functions
public:

	inline long		GetFirstType()	const {return m_FirstType;}
	inline long		GetSecondType()	const {return m_SecondType;}
	inline double	GetConsInt()	const {return m_ConsInt;}

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

	long m_FirstType;			// Numeric type of primary CBead type
	long m_SecondType;			// Numeric type of secondary CBead type

	double	m_ConsInt;			// New value of conservative interaction parameter
};

#endif // !defined(AFX_CCSETDPDBEADCONSINTBYTYPE_H__935C037D_45A2_4C18_8AA2_9139BB3BBC16__INCLUDED_)

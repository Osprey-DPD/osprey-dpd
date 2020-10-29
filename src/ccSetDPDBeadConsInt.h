// ccSetDPDBeadConsInt.h: interface for the ccSetDPDBeadConsInt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSETDPDBEADCONSINT_H__A435B333_0282_4194_9C3B_5C94495BA4A1__INCLUDED_)
#define AFX_CCSETDPDBEADCONSINT_H__A435B333_0282_4194_9C3B_5C94495BA4A1__INCLUDED_


#include "xxCommand.h"

class ccSetDPDBeadConsInt : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ccSetDPDBeadConsInt(long executionTime);
	ccSetDPDBeadConsInt(const ccSetDPDBeadConsInt& oldCommand);

	virtual ~ccSetDPDBeadConsInt();
	
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

	inline const zString GetFirstName()		const {return m_FirstName;}
	inline const zString GetSecondName()	const {return m_SecondName;}
	inline double		 GetConsInt()		const {return m_ConsInt;}

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

	zString m_FirstName;			// Name of primary CBead type
	zString m_SecondName;			// Name of secondary CBead type

	double	m_ConsInt;				// New value of conservative interaction parameter
};

#endif // !defined(AFX_CCSETDPDBEADCONSINT_H__A435B333_0282_4194_9C3B_5C94495BA4A1__INCLUDED_)

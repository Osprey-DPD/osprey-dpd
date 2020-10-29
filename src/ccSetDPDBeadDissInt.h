// ccSetDPDBeadDissInt.h: interface for the ccSetDPDBeadDissInt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSETDPDBEADDISSINT_H__AA12CDC7_CDB0_4577_AD92_B490E938A3D8__INCLUDED_)
#define AFX_CCSETDPDBEADDISSINT_H__AA12CDC7_CDB0_4577_AD92_B490E938A3D8__INCLUDED_


#include "xxCommand.h"

class ccSetDPDBeadDissInt : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ccSetDPDBeadDissInt(long executionTime);
	ccSetDPDBeadDissInt(const ccSetDPDBeadDissInt& oldCommand);

	virtual ~ccSetDPDBeadDissInt();
	
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
	inline double		 GetDissInt()		const {return m_DissInt;}

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

	double	m_DissInt;				// New value of dissipative interaction parameter
};

#endif // !defined(AFX_CCSETDPDBEADDISSINT_H__AA12CDC7_CDB0_4577_AD92_B490E938A3D8__INCLUDED_)

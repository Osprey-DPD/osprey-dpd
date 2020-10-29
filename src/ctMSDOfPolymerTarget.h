// ctMSDOfPolymerTarget.h: interface for the ctMSDOfPolymerTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTMSDOFPOLYMERTARGET_H__40A5AD7D_7F71_4F3E_AFFA_0AB27D622D14__INCLUDED_)
#define AFX_CTMSDOFPOLYMERTARGET_H__40A5AD7D_7F71_4F3E_AFFA_0AB27D622D14__INCLUDED_


#include "ctApplyCommand.h"

class ctMSDOfPolymerTarget : public ctApplyCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ctMSDOfPolymerTarget(long executionTime);
	ctMSDOfPolymerTarget(const ctMSDOfPolymerTarget& oldCommand);

	ctMSDOfPolymerTarget(long executionTime, const zString targetLabel,
						   const zString decLabel, long start, long end);

	virtual ~ctMSDOfPolymerTarget();
	
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


	// ****************************************
	// Public access functions
public:

	inline const zString GetDecLabel()		const {return m_DecLabel;}
	inline long   		 GetStartTime()		const {return m_Start;}
	inline long 		 GetEndTime()		const {return m_End;}

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

	zString		m_DecLabel;		// Label for this decorator object
	long		m_Start;		// Start time for the calculation
	long		m_End;			// End time for calculation

};

#endif // !defined(AFX_CTMSDOFPOLYMERTARGET_H__40A5AD7D_7F71_4F3E_AFFA_0AB27D622D14__INCLUDED_)

// ctReflectingBarrierOnTarget.h: interface for the ctReflectingBarrierOnTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTREFLECTINGBARRIERONTARGET_H__d7b67839_7608_4cb2_8439_9e6bb1f4d336__INCLUDED_)
#define AFX_CTREFLECTINGBARRIERONTARGET_H__d7b67839_7608_4cb2_8439_9e6bb1f4d336__INCLUDED_


#include "ctApplyCommand.h"

class ctReflectingBarrierOnTarget : public ctApplyCommand
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ctReflectingBarrierOnTarget(long executionTime);
	ctReflectingBarrierOnTarget(const ctReflectingBarrierOnTarget& oldCommand);

	ctReflectingBarrierOnTarget(long executionTime, const zString targetLabel,
							const zString decLabel, long xn, long yn, long zn,
							double mag);

	virtual ~ctReflectingBarrierOnTarget();
	
	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	 // Return the type of command
    static long  GetArgumentTotal(); // Return the number of arguments 

private:

	static const zString m_Type;	// Identifier used in control data file for command
    static long  m_ArgumentTotal;   // Number of arguments required not including the execution time


	// ****************************************
	// Public access functions
public:

	inline const zString GetDecLabel()	const {return m_DecLabel;}
	inline long          GetXNormal()	const {return m_XN;}
	inline long          GetYNormal()	const {return m_YN;}
	inline long          GetZNormal()	const {return m_ZN;}
	inline double		 GetMagnitude() const {return m_Magnitude;}

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

	zString		m_DecLabel;		// Label for the decorator object
	long        m_XN;
	long        m_YN;			// Barrier normal direction
	long        m_ZN;
	double		m_Magnitude;	// Fractional position along axis

};

#endif // !defined(AFX_CTREFLECTINGBARRIERONTARGET_H__d7b67839_7608_4cb2_8439_9e6bb1f4d336__INCLUDED_)

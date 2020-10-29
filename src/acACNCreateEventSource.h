// acACNCreateEventSource.h: interface for the acACNCreateEventSource class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACACNCREATEEVENTSOURCE_H__39C9F9D8_2C1C_4EAC_9478_27671DE915CD__INCLUDED_)
#define AFX_ACACNCREATEEVENTSOURCE_H__39C9F9D8_2C1C_4EAC_9478_27671DE915CD__INCLUDED_


#include "xxCommand.h" 

class acACNCreateEventSource : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: protected constructor defined below
public:


	virtual ~acACNCreateEventSource();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	zOutStream& put(zOutStream& os) const;
	zInStream&  get(zInStream& is);

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an xxCommand pointer,
	// respond to the SimBox's request to execute and return the name of the command.

	virtual bool Execute(long simTime, ISimCmd* const pISimCmd) const = 0;

	virtual const zString GetCommandType() const = 0;

	virtual const xxCommand* GetCommand()  const = 0;

	// The following function is not a PVF in this class as it is used by 
	// derived classes to check their common data stored here

	virtual bool IsDataValid(const CInputData& riData) const;

    
	// ****************************************
	// Public access functions
public:

	inline const zString GetACNTargetId()   const {return m_ACNTargetId;}
	inline const zString GetEventSourceId() const {return m_SourceId;}


	// ****************************************
	// Protected local functions
protected:

 	// Protected constructor ensures that only subclasses can be instantiated

    acACNCreateEventSource(long executionTime);
	acACNCreateEventSource(const acACNCreateEventSource& oldCommand);


	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

	zString m_ACNTargetId; // Name of target that wraps the ACN
	zString m_SourceId;    // Name of the event source decorator instance that wraps the ACNtarget


};

#endif // !defined(AFX_ACACNCREATEEVENTSOURCE_H__39C9F9D8_2C1C_4EAC_9478_27671DE915CD__INCLUDED_)

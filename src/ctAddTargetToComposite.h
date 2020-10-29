// ctAddTargetToComposite.h: interface for the ctAddTargetToComposite class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTADDTARGETTOCOMPOSITE_H__A97722A9_B166_4B89_B5F0_80888EC6895B__INCLUDED_)
#define AFX_CTADDTARGETTOCOMPOSITE_H__A97722A9_B166_4B89_B5F0_80888EC6895B__INCLUDED_


#include "xxCommand.h"

class ctAddTargetToComposite : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ctAddTargetToComposite(long executionTime);
	ctAddTargetToComposite(const ctAddTargetToComposite& oldCommand);

	ctAddTargetToComposite(long executionTime, const zString destination, const zString source);

	virtual ~ctAddTargetToComposite();
	
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

	inline const zString GetDestinationTargetLabel() const {return m_Destination;}
	inline const zString GetSourceTargetLabel()	     const {return m_Source;}


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

	zString		m_Destination;	// Name of composite target
	zString		m_Source;		// Name of command target to add to composite target

};

#endif // !defined(AFX_CTADDTARGETTOCOMPOSITE_H__A97722A9_B166_4B89_B5F0_80888EC6895B__INCLUDED_)

// ctRemoveTargetFromComposite.h: interface for the ctRemoveTargetFromComposite class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTREMOVETARGETFROMCOMPOSITE_H__E01B2DDF_14F0_4E45_BD12_7BE6535F6582__INCLUDED_)
#define AFX_CTREMOVETARGETFROMCOMPOSITE_H__E01B2DDF_14F0_4E45_BD12_7BE6535F6582__INCLUDED_


#include "xxCommand.h"

class ctRemoveTargetFromComposite : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ctRemoveTargetFromComposite(long executionTime);
	ctRemoveTargetFromComposite(const ctRemoveTargetFromComposite& oldCommand);

	ctRemoveTargetFromComposite(long executionTime, const zString destination, const zString source);

	virtual ~ctRemoveTargetFromComposite();
	
	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	// Return the type of command
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
	inline const zString GetSourceTargetLabel()	    const {return m_Source;}


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

	zString		m_Destination;		// Name of composite target
	zString		m_Source;			// Name of command target to remove from composite target

};

#endif // !defined(AFX_CTREMOVETARGETFROMCOMPOSITE_H__E01B2DDF_14F0_4E45_BD12_7BE6535F6582__INCLUDED_)

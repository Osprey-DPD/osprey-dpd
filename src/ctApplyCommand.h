// ctApplyCommand.h: interface for the ctApplyCommand class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTAPPLYCOMMAND_H__FE2FA94E_CF46_429F_8823_FCBC61D29E12__INCLUDED_)
#define AFX_CTAPPLYCOMMAND_H__FE2FA94E_CF46_429F_8823_FCBC61D29E12__INCLUDED_


#include "xxCommand.h"

class ctApplyCommand : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: protected constructor declared below
public:

	virtual ~ctApplyCommand();

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

    virtual bool Pack(const tguArgumentSequence& vArguments);

	// ****************************************
	// Public access functions
public:

	inline const zString GetTargetLabel() const {return m_TargetLabel;}


	// ****************************************
	// Protected local functions
protected:

	// Protected constructor ensures that only subclasses can be instantiated

	ctApplyCommand(long executionTime);
	ctApplyCommand(const ctApplyCommand& oldCommand);

	ctApplyCommand(long executionTime, const zString target);


	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

	zString	m_TargetLabel;	// Name of command target to receive command

};

#endif // !defined(AFX_CTAPPLYCOMMAND_H__FE2FA94E_CF46_429F_8823_FCBC61D29E12__INCLUDED_)

// pcBLMVesicleFusionSetMaxDistalTouch.h: interface for the pcBLMVesicleFusionSetMaxDistalTouch class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PCBLMVESICLEFUSIONSETMAXDISTALTOUCH_H__62988760_862C_4701_A51A_D2CA652CB452__INCLUDED_)
#define AFX_PCBLMVESICLEFUSIONSETMAXDISTALTOUCH_H__62988760_862C_4701_A51A_D2CA652CB452__INCLUDED_


#include "xxCommand.h"

class pcBLMVesicleFusionSetMaxDistalTouch : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	pcBLMVesicleFusionSetMaxDistalTouch(long executionTime);
	pcBLMVesicleFusionSetMaxDistalTouch(const pcBLMVesicleFusionSetMaxDistalTouch& oldCommand);

	virtual ~pcBLMVesicleFusionSetMaxDistalTouch();
	
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

	inline const     zString GetPid() const {return m_pid;}
	inline double GetMaxDistalTouch() const {return m_MaxDistalTouch;}

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

	zString	 m_pid;				// Process id that uniquely identifies each process
	double	 m_MaxDistalTouch;	// New distance at which outer leaflets "touch"

};

#endif // !defined(AFX_PCBLMVESICLEFUSIONSETMAXDISTALTOUCH_H__62988760_862C_4701_A51A_D2CA652CB452__INCLUDED_)

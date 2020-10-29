// pcBLMVesicleFusionSetMinPoreSize.h: interface for the pcBLMVesicleFusionSetMinPoreSize class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PCBLMVESICLEFUSIONSETMINPORESIZE_H__E5A1C194_358D_4C8E_910F_4FB2A595CA8A__INCLUDED_)
#define AFX_PCBLMVESICLEFUSIONSETMINPORESIZE_H__E5A1C194_358D_4C8E_910F_4FB2A595CA8A__INCLUDED_


#include "xxCommand.h"

class pcBLMVesicleFusionSetMinPoreSize : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	pcBLMVesicleFusionSetMinPoreSize(long executionTime);
	pcBLMVesicleFusionSetMinPoreSize(const pcBLMVesicleFusionSetMinPoreSize& oldCommand);

	virtual ~pcBLMVesicleFusionSetMinPoreSize();
	
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

	inline const zString GetPid() const {return m_pid;}
	inline long  GetMinPoreSize() const {return m_MinPoreSize;}

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

	zString	 m_pid;			// Process id that uniquely identifies each process
	long	 m_MinPoreSize;	// Minimum pore size for premature rupture

};

#endif // !defined(AFX_PCBLMVESICLEFUSIONSETMINPORESIZE_H__E5A1C194_358D_4C8E_910F_4FB2A595CA8A__INCLUDED_)

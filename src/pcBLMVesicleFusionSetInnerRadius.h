// pcBLMVesicleFusionSetInnerRadius.h: interface for the pcBLMVesicleFusionSetInnerRadius class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PCBLMVESICLEFUSIONSETINNERRADIUS_H__6EF5FC8F_74EC_426C_9ECB_73DCCCB125A1__INCLUDED_)
#define AFX_PCBLMVESICLEFUSIONSETINNERRADIUS_H__6EF5FC8F_74EC_426C_9ECB_73DCCCB125A1__INCLUDED_


#include "xxCommand.h"

class pcBLMVesicleFusionSetInnerRadius : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	pcBLMVesicleFusionSetInnerRadius(long executionTime);
	pcBLMVesicleFusionSetInnerRadius(const pcBLMVesicleFusionSetInnerRadius& oldCommand);

	virtual ~pcBLMVesicleFusionSetInnerRadius();
	
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

	inline const  zString GetPid() const {return m_pid;}
	inline double GetInnerRadius() const {return m_InnerRadius;}

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
	double	 m_InnerRadius;	// New inner radius for the stretching force cylinder

};

#endif // !defined(AFX_PCBLMVESICLEFUSIONSETINNERRADIUS_H__6EF5FC8F_74EC_426C_9ECB_73DCCCB125A1__INCLUDED_)

// acCreateBondBindsForwardConeToPolymerHeadEventSource.h: interface for the acCreateBondBindsForwardConeToPolymerHeadEventSource class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACCREATEBONDBINDSFORWARDCONETOPOLYMERHEADEVENTSOURCE_H__DE715B89_6B2F_41BA_B135_11B3F1D25E8A__INCLUDED_)
#define AFX_ACCREATEBONDBINDSFORWARDCONETOPOLYMERHEADEVENTSOURCE_H__DE715B89_6B2F_41BA_B135_11B3F1D25E8A__INCLUDED_


#include "acACNCreateEventSource.h" 

class acCreateBondBindsForwardConeToPolymerHeadEventSource : public acACNCreateEventSource  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	acCreateBondBindsForwardConeToPolymerHeadEventSource(long executionTime);
	acCreateBondBindsForwardConeToPolymerHeadEventSource(const acCreateBondBindsForwardConeToPolymerHeadEventSource& oldCommand);

	virtual ~acCreateBondBindsForwardConeToPolymerHeadEventSource();
	
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



};

#endif // !defined(AFX_ACCREATEBONDBINDSFORWARDCONETOPOLYMERHEADEVENTSOURCE_H__DE715B89_6B2F_41BA_B135_11B3F1D25E8A__INCLUDED_)

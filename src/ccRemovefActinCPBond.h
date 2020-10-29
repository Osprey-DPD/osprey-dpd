// ccRemovefActinCPBond.h: interface for the ccRemovefActinCPBond class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCREMOVEFACTINCPBOND_H__16C7821D_8B61_49BB_871F_FB2CA7D06F5C__INCLUDED_)
#define AFX_CCREMOVEFACTINCPBOND_H__16C7821D_8B61_49BB_871F_FB2CA7D06F5C__INCLUDED_


#include "xxCommand.h"

class ccRemovefActinCPBond : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ccRemovefActinCPBond(long executionTime);
	ccRemovefActinCPBond(const ccRemovefActinCPBond& oldCommand);

	virtual ~ccRemovefActinCPBond();
	
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

	inline const zString GetHeadMonomerName()	const {return m_HeadMonomerName;}
	inline const zString GetTailMonomerName()	const {return m_TailMonomerName;}

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

	zString m_HeadMonomerName;		// Name of polymer type bound to this active bond type
	zString m_TailMonomerName;		// Name of polymer type wrapped by this active bond type

};

#endif // !defined(AFX_CCREMOVEFACTINCPBOND_H__16C7821D_8B61_49BB_871F_FB2CA7D06F5C__INCLUDED_)

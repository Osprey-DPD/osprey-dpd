// ccRemoveCNTCellsOutsideRadius.h: interface for the ccRemoveCNTCellsOutsideRadius class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCREMOVECNTCELLSOUTSIDERADIUS_H__CCFC4667_F6D9_424B_B37A_2E67FBBC543A__INCLUDED_)
#define AFX_CCREMOVECNTCELLSOUTSIDERADIUS_H__CCFC4667_F6D9_424B_B37A_2E67FBBC543A__INCLUDED_


#include "xxCommand.h"

class ccRemoveCNTCellsOutsideRadius : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: protected constructor declared below
public:

	ccRemoveCNTCellsOutsideRadius(long executionTime);
	ccRemoveCNTCellsOutsideRadius(const ccRemoveCNTCellsOutsideRadius& oldCommand);

	virtual ~ccRemoveCNTCellsOutsideRadius();

	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	// Return the type of command

private:

	static const zString m_Type;	// Identifier used in control data file for command
	// ****************************************
	// Public access functions
public:

	inline double GetRadius() const {return m_Radius;}

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an xxCommand pointer,
	// respond to the SimBox's request to execute and return the name of the command,
	// and validate their data given the CInputData object to get access to the
	// SimBox definitions.

	virtual zOutStream& put(zOutStream& os) const;
	virtual zInStream&  get(zInStream& is);

	virtual bool Execute(long simTime, ISimCmd* const pISimCmd) const;

	virtual const zString GetCommandType() const;

	virtual const xxCommand* GetCommand()  const;

	virtual bool IsDataValid(const CInputData& riData) const;

	// ****************************************
	// Protected local functions
protected:

	
	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

	double	m_Radius;		// in units of bead diameter

};

#endif // !defined(AFX_CCREMOVECNTCELLSOUTSIDERADIUS_H__CCFC4667_F6D9_424B_B37A_2E67FBBC543A__INCLUDED_)

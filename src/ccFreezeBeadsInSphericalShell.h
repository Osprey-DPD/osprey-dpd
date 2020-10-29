// ccFreezeBeadsInSphericalShell.h: interface for the ccFreezeBeadsInSphericalShell class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCFREEZEBEADSINSPHERICALSHELL_H__689B2522_4A61_4E61_BFBE_80C2695FDD4C__INCLUDED_)
#define AFX_CCFREEZEBEADSINSPHERICALSHELL_H__689B2522_4A61_4E61_BFBE_80C2695FDD4C__INCLUDED_


#include "xxCommand.h"

class ccFreezeBeadsInSphericalShell : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: protected constructor declared below
public:

	ccFreezeBeadsInSphericalShell(long executionTime);
	ccFreezeBeadsInSphericalShell(const ccFreezeBeadsInSphericalShell& oldCommand);

	virtual ~ccFreezeBeadsInSphericalShell();

	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	// Return the type of command

private:

	static const zString m_Type;	// Identifier used in control data file for command

	// ****************************************
	// Public access functions
public:

	inline double GetInnerRadius() const {return m_InnerRadius;}
	inline double GetThickness()   const {return m_Thickness;}

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

	double	m_InnerRadius;		// in units of bead diameter
	double	m_Thickness;		// in units of bead diameter

};


#endif // !defined(AFX_CCFREEZEBEADSINSPHERICALSHELL_H__689B2522_4A61_4E61_BFBE_80C2695FDD4C__INCLUDED_)



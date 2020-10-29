// mcTogglePolymerDisplay.h: interface for the mcTogglePolymerDisplay class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCTOGGLEPOLYMERDISPLAY_H__77A180EB_D102_4CDE_934C_CEE1CD38E99C__INCLUDED_)
#define AFX_MCTOGGLEPOLYMERDISPLAY_H__77A180EB_D102_4CDE_934C_CEE1CD38E99C__INCLUDED_


// Forward declarations

class ISimCmd;


#include "xxCommand.h"

class mcTogglePolymerDisplay : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	mcTogglePolymerDisplay(long executionTime);
	mcTogglePolymerDisplay(const mcTogglePolymerDisplay& oldCommand);

	mcTogglePolymerDisplay(long executionTime, const zString name);

	virtual ~mcTogglePolymerDisplay();
	
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

	virtual bool Execute(long simTime, ISimCmd* const pISimCmd) const;

	virtual const xxCommand* GetCommand() const;

	virtual bool IsDataValid(const CInputData& riData) const;


	// ****************************************
	// Public access functions
public:

	inline const zString GetPolymerName() const {return m_PolymerName;}

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

	zString m_PolymerName;		// Polymer whose beads are to be toggled to/from display
};

#endif // !defined(AFX_MCTOGGLEPOLYMERDISPLAY_H__77A180EB_D102_4CDE_934C_CEE1CD38E99C__INCLUDED_)

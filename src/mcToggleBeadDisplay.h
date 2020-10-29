// mcToggleBeadDisplay.h: interface for the mcToggleBeadDisplay class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCTOGGLEBEADDISPLAY_H__7DEE8084_57D7_11D4_BF42_004095086186__INCLUDED_)
#define AFX_MCTOGGLEBEADDISPLAY_H__7DEE8084_57D7_11D4_BF42_004095086186__INCLUDED_


// Forward declarations

class ISimCmd;


#include "xxCommand.h"

class mcToggleBeadDisplay : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	mcToggleBeadDisplay(long executionTime);
	mcToggleBeadDisplay(const mcToggleBeadDisplay& oldCommand);

	mcToggleBeadDisplay(long executionTime, const zString name);

	virtual ~mcToggleBeadDisplay();
	
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

	inline const zString GetBeadName() const {return m_BeadName;}

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

	zString m_BeadName;		// Bead to be toggled to/from display
};

#endif // !defined(AFX_MCTOGGLEBEADDISPLAY_H__7DEE8084_57D7_11D4_BF42_004095086186__INCLUDED_)




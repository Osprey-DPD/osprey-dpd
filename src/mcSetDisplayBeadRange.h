// mcSetDisplayBeadRange.h: interface for the mcSetDisplayBeadRange class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSETDISPLAYBEADRANGE_H__C096A840_A8FA_4E98_8BD7_4E0EE228C15F__INCLUDED_)
#define AFX_MCSETDISPLAYBEADRANGE_H__C096A840_A8FA_4E98_8BD7_4E0EE228C15F__INCLUDED_


// Forward declarations

class ISimCmd;


#include "xxCommand.h"

class mcSetDisplayBeadRange : public xxCommand 
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	mcSetDisplayBeadRange(long executionTime);
	mcSetDisplayBeadRange(const mcSetDisplayBeadRange& oldCommand);

	mcSetDisplayBeadRange(long executionTime, const zString axis, double min, double max);

	virtual ~mcSetDisplayBeadRange();
	
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

	inline const zString GetAxis()  const {return m_Axis;}
	inline double GetMinFraction()	const {return m_MinFraction;}
	inline double GetMaxFraction()	const {return m_MaxFraction;}

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

	zString m_Axis;			// Coordinate axis label: X, Y, Z
	double  m_MinFraction;	// Minimum coordinate as fraction of SimBox size
	double  m_MaxFraction;	// Maximum     "             "        "
};

#endif // !defined(AFX_MCSETDISPLAYBEADRANGE_H__C096A840_A8FA_4E98_8BD7_4E0EE228C15F__INCLUDED_)

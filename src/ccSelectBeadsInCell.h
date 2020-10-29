// ccSelectBeadsInCell.h: interface for the ccSelectBeadsInCell class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSELECTBEADSINCELL_H__1A88D2F4_0D4A_11D4_BF31_004095086186__INCLUDED_)
#define AFX_CCSELECTBEADSINCELL_H__1A88D2F4_0D4A_11D4_BF31_004095086186__INCLUDED_


#include "ccSelection.h"

class ccSelectBeadsInCell : public ccSelection  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ccSelectBeadsInCell(long executionTime);
	ccSelectBeadsInCell(const ccSelectBeadsInCell& oldCommand);

	virtual ~ccSelectBeadsInCell();

	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	// Return the type of command

	friend bool operator ==(const ccSelectBeadsInCell &a, const ccSelectBeadsInCell &b);

private:

	static const zString m_Type;	// Identifier used in control data file for command

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	zOutStream& put(zOutStream& os) const;
	zInStream&  get(zInStream& is);

	virtual bool Execute(long simTime, ISimCmd* const pISimCmd) const;

	virtual const xxCommand* GetCommand() const;

	bool IsDataValid(const CInputData& riData) const;

	// ****************************************
	// Public access functions
public:

	inline long GetXCellIndex() const {return m_XCellIndex;}
	inline long GetYCellIndex() const {return m_YCellIndex;}
	inline long GetZCellIndex() const {return m_ZCellIndex;}

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

	long m_XCellIndex;
	long m_YCellIndex;				// Coordinates of cell
	long m_ZCellIndex;
};

#endif // !defined(AFX_CCSELECTBEADSINCELL_H__1A88D2F4_0D4A_11D4_BF31_004095086186__INCLUDED_)

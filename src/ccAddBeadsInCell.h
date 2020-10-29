// ccAddBeadsInCell.h: interface for the ccAddBeadsInCell class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCADDBEADSINCELL_H__3FE65EB6_0EB5_11D4_BF32_004095086186__INCLUDED_)
#define AFX_CCADDBEADSINCELL_H__3FE65EB6_0EB5_11D4_BF32_004095086186__INCLUDED_


#include "ccSelection.h"

class ccAddBeadsInCell : public ccSelection 
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ccAddBeadsInCell(long executionTime);
	ccAddBeadsInCell(const ccAddBeadsInCell& oldCommand);

	virtual ~ccAddBeadsInCell();

	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	// Return the type of command

	friend bool operator ==(const ccAddBeadsInCell &a, const ccAddBeadsInCell &b);
	
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

#endif // !defined(AFX_CCADDBEADSINCELL_H__3FE65EB6_0EB5_11D4_BF32_004095086186__INCLUDED_)

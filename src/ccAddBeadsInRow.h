// ccAddBeadsInRow.h: interface for the ccAddBeadsInRow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCADDBEADSINROW_H__3FE65EB5_0EB5_11D4_BF32_004095086186__INCLUDED_)
#define AFX_CCADDBEADSINROW_H__3FE65EB5_0EB5_11D4_BF32_004095086186__INCLUDED_


#include "ccSelection.h"

class ccAddBeadsInRow : public ccSelection 
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ccAddBeadsInRow(long executionTime);
	ccAddBeadsInRow(const ccAddBeadsInRow& oldCommand);

	virtual ~ccAddBeadsInRow();

	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	// Return the type of command

	friend bool operator ==(const ccAddBeadsInRow &a, const ccAddBeadsInRow &b);
	
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

	inline long GetSliceIndex() const {return m_SliceIndex;}
	inline long GetRowIndex()   const {return m_RowIndex;}

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

	long m_SliceIndex;				// Identifier for slice containing the row
	long m_RowIndex;				// Identifier for row location in the slice
};

#endif // !defined(AFX_CCADDBEADSINROW_H__3FE65EB5_0EB5_11D4_BF32_004095086186__INCLUDED_)

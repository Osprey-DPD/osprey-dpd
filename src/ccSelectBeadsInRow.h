// ccSelectBeadsInRow.h: interface for the ccSelectBeadsInRow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSELECTBEADSINROW_H__1A88D2F3_0D4A_11D4_BF31_004095086186__INCLUDED_)
#define AFX_CCSELECTBEADSINROW_H__1A88D2F3_0D4A_11D4_BF31_004095086186__INCLUDED_


#include "ccSelection.h"

class ccSelectBeadsInRow : public ccSelection  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ccSelectBeadsInRow(long executionTime);
	ccSelectBeadsInRow(const ccSelectBeadsInRow& oldCommand);

	virtual ~ccSelectBeadsInRow();

	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	// Return the type of command

	friend bool operator ==(const ccSelectBeadsInRow &a, const ccSelectBeadsInRow &b);

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

#endif // !defined(AFX_CCSELECTBEADSINROW_H__1A88D2F3_0D4A_11D4_BF31_004095086186__INCLUDED_)

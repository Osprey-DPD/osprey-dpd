// ccSelection.h: interface for the ccSelection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSELECTION_H__0FF39E48_0723_11D4_BF30_004095086186__INCLUDED_)
#define AFX_CCSELECTION_H__0FF39E48_0723_11D4_BF30_004095086186__INCLUDED_


#include "xxCommand.h"

class ccSelection : public xxCommand  
{
	// ****************************************
	// Construction/Destruction
public:

	ccSelection(long executionTime);
	ccSelection(const ccSelection& oldCommand);

	virtual ~ccSelection();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend bool operator ==(const ccSelection &a, const ccSelection &b);

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:
		
	zOutStream& put(zOutStream& os) const;
	zInStream&  get(zInStream& is);

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an xxCommand pointer,
	// respond to the SimBox's request to execute and return the name of the command.
	// We don't provide them here because we don't want ccSelection to be instantiated.

	virtual bool Execute(long simTime, ISimCmd* const pISimCmd) const = 0;

	virtual const zString GetCommandType() const = 0;

	virtual const xxCommand* GetCommand()  const = 0;

	// The following function is not a PVF as it is used by its derived classes 
	// to check their common data

	virtual bool IsDataValid(const CInputData& riData) const;

	// ****************************************
	// Public access functions
public:

	inline const zString GetLabel() const {return m_Label;};	// Label for aggregate
	inline		 long	 GetX()     const {return m_X;}
	inline		 long    GetY()     const {return m_Y;}			// Region
	inline		 long    GetZ()     const {return m_Z;}
	inline const StringSequence GetNames() const {return m_Names;}	// Bead/bond/polymer names

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

	zString m_Label;			// Label for the selection to identify it in later commands
	long m_X;					// X component of normal for slice or row selections
	long m_Y;					// Y     "
	long m_Z;					// Z     "
	StringSequence m_Names;		// Set of bead, bond or polymer names in selection

};

#endif // !defined(AFX_CCSELECTION_H__0FF39E48_0723_11D4_BF30_004095086186__INCLUDED_)

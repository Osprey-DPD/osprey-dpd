// CommandTargetACNFreeBonds.h: interface for the CCommandTargetACNFreeBonds class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMANDTARGETACNFREEBONDS_H__027C7E2B_AA02_42E3_ABAE_8F4C3677D795__INCLUDED_)
#define AFX_COMMANDTARGETACNFREEBONDS_H__027C7E2B_AA02_42E3_ABAE_8F4C3677D795__INCLUDED_


// Forward declarations

class aeActiveCellNetwork;



#include "CommandTargetNode.h"

class CCommandTargetACNFreeBonds : public CCommandTargetNode
{
	// ****************************************
	// Construction/Destruction
public:

	CCommandTargetACNFreeBonds(const zString label);
	CCommandTargetACNFreeBonds(const zString label, aeActiveCellNetwork* pACN);

	virtual ~CCommandTargetACNFreeBonds();


	// ****************************************
	// Global functions, static member functions and variables
public:

    static const zString GetType();		// return the target's type

private:

	static const zString m_Type;

	// ****************************************
	// PVFs that must be implemented by all instantiated derived classes 
public:

    const zString GetTargetType() const;    // return the target's type

    // Implementation of the ISerialiseInclusiveRestartState interface
    // to allow this class to read/write data that can be modified
    // for restarts.

    virtual zInStream& Read(zInStream& is);
    virtual zOutStream& Write(zOutStream& is) const;

	// ****************************************
	// Public access functions
public:

	// IQueryBeadTarget interface

	virtual bool IsBeadTypeInTarget(long type)  const;
	virtual long GetBeadTotalForType(long type) const;
	virtual long GetBeadTotal() const;
	virtual BeadVector GetBeads() const;

	// IQueryPolymerTarget interface

	virtual bool IsPolymerTypeInTarget(long type)	const;
	virtual long GetPolymerTotalForType(long type)	const;
	virtual long GetPolymerTotal()					const;
	virtual long GetOriginalPolymerType()			const;
	virtual PolymerVector GetPolymers()				const;


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

	aeActiveCellNetwork*  const m_pACN;	// Pointer to ACN whose polymers are in the target
	long m_OriginalType;				// Type of polymers in target (unchangeable)
};

#endif // !defined(AFX_COMMANDTARGETACNFREEBONDS_H__027C7E2B_AA02_42E3_ABAE_8F4C3677D795__INCLUDED_)

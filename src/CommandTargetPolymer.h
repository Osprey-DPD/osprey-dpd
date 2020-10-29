// CommandTargetPolymer.h: interface for the CCommandTargetPolymer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMANDTARGETPOLYMER_H__C06362C9_D88A_4049_BC4D_93B67163BDAA__INCLUDED_)
#define AFX_COMMANDTARGETPOLYMER_H__C06362C9_D88A_4049_BC4D_93B67163BDAA__INCLUDED_


#include "CommandTargetNode.h"
#include "xxBase.h"	// Added by ClassView

class CCommandTargetPolymer : public CCommandTargetNode
{
	// ****************************************
	// Construction/Destruction
public:

	CCommandTargetPolymer(const zString label);
	CCommandTargetPolymer(const zString label, long polyType, PolymerVector polymers);

	virtual ~CCommandTargetPolymer();

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

    // Overriden base class function to replace bead is with actual beads

    virtual bool ReplaceProxyIds(CInitialState* const pIS);

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
    virtual void AddPolymer(CPolymer* pPolymer);
    virtual void RemovePolymer(CPolymer* pPolymer);


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

	long		   m_OriginalType;	// Type of polymers in target (unchangeable)

	PolymerVector  m_Polymers;		// Set of polymers contained in the target
};

#endif // !defined(AFX_COMMANDTARGETPOLYMER_H__C06362C9_D88A_4049_BC4D_93B67163BDAA__INCLUDED_)

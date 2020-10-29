// CommandTargetComposite.h: interface for the CCommandTargetComposite class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMANDTARGETCOMPOSITE_H__72D49055_8858_4DBF_A647_D866B00C06E5__INCLUDED_)
#define AFX_COMMANDTARGETCOMPOSITE_H__72D49055_8858_4DBF_A647_D866B00C06E5__INCLUDED_


#include "CommandTargetNode.h"

class CCommandTargetComposite : public CCommandTargetNode  
{
	// ****************************************
	// Construction/Destruction
public:

	CCommandTargetComposite(const zString label);
	virtual ~CCommandTargetComposite();

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

    // Overriden base class function to replace target proxy ids with targets

    virtual bool ReplaceProxyIds(CInitialState* const pIS);

    // ****************************************
	// Public access functions
public:

	// Function needed to distinguish node types

	virtual bool IsComposite() const;
	virtual bool IsSimpleBeadTargetInComposite() const;

	// IQueryBeadTarget interface

	virtual bool  IsBeadTypeInTarget(long type)     const;
	virtual long GetBeadTotalForType(long type)     const;
	virtual long	    GetBeadTotal()              const;
	virtual long    GetFirstBeadType()              const;
	virtual BeadVector	    GetBeads()		        const;

 	virtual bool ChangeNamedBeadType(long newType);

    // IQueryPolymerTarget interface

	virtual bool IsPolymerTypeInTarget(long type)	const;
	virtual long GetPolymerTotalForType(long type)	const;
	virtual long GetPolymerTotal()					const;
	virtual PolymerVector GetPolymers()				const;

	// Functions peculiar to composite targets

	virtual long GetTargetTotal() const;
	virtual bool AddTarget(CCommandTargetNode* const pTarget);
	virtual bool RemoveTarget(const zString label);
	CCommandTargetNode* GetTarget(const zString label) const;
	CCommandTargetNode* GetTargetRecursively(const zString label) const;

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

	StringTargetMap			m_mTargetMap;	// Map of label, target pairs
};

#endif // !defined(AFX_COMMANDTARGETCOMPOSITE_H__72D49055_8858_4DBF_A647_D866B00C06E5__INCLUDED_)

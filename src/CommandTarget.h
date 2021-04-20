// CommandTarget.h: interface for the CCommandTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMANDTARGET_H__0A27C00E_7293_453B_90B3_EA769FB96669__INCLUDED_)
#define AFX_COMMANDTARGET_H__0A27C00E_7293_453B_90B3_EA769FB96669__INCLUDED_


#include "CommandTargetNode.h"

class CCommandTarget : public CCommandTargetNode
{
	// ****************************************
	// Construction/Destruction
public:

	CCommandTarget(const zString label);
	CCommandTarget(const zString label, long beadType, BeadVector beads);
	virtual ~CCommandTarget();


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

    virtual aaVector GetCM() const;
    
	virtual bool IsBeadTypeInTarget(long type)      const;
	virtual long GetBeadTotalForType(long type)     const;
	virtual long	   GetBeadTotal()		        const;
	virtual long GetFirstBeadType()                 const;
	virtual BeadVector	   GetBeads()		        const;

 	virtual bool ChangeNamedBeadType(long newType);

	// Helper functions used to track bead type changes that mask the 
	// difference between composite targets and single bead targets

	virtual long GetCurrentBeadType() const;
	virtual long GetPreviousBeadType(long newType) const;
	virtual long GetOriginalBeadType() const;
	virtual bool AddBeadTypePair(long newType, long oldType);
	virtual bool RemoveBeadTypePair(long newType);
	virtual bool ClearBeadTypePairs();


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

	long		m_OriginalType;	// Original bead type
	long	    m_CurrentType;	// Latest type assigned to beads

	BeadVector	m_Beads;		// Set of beads contained in the target

	LongLongMap	m_mTypeMap;		// Map of (newType, oldType) pairs in target(s)

};

#endif // !defined(AFX_COMMANDTARGET_H__0A27C00E_7293_453B_90B3_EA769FB96669__INCLUDED_)

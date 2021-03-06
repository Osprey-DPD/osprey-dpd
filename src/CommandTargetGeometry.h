// CommandTargetGeometry.h: interface for the CCommandTargetGeometryGeometry class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMANDTARGETGEOMETRY_H__FB091DB5_70AE_42CD_93E9_6612ABB124C0__INCLUDED_)
#define AFX_COMMANDTARGETGEOMETRY_H__FB091DB5_70AE_42CD_93E9_6612ABB124C0__INCLUDED_


#include "CommandTargetNode.h"

class CCommandTargetGeometry : public CCommandTargetNode
{
	// ****************************************
	// Construction/Destruction
public:

	CCommandTargetGeometry(const zString label);
	CCommandTargetGeometry(const zString label, long beadType, BeadVector beads);
	virtual ~CCommandTargetGeometry();

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

	// Functions common to all targets

	virtual bool IsBeadTypeInTarget(long type)  const;
	virtual long GetBeadTotalForType(long type) const;
	virtual long	   GetBeadTotal()		    const;
	virtual BeadVector	   GetBeads()		    const;

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
	long		m_CurrentType;	// Latest type assigned to beads

	BeadVector	m_Beads;		// Set of beads contained in the target

	LongLongMap	m_mTypeMap;		// Map of (newType, oldType) pairs in target(s)

};

#endif // !defined(AFX_COMMANDTARGETGEOMETRY_H__FB091DB5_70AE_42CD_93E9_6612ABB124C0__INCLUDED_)

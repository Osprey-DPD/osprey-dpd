// CommandTargetACNFreePolymers.h: interface for the CCommandTargetACNFreePolymers class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMANDTARGETACNFREEPOLYMERS_H__604BE42D_F1B4_4639_B58C_66D0F5A53313__INCLUDED_)
#define AFX_COMMANDTARGETACNFREEPOLYMERS_H__604BE42D_F1B4_4639_B58C_66D0F5A53313__INCLUDED_

#include "xxBase.h"	// Added by ClassView


// Forward declarations

class aeActiveCellNetwork;



#include "CommandTargetNode.h"

class CCommandTargetACNFreePolymers : public CCommandTargetNode
{
	// ****************************************
	// Construction/Destruction
public:

	CCommandTargetACNFreePolymers(const zString label);
	CCommandTargetACNFreePolymers(const zString label, aeActiveCellNetwork* pACN);

	virtual ~CCommandTargetACNFreePolymers();

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


    const zString GetACNType() const;

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

#endif // !defined(AFX_COMMANDTARGETACNFREEPOLYMERS_H__604BE42D_F1B4_4639_B58C_66D0F5A53313__INCLUDED_)

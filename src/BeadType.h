// BeadType.h: interface for the CBeadType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BEADTYPE_H__8EFC5A60_5594_11D3_820E_0060088AD300__INCLUDED_)
#define AFX_BEADTYPE_H__8EFC5A60_5594_11D3_820E_0060088AD300__INCLUDED_


// Include file to gain access to the STL container classes 

#include "xxBase.h"


// Forward declarations

class CBeadStructure;
class CBDBeadStructure;
class CDPDBeadStructure;
class CMDBeadStructure;

class CBeadType  
{
	// ****************************************
	// Construction/Destruction: protected constructor declared below
public:

	virtual ~CBeadType();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend bool operator==(const CBeadType &a, const CBeadType &b);

	static CBeadType* AddBDBeadType(zString name, double radius, double transDiff, double rotDiff, zDoubleVector consInt, zDoubleVector dissInt);

    static CBeadType* AddDPDBeadType(zString name, zDoubleVector consInt, zDoubleVector dissInt);

    static CBeadType* AddDPDBeadType(zString name, double radius, zDoubleVector consInt, zDoubleVector dissInt);

    // Function to add a bead type in a liquid-gas DPD system

    static CBeadType* AddDPDBeadType(zString name, double radius, double lgRadius, zDoubleVector consInt, zDoubleVector dissInt, zDoubleVector lgInt);

    static CBeadType* AddMDBeadType(zString name,  zDoubleVector ljDepth, zDoubleVector ljRange,
												   zDoubleVector scDepth, zDoubleVector scRange);

	static long GetTotal();
	static void ZeroTotal();

private:

	static long m_BeadTypeTotal;

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:


	// ****************************************
	// Public access functions
public:

	const inline zString& GetName() const {return m_Name;}

	const CBDBeadStructure*  GetBDStructure() const;
	const CDPDBeadStructure* GetDPDStructure() const;
	const CMDBeadStructure*  GetMDStructure() const;

	// ****************************************
	// Protected local functions
protected:

	// Protected constructor ensures that instances of this class can only
    // be created by calling one of its static AddXXXBeadType() functions.

	CBeadType(zString name, CBeadStructure* const pStructure);


	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members

	zString m_Name;

	CBeadStructure* const m_pStructure;
};

#endif // !defined(AFX_BEADTYPE_H__8EFC5A60_5594_11D3_820E_0060088AD300__INCLUDED_)

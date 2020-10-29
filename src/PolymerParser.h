// PolymerParser.h: interface for the CPolymerParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POLYMERPARSER_H__A00386C2_80A4_4B0E_B3E3_7C64EA530F10__INCLUDED_)
#define AFX_POLYMERPARSER_H__A00386C2_80A4_4B0E_B3E3_7C64EA530F10__INCLUDED_


#include "xxBase.h"

class CPolymerParser : public xxBase
{
public:
	// ****************************************
	// Construction/Destruction
public:

	CPolymerParser();

	virtual ~CPolymerParser();

	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// Public access functions
public:

	const zString ConvertToTags(const zString shape);

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:


	// ****************************************
	// Protected local functions
protected:


	// ****************************************
	// Implementation

protected:



	// ****************************************
	// Private functions
private:

	// Function to parse an element and return a tag or tags

	const zString ParseElement(bool bNewBeadGroup, const zString element);

	// Function to return a tag-wrapped bead name

	const zString BeadNameTag(const zString beadName) const;

	// Helper functions to parse the shape string

	bool IsBeadName(const zString name) const;
	bool IsTailEmpty(const zString tail) const;
	bool IsStringEmpty(const zString oldString) const;

	zString CAR(const zString oldString);
	zString CDR(const zString oldString);
	zString RemoveOuterBrackets(zString element);
	zString SpaceStrippedString(const zString oldString) const;

	// ****************************************
	// Data members

private:

	// Strings used by the parser to verify the validity of bead names and
	// identify branch and loop elements in a polymer's shape string.

	const zString m_SpaceChar;
	const zString m_BranchChar;
	const zString m_LoopChar;
	const zString m_ValidNameChars; 
	const zString m_ValidNumOpChars;
	const zString m_ValidSeparatorOpChars;
	const zString m_ValidChars;

	// Local data used to convert parts of a polymer shape string into tags

	const zString m_Shape;				//  <Shape>
	const zString m_CloseShape;			//  </Shape>
	const zString m_BeadName;			//	<BeadName>
	const zString m_CloseBeadName;		//	</BeadName>
	const zString m_BeadGroup;			//	<BeadGroup>
	const zString m_CloseBeadGroup;		//	</BeadGroup>
	const zString m_BeadGroupMulti;		//	<BeadGroup multiple=> Note missing attribute!
	const zString m_BeadGroupBranch;	//	<BeadGroup branch="true">
	const zString m_BeadGroupLoop;		//	<BeadGroup loop="true">


};

#endif // !defined(AFX_POLYMERPARSER_H__A00386C2_80A4_4B0E_B3E3_7C64EA530F10__INCLUDED_)

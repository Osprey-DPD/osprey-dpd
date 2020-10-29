// xxCommandObject.h: interface for the xxCommandObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXCOMMANDOBJECT_H__EBF46FFA_F98F_11D3_BF2D_004095086186__INCLUDED_)
#define AFX_XXCOMMANDOBJECT_H__EBF46FFA_F98F_11D3_BF2D_004095086186__INCLUDED_

// Forward declarations

class xxCommand;


// Include file to gain access to the STL container classes 

#include "xxBase.h"


class xxCommandObject  
{
public:
	xxCommandObject(const zString type, long executionTime);
	virtual ~xxCommandObject();

	friend zOutStream& operator<<(zOutStream& os, const xxCommandObject& ComOb);
	friend zInStream&  operator>>(zInStream& is,        xxCommandObject& ComOb);

	const xxCommand* GetCommand() const;
	bool IsCommandValid() const;
	long GetExecutionTime() const;

private:
	void SetCommandValid(bool bValid);

	xxCommand* m_pCommand;		// Pointer to command
	bool m_bCommandValid;		// Flag showing if command data is valid

};

#endif // !defined(AFX_XXCOMMANDOBJECT_H__EBF46FFA_F98F_11D3_BF2D_004095086186__INCLUDED_)

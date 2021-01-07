// LogVesicleFusionPore.h: interface for the CLogVesicleFusionPore class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGVESICLEFUSIONPORE_H__46D0EC1A_7C8B_4E9E_94E3_1E85F492A194__INCLUDED_)
#define AFX_LOGVESICLEFUSIONPORE_H__46D0EC1A_7C8B_4E9E_94E3_1E85F492A194__INCLUDED_


#include "LogEventMessage.h"

class CLogVesicleFusionPore : public CLogEventMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogVesicleFusionPore(long time, const zString name1, long type1, 
									 const zString name2, long type2, long poreSize);
	virtual ~CLogVesicleFusionPore();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogVesicleFusionPore& rMsg);

	// ****************************************
	// Public access functions
public:

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	virtual	void Serialize(zOutStream& os) const;

	// ****************************************
	// Protected local functions
protected:

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:
	
	// Explicitly disallow the copy constructor and assignment operators
	// by declaring them private and providing NO definitions.

	CLogVesicleFusionPore(const CLogVesicleFusionPore& oldMessage);
	CLogVesicleFusionPore& operator=(const CLogVesicleFusionPore& rhs);


	// ****************************************
	// Data members
private:

	const zString m_Name1;				// Aggregate name, e.g., bilayer
	const long    m_Type1;				// Aggregate id, e.g., 0, 1, 2
	const zString m_Name2;				// Aggregate name, e.g., bilayer
	const long    m_Type2;				// Aggregate id, e.g., 0, 1, 2
	const long	  m_PoreSize;			// Fusion pore size in grid points

};

#endif // !defined(AFX_LOGVESICLEFUSIONPORE_H__46D0EC1A_7C8B_4E9E_94E3_1E85F492A194__INCLUDED_)

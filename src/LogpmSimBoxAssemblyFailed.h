// LogpmSimBoxAssemblyFailed.h: interface for the CLogpmSimBoxAssemblyFailed class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGPMSIMBOXASSEMBLYFAILED_H__21F9266B_36F3_4BEF_8A49_FCF97034C82A__INCLUDED_)
#define AFX_LOGPMSIMBOXASSEMBLYFAILED_H__21F9266B_36F3_4BEF_8A49_FCF97034C82A__INCLUDED_


#include "LogErrorMessage.h"

class CLogpmSimBoxAssemblyFailed : public CLogErrorMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogpmSimBoxAssemblyFailed(long time, long pid);

	virtual ~CLogpmSimBoxAssemblyFailed();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogpmSimBoxAssemblyFailed& rMsg);

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

	CLogpmSimBoxAssemblyFailed(const CLogpmSimBoxAssemblyFailed& oldMessage);
	CLogpmSimBoxAssemblyFailed& operator=(const CLogpmSimBoxAssemblyFailed& rhs);

	// ****************************************
	// Data members
private:

    const long m_ProcId;  // Processor id for a given processor    
};

#endif // !defined(AFX_LOGPMSIMBOXASSEMBLYFAILED_H__21F9266B_36F3_4BEF_8A49_FCF97034C82A__INCLUDED_)

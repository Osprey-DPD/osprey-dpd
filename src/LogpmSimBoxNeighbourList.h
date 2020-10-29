// LogpmSimBoxNeighbourList.h: interface for the CLogpmSimBoxNeighbourList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGPMSIMBOXNEIGHBOURLIST_H__06138CA4_ADCD_4F55_9016_0AAD858B513E__INCLUDED_)
#define AFX_LOGPMSIMBOXNEIGHBOURLIST_H__06138CA4_ADCD_4F55_9016_0AAD858B513E__INCLUDED_


#include "LogInfoMessage.h"

class CLogpmSimBoxNeighbourList : public CLogInfoMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogpmSimBoxNeighbourList(long time, long pid, long nnProcId[27]);

	virtual ~CLogpmSimBoxNeighbourList();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogpmSimBoxNeighbourList& rMsg);

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

	CLogpmSimBoxNeighbourList(const CLogpmSimBoxNeighbourList& oldMessage);
	CLogpmSimBoxNeighbourList& operator=(const CLogpmSimBoxNeighbourList& rhs);

	// ****************************************
	// Data members
private:

    const long m_ProcId;  // Processor id for a given processor
    long m_NNProcId[27];  // List of nearest-neighbour processor ids
    
};

#endif // !defined(AFX_LOGPMSIMBOXNEIGHBOURLIST_H__06138CA4_ADCD_4F55_9016_0AAD858B513E__INCLUDED_)

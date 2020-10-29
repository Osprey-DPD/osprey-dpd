// IActiveEventSource.h: interface for the IActiveEventSource class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IACTIVEEVENTSOURCE_H__3549B086_66CD_438C_B2FC_E7056CF8A066__INCLUDED_)
#define AFX_IACTIVEEVENTSOURCE_H__3549B086_66CD_438C_B2FC_E7056CF8A066__INCLUDED_


// Include files

#include "aeActiveSimBox.h"


#include "xxBase.h"

class IActiveEventSource : public xxBase
{
	// ****************************************
	// Construction/Destruction: protected constructor declared below so that only
	// derived classes can be instantiated. This is an interface base class
	// that wraps the active SimBox to that event interface classes can access it.
public:

	virtual ~IActiveEventSource();


	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:


	// ****************************************
	// Public access functions
public:


	// ****************************************
	// Protected local functions
protected:

	IActiveEventSource(aeActiveSimBox* pShadow);

	// ****************************************
	// Implementation
protected:

	aeActiveSimBox* const m_pShadow;  // Pointer to the aeActiveSimBox instance

	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members: 
private:


};

#endif // !defined(AFX_IACTIVEEVENTSOURCE_H__3549B086_66CD_438C_B2FC_E7056CF8A066__INCLUDED_)

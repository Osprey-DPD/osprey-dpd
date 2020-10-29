// mpsCNTCell.h: interface for the mpsCNTCell class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MPSCNTCELL_H__1FC839E2_1F1F_4054_9720_9AF4828D8850__INCLUDED_)
#define AFX_MPSCNTCELL_H__1FC839E2_1F1F_4054_9720_9AF4828D8850__INCLUDED_


#include "AbstractCell.h"

class mpsCNTCell : public CAbstractCell  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	mpsCNTCell();
	mpsCNTCell(long index, long coords[3], bool flag);

	virtual ~mpsCNTCell();


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


	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:
	
	bool m_bExternal;   // Flag showing if the cell is on the boundary between processors


};

#endif // !defined(AFX_MPSCNTCELL_H__1FC839E2_1F1F_4054_9720_9AF4828D8850__INCLUDED_)

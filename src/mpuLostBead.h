// mpuLostBead.h: interface for the mpuLostBead class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MPULOSTBEAD_H__BA6BEE1D_B12C_4B05_A63D_1AEE64765EF4__INCLUDED_)
#define AFX_MPULOSTBEAD_H__BA6BEE1D_B12C_4B05_A63D_1AEE64765EF4__INCLUDED_


class mpuLostBead  
{
	// ****************************************
	// Construction/Destruction
public:

	mpuLostBead(long id, long lCellId, long gCellId);
	mpuLostBead(const mpuLostBead& oldNPD);

	virtual ~mpuLostBead();

	// ****************************************
	// Global functions, static member functions and variables



	// ****************************************
	// PVFs that must be overridden by all derived classes
public:



	// ****************************************
	// Public access functions
public:


	// ****************************************
	// Protected local functions


	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
public:

    const long m_id;             // Bead id
    const long m_LosingCellId;   // Losing CNT cell id
    const long m_GainingCellId;  // Gaining CNT cell id


};

#endif // !defined(AFX_MPULOSTBEAD_H__BA6BEE1D_B12C_4B05_A63D_1AEE64765EF4__INCLUDED_)

// mpsNPD.h: interface for the mpsNPD class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MPSNPD_H__85361394_5EC7_415F_ACBF_9FAD0F1D9CE6__INCLUDED_)
#define AFX_MPSNPD_H__85361394_5EC7_415F_ACBF_9FAD0F1D9CE6__INCLUDED_


class mpsNPD  
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mpsNPD(long pid, long pi, long pj, long pk);
	mpsNPD(const mpsNPD& oldNPD);

	virtual ~mpsNPD();

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
private:

    const long m_pid;  // Processor id for containing processor (same as rank)
    const long m_pi;
    const long m_pj;   // Coordinates of containing processor in the set
    const long m_pk;


};

#endif // !defined(AFX_MPSNPD_H__85361394_5EC7_415F_ACBF_9FAD0F1D9CE6__INCLUDED_)

// tguRandom1dVector.h: interface for the tguRandom1dVector class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TGURANDOM1DVECTOR_H__FD7DADCD_99C5_4130_9EFA_C1E63F561349__INCLUDED_)
#define AFX_TGURANDOM1DVECTOR_H__FD7DADCD_99C5_4130_9EFA_C1E63F561349__INCLUDED_


#include "tguRandomVectorType.h"

class tguRandom1dVector : public tguRandomVectorType
{
    // ****************************************
	// Construction/Destruction:
public:

    tguRandom1dVector(double delta, double xmin, double xmax);

    virtual ~tguRandom1dVector();


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

	// Protected constructors ensure that only subclasses can be instantiated



	// ****************************************
	// Implementation
protected:


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

    double  m_XMin;    // Minimum value of argument (inclusive)
    double  m_XMax;    // Maximum value of argument (inclusiveO

};

#endif // !defined(AFX_TGURANDOM1DVECTOR_H__FD7DADCD_99C5_4130_9EFA_C1E63F561349__INCLUDED_)

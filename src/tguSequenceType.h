// tguSequenceType.h: interface for the tguSequenceType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TGUSEQUENCETYPE_H__67E5E5AA_25B5_4641_A493_3F94B6D82E10__INCLUDED_)
#define AFX_TGUSEQUENCETYPE_H__67E5E5AA_25B5_4641_A493_3F94B6D82E10__INCLUDED_



#include "tguArgumentType.h"

class tguSequenceType : public tguArgumentType
{
    // ****************************************
	// Construction/Destruction:
public:

    tguSequenceType(long initial, long delta);
    tguSequenceType(double initial, double delta);
    tguSequenceType(zString initial);

    virtual ~tguSequenceType();


	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

    // Functions that increment and return the value
	void GetNextValue(long* pValue);
	void GetNextValue(double* pValue);
	void GetNextValue(zString* pValue);

	// ****************************************
	// Public access functions
public:

    // Function that returns the number of invocations so far
    inline long   GetCounter()              const {return m_Counter;}

	// ****************************************
	// Protected local functions
protected:



	// ****************************************
	// Implementation
protected:


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

    long     m_InitialZ;    // Initial value of an integer sequence
    double   m_InitialR;    // Initial value of a real sequence
    zString  m_InitialS;    // Root value of a string sequence

    long     m_DZ;          // Integer increment
    double   m_DR;          // Real increment


    // Local variables

    long     m_Counter;     // Count of the number of invocations so far



};

#endif // !defined(AFX_TGUSEQUENCETYPE_H__67E5E5AA_25B5_4641_A493_3F94B6D82E10__INCLUDED_)

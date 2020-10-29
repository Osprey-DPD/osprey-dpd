// tguArgumentType.h: interface for the tguArgumentType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TGUARGUMENTTYPE_H__E31200D9_1B5D_41FB_BF01_6AAA12285928__INCLUDED_)
#define AFX_TGUARGUMENTTYPE_H__E31200D9_1B5D_41FB_BF01_6AAA12285928__INCLUDED_



#include "xxBase.h"

class tguArgumentType : public xxBase
{
    // ****************************************
	// Construction/Destruction: protected constructors declared below
public:

    virtual ~tguArgumentType();


	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

    // Functions that increment and return the current value

    virtual void GetNextValue(long* pValue)    = 0;
    virtual void GetNextValue(double* pValue)  = 0;
    virtual void GetNextValue(zString* pValue) = 0;


	// ****************************************
	// Public access functions
public:

    // Functions that return the current value without incrementing it

     virtual void GetValue(long* pValue)    const {*pValue = m_Integer;};
     virtual void GetValue(double* pValue)  const {*pValue = m_Real;};
     virtual void GetValue(zString* pValue) const {*pValue = m_String;};


	// ****************************************
	// Protected local functions
protected:

	// Protected constructors ensure that only subclasses can be instantiated

    tguArgumentType(long value);
    tguArgumentType(double value);
    tguArgumentType(zString value);


	// ****************************************
	// Implementation
protected:

    long     m_Integer;  // Integer-valued argument
    double   m_Real;     // Real-valued argument
    zString  m_String;   // String-valued argument



	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:



};

#endif // !defined(AFX_TGUARGUMENTTYPE_H__E31200D9_1B5D_41FB_BF01_6AAA12285928__INCLUDED_)

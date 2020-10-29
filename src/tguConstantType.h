// tguConstantType.h: interface for the tguConstantType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TGUCONSTANTTYPE_H__B1EF94DE_A033_4794_8318_501B318D298A__INCLUDED_)
#define AFX_TGUCONSTANTTYPE_H__B1EF94DE_A033_4794_8318_501B318D298A__INCLUDED_



#include "tguArgumentType.h"

class tguConstantType : public tguArgumentType
{
    // ****************************************
	// Construction/Destruction:
public:

    tguConstantType(long value);
    tguConstantType(double value);
    tguConstantType(zString value);

    virtual ~tguConstantType();


	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	virtual void GetNextValue(long* pValue);
	virtual void GetNextValue(double* pValue);
	virtual void GetNextValue(zString* pValue);

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


};

#endif // !defined(AFX_TGUCONSTANTTYPE_H__B1EF94DE_A033_4794_8318_501B318D298A__INCLUDED_)

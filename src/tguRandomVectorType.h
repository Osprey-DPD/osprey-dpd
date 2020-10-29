// tguRandomVectorType.h: interface for the tguRandomVectorType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TGURANDOMVECTORTYPE_H__818B7F96_8658_41A1_B505_560C45C12A68__INCLUDED_)
#define AFX_TGURANDOMVECTORTYPE_H__818B7F96_8658_41A1_B505_560C45C12A68__INCLUDED_


// Include files

#include "CNTCell.h"   // Needed for the RNG

#include "tguArgumentType.h"

class tguRandomVectorType : public tguArgumentType
{
    // ****************************************
	// Construction/Destruction:
public:

    tguRandomVectorType(double delta, double value);

    virtual ~tguRandomVectorType();


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

    inline double GetExclusionRadius() const {return m_Delta;}


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

    double  m_Delta;       // Exclusion radius (can be zero)

};

#endif // !defined(AFX_TGURANDOMVECTORTYPE_H__818B7F96_8658_41A1_B505_560C45C12A68__INCLUDED_)

// aeBondProximityOn.h: interface for the aeBondProximityOn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AEBONDPROXIMITYON_H__4803B0B9_F727_4250_A4E1_12293C4A8149__INCLUDED_)
#define AFX_AEBONDPROXIMITYON_H__4803B0B9_F727_4250_A4E1_12293C4A8149__INCLUDED_


#include "aeBondOnCondition.h"

class aeBondProximityOn : public aeBondOnCondition  
{
	// ****************************************
	// Construction/Destruction
public:

	aeBondProximityOn(double max);
	aeBondProximityOn(double maxHead, double maxTail);

	virtual ~aeBondProximityOn();


	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	virtual bool ActivateHead(const aeActiveBond* const pEnd, const aeActiveBond* const pFree) const;
	virtual bool ActivateTail(const aeActiveBond* const pEnd, const aeActiveBond* const pFree) const;

	// ****************************************
	// Public access functions
public:

	inline double GetHeadSeparation() const {return m_MaxSeparation[0];}
	inline double GetTailSeparation() const {return m_MaxSeparation[1];}

	void SetSeparation(double max);		// Symmetric values
	void SetHeadSeparation(double max);
	void SetTailSeparation(double max);



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

	const bool m_bAsymmetry;		// Flag showing if bond formation is asymmetric
	double m_MaxSeparation[2];		// Max separation for bond to be formed at each end
	double m_MaxSepSquared[2];		// Max separation squared

};

#endif // !defined(AFX_AEBONDPROXIMITYON_H__4803B0B9_F727_4250_A4E1_12293C4A8149__INCLUDED_)

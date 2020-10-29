// aeBondOffRate.h: interface for the aeBondOffRate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AEBONDOFFRATE_H__BE593621_10B2_4453_A4FB_9DC5E6A3124E__INCLUDED_)
#define AFX_AEBONDOFFRATE_H__BE593621_10B2_4453_A4FB_9DC5E6A3124E__INCLUDED_


#include "aeBondOffCondition.h"

class aeBondOffRate : public aeBondOffCondition  
{
	// ****************************************
	// Construction/Destruction
public:

	aeBondOffRate(double prob);
	aeBondOffRate(double headProb, double tailProb);

	virtual ~aeBondOffRate();


	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	virtual bool ActivateHead(const aeActiveBond* const pEnd) const;
	virtual bool ActivateTail(const aeActiveBond* const pEnd) const;

	// ****************************************
	// Public access functions
public:

	inline double GetHeadProbability() const {return m_HeadProbability;}
	inline double GetTailProbability() const {return m_TailProbability;}

	void SetHeadProbability(double prob);
	void SetTailProbability(double prob);

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

	double m_HeadProbability;	// Probability of a monomer detaching from its head (ie, the polymer's tail)
	double m_TailProbability;	// Probability of a monomer detaching from its tail (ie, the polymer's head)

};

#endif // !defined(AFX_AEBONDOFFRATE_H__BE593621_10B2_4453_A4FB_9DC5E6A3124E__INCLUDED_)

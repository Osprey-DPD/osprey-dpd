// aeBondProximityOff.h: interface for the aeBondProximityOff class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AEBONDPROXIMITYOFF_H__72CDCB21_3AE6_408D_B7DC_411451BC5908__INCLUDED_)
#define AFX_AEBONDPROXIMITYOFF_H__72CDCB21_3AE6_408D_B7DC_411451BC5908__INCLUDED_


#include "aeBondOffCondition.h"

class aeBondProximityOff : public aeBondOffCondition  
{
	// ****************************************
	// Construction/Destruction
public:

	aeBondProximityOff(double max);
	aeBondProximityOff(double maxHead, double maxTail);

	
	virtual ~aeBondProximityOff();


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

	inline double GetHeadSeparation() const {return m_MaxSeparation[0];}
	inline double GetTailSeparation() const {return m_MaxSeparation[1];}

	void SetSeparation(double max);
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

	const bool m_bAsymmetry;		// Flag showing if bond dissociation is asymmetric
	double m_MaxSeparation[2];		// Max separation for bond to dissociate at each end
	double m_MaxSepSquared[2];		// Max separation squared

};

#endif // !defined(AFX_AEBONDPROXIMITYOFF_H__72CDCB21_3AE6_408D_B7DC_411451BC5908__INCLUDED_)

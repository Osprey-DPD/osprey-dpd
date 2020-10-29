// ForceTarget.h: interface for the CForceTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FORCETARGET_H__488D9844_0C5C_11D4_BF30_004095086186__INCLUDED_)
#define AFX_FORCETARGET_H__488D9844_0C5C_11D4_BF30_004095086186__INCLUDED_


// Forward declarations

class CForceLaw;


#include "xxBase.h"

class CForceTarget : public xxBase  
{
public:
	CForceTarget(const zString label, BeadVector beads);
	virtual ~CForceTarget();

	inline const zString     GetLabel()   const {return m_Label;}
	inline const BeadVector& GetBeads()   const {return m_Beads;}
	inline		 long		 GetEndTime() const {return m_End;}

	void SetForceLaw(CForceLaw* const pForce);

	bool AddForce(long simTime);	

	void AddBeads(const BeadVector& beads);
	void RemoveDuplicateBeads();

private:
	const zString m_Label;			// Aggregate identifying label
	BeadVector m_Beads;				// Vector of beads composing aggregate
	long m_End;						// Time at which force ends

	CForceLaw* m_ForceLaw;			// Pointer to a particular force object


};

#endif // !defined(AFX_FORCETARGET_H__488D9844_0C5C_11D4_BF30_004095086186__INCLUDED_)

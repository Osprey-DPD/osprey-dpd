// BeadChargeWrapper.h: interface for the CBeadChargeWrapper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BEADCHARGEWRAPPER_H__9662103E_8034_4119_8CB7_21488FA8D8C2__INCLUDED_)
#define AFX_BEADCHARGEWRAPPER_H__9662103E_8034_4119_8CB7_21488FA8D8C2__INCLUDED_


// Include files to give access to the underlying CAbstractBead object

#include "AbstractBead.h"


class CBeadChargeWrapper  
{
	// The SimBox must be a friend so that it can create the charged beads
	// and tell them to calculate their forces. The constructor is private 
	// so that this wrapper class cannot be instantiated elsewhere, and the
	// destructor is not virtual because we have no base classes.
	// Other command classes that need to modify charged beads must also be
	// declared as friends here.

	friend class CSimBox;
	friend class ccChargeBeadByTypeImpl;
	friend class ccUnchargeBeadByTypeImpl;

public:
	~CBeadChargeWrapper();


	// Forwarding functions to the contained CAbstractBead object

	inline long	  GetId()			const {return m_pBead->GetId();}
	inline long	  GetType()			const {return m_pBead->GetType();}
	inline bool	  GetMovable()		const {return m_pBead->GetMovable();}
	inline double GetXPos()			const {return m_pBead->GetXPos();}
	inline double GetYPos()			const {return m_pBead->GetYPos();}
	inline double GetZPos()			const {return m_pBead->GetZPos();}
	inline double GetXForce()		const {return m_pBead->GetXForce();}
	inline double GetYForce()		const {return m_pBead->GetYForce();}
	inline double GetZForce()		const {return m_pBead->GetZForce();}

private:
	CBeadChargeWrapper(CAbstractBead* const pBead, double strength, double range, double kappa);

	void AddForce(CBeadChargeWrapper* const pChargedBead2, double dx[3]);


private:
	CAbstractBead* const m_pBead;		// Contained bead pointer

	double m_Strength;					// Strength of screened charge force
	double m_Range;						// Range of force
	double m_Kappa;						// Inverse range of force

};

#endif // !defined(AFX_BEADCHARGEWRAPPER_H__9662103E_8034_4119_8CB7_21488FA8D8C2__INCLUDED_)

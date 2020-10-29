// PolymerCrossLink.h: interface for the CPolymerCrossLink class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POLYMERCROSSLINK_H__362D55BE_EAA5_42B5_883B_D7AFEA08F967__INCLUDED_)
#define AFX_POLYMERCROSSLINK_H__362D55BE_EAA5_42B5_883B_D7AFEA08F967__INCLUDED_


// Include file to gain access to the typedef for zString 

#include "xxBase.h"



class CPolymerCrossLink  
{
public:
	CPolymerCrossLink();
	CPolymerCrossLink(zString name, double fraction, long position, 
					  double spring, double length);
	CPolymerCrossLink(zString name, long type, double fraction, long position, 
					  double spring, double length);

	virtual ~CPolymerCrossLink();

	// Access functions to the cross-link's data

	inline const zString GetName()			const {return m_Name;}
	inline long		GetType()				const {return m_Type;}
	inline double	GetFraction()			const {return m_Fraction;}
	inline long		GetPosition()			const {return m_Position;}
	inline double	GetSpringConstant()		const {return m_SpringConstant;}
	inline double	GetUnstretchedLength()  const {return m_UnstretchedLength;}

	// Function to allow the type of the polymer to be set manually

	inline void		SetType(long type) {m_Type = type;}


private:
	zString m_Name;					// of polymer to cross-link
	long	m_Type;					// of polymer
	double	m_Fraction;				// Fraction of polymers to cross-link
	long	m_Position;				// Bead position in polymer architecture
	double	m_SpringConstant;		// Parameters of Hookean spring cross-linker
	double	m_UnstretchedLength;


};

#endif // !defined(AFX_POLYMERCROSSLINK_H__362D55BE_EAA5_42B5_883B_D7AFEA08F967__INCLUDED_)

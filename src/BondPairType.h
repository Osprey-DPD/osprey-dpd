// BondPairType.h: interface for the CBondPairType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BONDPAIRTYPE_H__1A731794_8ECC_11D4_BF4B_004095086186__INCLUDED_)
#define AFX_BONDPAIRTYPE_H__1A731794_8ECC_11D4_BF4B_004095086186__INCLUDED_


// Include file to gain access to the STL container classes 

#include "xxBase.h"



class CBondPairType  
{
public:
	static long GetTotal();
	static void ZeroTotal();

	static CBondPairType* AddBondPairType(zString name1, zString name2, zString name3, double Strength, double Phi0);

	virtual ~CBondPairType();

	friend bool operator==(const CBondPairType &a, const CBondPairType &b);

	const inline zString& GetFirstName()  const {return m_Name1;}
	const inline zString& GetSecondName() const {return m_Name2;}
	const inline zString& GetThirdName()  const {return m_Name3;}
	inline double GetModulus()			  const {return m_BendingModulus;}
	inline double GetPhi0()				  const {return m_Phi0;}

	void SetModulus(double newModulus);
	void SetPhi0(double newPhi0);

protected:
	CBondPairType(zString name1, zString name2, zString name3, double Strength, double Phi0);

private:
	static long m_BondPairTypeTotal;

	zString m_Name1;			// Name of first bead in bondpair triple
	zString m_Name2;			// Name of middle bead 
	zString m_Name3;			// Name of third bead

	double m_BendingModulus;	// Stiffness of bonds
	double m_Phi0;				// Preferred angle

};

#endif // !defined(AFX_BONDPAIRTYPE_H__1A731794_8ECC_11D4_BF4B_004095086186__INCLUDED_)

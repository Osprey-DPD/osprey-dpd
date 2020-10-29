// BondType.h: interface for the CBondType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BONDTYPE_H__8EFC5A61_5594_11D3_820E_0060088AD300__INCLUDED_)
#define AFX_BONDTYPE_H__8EFC5A61_5594_11D3_820E_0060088AD300__INCLUDED_


// Include file to gain access to the STL container classes 

#include "xxBase.h"


class CBondType  
{
public:
	static long GetTotal();
	static void ZeroTotal();

	static CBondType* AddBondType(zString head, zString tail, double SprConst, double UnStrLen);

	virtual ~CBondType();

	friend bool operator==(const CBondType &a, const CBondType &b);

	const inline zString& GetHeadName() const {return m_headName;}
	const inline zString& GetTailName() const {return m_tailName;}
	inline double GetSpringConst() const {return m_SprConst;}
	inline double GetLength() const {return m_UnStrLen;}

	void SetSpringConst(double springConst);
	void SetLength(double unstretchedLength);

protected:
	CBondType(zString head, zString tail, double SprConst, double UnStrLen);

private:
	static long m_BondTypeTotal;

	zString m_headName;
	zString m_tailName;
	double m_SprConst;
	double m_UnStrLen;
};

#endif // !defined(AFX_BONDTYPE_H__8EFC5A61_5594_11D3_820E_0060088AD300__INCLUDED_)

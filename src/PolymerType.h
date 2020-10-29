// PolymerType.h: interface for the CPolymerType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POLYMERTYPE_H__8EFC5A62_5594_11D3_820E_0060088AD300__INCLUDED_)
#define AFX_POLYMERTYPE_H__8EFC5A62_5594_11D3_820E_0060088AD300__INCLUDED_


// Include file to gain access to the STL container classes 

#include "xxBase.h"


class CPolymerType  
{
public:
	static long GetTotal();
	static void ZeroTotal();

	static CPolymerType* AddPolymerType(zString name, zString shape, double fraction);
	static CPolymerType* AddPolymerType(zString name, zString shape, double fraction,
										zString head, zString tail);

	virtual ~CPolymerType();

	friend bool operator ==(const CPolymerType &a, const CPolymerType &b);

	const inline zString GetName() const {return m_Name;}
	const inline zString GetShape() const {return m_Shape;}
	inline double GetFraction() const {return m_Fraction;}
	const inline zString GetHeadName() const {return m_Head;}
	const inline zString GetTailName() const {return m_Tail;}

protected:
	CPolymerType(zString name, zString shape, double fraction);
	CPolymerType(zString name, zString shape, double fraction,
				 zString head, zString tail);

private:
	static long m_PolymerTypeTotal;

	zString m_Name;
	zString m_Shape;
	double m_Fraction;
	zString m_Head;
	zString m_Tail;
};

#endif // !defined(AFX_POLYMERTYPE_H__8EFC5A62_5594_11D3_820E_0060088AD300__INCLUDED_)

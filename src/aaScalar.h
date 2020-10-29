// aaScalar.h: interface for the aaScalar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AASCALAR_H__C2278A1F_AC86_11D3_BF15_004095086186__INCLUDED_)
#define AFX_AASCALAR_H__C2278A1F_AC86_11D3_BF15_004095086186__INCLUDED_


class aaScalar
{
public:
	aaScalar(double x);
    aaScalar(const aaScalar& oldScalar);
	aaScalar& operator =(const aaScalar &oldScalar);

    ~aaScalar();

	inline double GetData()  const {return m_Data;}
	inline double GetData2() const {return m_Data*m_Data;}

private:
	double m_Data;		// The scalar data item
};


#endif // !defined(AFX_AASCALAR_H__C2278A1F_AC86_11D3_BF15_004095086186__INCLUDED_)

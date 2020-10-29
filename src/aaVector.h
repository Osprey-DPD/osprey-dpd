// aaVector.h: interface for the aaVector class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AAVECTOR_H__C2278A20_AC86_11D3_BF15_004095086186__INCLUDED_)
#define AFX_AAVECTOR_H__C2278A20_AC86_11D3_BF15_004095086186__INCLUDED_


class aaVector 
{
public:
	aaVector(double x, double y, double z);
    aaVector(const aaVector& oldVector);
	aaVector& operator =(const aaVector &oldVector);

	~aaVector();

    inline double GetX() const {return m_Data[0];}
    inline double GetY() const {return m_Data[1];}
    inline double GetZ() const {return m_Data[2];}

	inline double GetData(short int i)  const {return m_Data[i];}
	inline double GetData2(short int i) const {return m_Data[i]*m_Data[i];}

	
	inline aaVector& operator*=(double x) {m_Data[0] *= x; m_Data[1] *= x; m_Data[2] *= x; return *this;}
	inline aaVector& operator/=(double x) {m_Data[0] /= x; m_Data[1] /= x; m_Data[2] /= x; return *this;}

	inline aaVector& operator+=(const aaVector& t) 
							{m_Data[0] += t.m_Data[0]; 
							 m_Data[1] += t.m_Data[1];
							 m_Data[2] += t.m_Data[2];
							 return *this;}

	inline aaVector& operator-=(const aaVector& t) 
							{m_Data[0] -= t.m_Data[0]; 
							 m_Data[1] -= t.m_Data[1];
							 m_Data[2] -= t.m_Data[2];
							 return *this;}

	inline aaVector& operator*=(const aaVector& t) 
							{m_Data[0] *= t.m_Data[0]; 
							 m_Data[1] *= t.m_Data[1];
							 m_Data[2] *= t.m_Data[2];
							 return *this;}

	inline aaVector& operator/=(const aaVector& t) 
							{m_Data[0] /= t.m_Data[0]; 
							 m_Data[1] /= t.m_Data[1];
							 m_Data[2] /= t.m_Data[2];
							 return *this;}

private:
	double m_Data[3];	// the vector data
};

// We make the binary operators global so that they can handle
// commutative operations later.

inline aaVector operator+(const aaVector& t1, const aaVector& t2) 
{
	aaVector result = t1;
	result += t2;
	return result;
}

inline aaVector operator-(const aaVector& t1, const aaVector& t2) 
{
	aaVector result = t1;
	result -= t2;
	return result;
}

inline aaVector operator*(const aaVector& t1, const aaVector& t2) 
{
	aaVector result = t1;
	result *= t2;
	return result;
}

inline aaVector operator/(const aaVector& t1, const aaVector& t2) 
{
	aaVector result = t1;
	result /= t2;
	return result;
}

inline aaVector operator*(const aaVector& t1, double x) 
{
	aaVector result = t1;
	result *= x;
	return result;
}

inline aaVector operator*(double x, const aaVector& t1) 
{
	return t1*x;
}

inline aaVector operator/(const aaVector& t1, double x) 
{
	aaVector result = t1;
	result /= x;
	return result;
}

#endif // !defined(AFX_AAVECTOR_H__C2278A20_AC86_11D3_BF15_004095086186__INCLUDED_)

// aaTensor.h: interface for the aaTensor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AATENSOR_H__C2278A21_AC86_11D3_BF15_004095086186__INCLUDED_)
#define AFX_AATENSOR_H__C2278A21_AC86_11D3_BF15_004095086186__INCLUDED_


class aaTensor
{
public:
	aaTensor(const double* const pData);
    aaTensor(const aaTensor& oldTensor);
	aaTensor& operator =(const aaTensor &oldTensor);

	~aaTensor();

	inline double GetData(short int i)  const {return m_Data[i];}
	inline double GetData2(short int i) const {return m_Data[i]*m_Data[i];}

	inline  aaTensor& operator*=(double x) 
								{	for(short int i=0; i<9; i++)
									{m_Data[i] *= x;}
									return *this;}

	inline  aaTensor& operator/=(double x) 
								{	for(short int i=0; i<9; i++)
									{m_Data[i] /= x;}
									return *this;}

	inline  aaTensor& operator+=(const aaTensor& t) 
								{	for(short int i=0; i<9; i++)
									{m_Data[i] += t.m_Data[i];}
									return *this;}

	inline  aaTensor& operator-=(const aaTensor& t) 
								{	for(short int i=0; i<9; i++)
									{m_Data[i] -= t.m_Data[i];}
									return *this;}

	inline  aaTensor& operator*=(const aaTensor& t) 
								{	for(short int i=0; i<9; i++)
									{m_Data[i] *= t.m_Data[i];}
									return *this;}

	inline  aaTensor& operator/=(const aaTensor& t) 
								{	for(short int i=0; i<9; i++)
									{m_Data[i] /= t.m_Data[i];}
									return *this;}

private:
	double m_Data[9];	// The 3 x 3 tensor data item

};

// We make the binary operators global so that they can handle
// cummutative operations later.

inline aaTensor operator+(const aaTensor& t1, const aaTensor& t2) 
{
	aaTensor result = t1;
	result += t2;
	return result;
}

inline aaTensor operator-(const aaTensor& t1, const aaTensor& t2) 
{
	aaTensor result = t1;
	result -= t2;
	return result;
}

inline aaTensor operator*(const aaTensor& t1, const aaTensor& t2) 
{
	aaTensor result = t1;
	result *= t2;
	return result;
}

inline aaTensor operator/(const aaTensor& t1, const aaTensor& t2) 
{
	aaTensor result = t1;
	result /= t2;
	return result;
}

inline aaTensor operator*(const aaTensor& t1, double x) 
{
	aaTensor result = t1;
	result *= x;
	return result;
}

inline aaTensor operator*(double x, const aaTensor& t1) 
{
	return t1*x;
}

inline aaTensor operator/(const aaTensor& t1, double x) 
{
	aaTensor result = t1;
	result /= x;
	return result;
}


#endif // !defined(AFX_AATENSOR_H__C2278A21_AC86_11D3_BF15_004095086186__INCLUDED_)

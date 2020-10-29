// DiagonaliseMatrix.h: interface for the CDiagonaliseMatrix class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIAGONALISEMATRIX_H__58B6B488_EB6D_4BB7_B2B9_68F1C455D819__INCLUDED_)
#define AFX_DIAGONALISEMATRIX_H__58B6B488_EB6D_4BB7_B2B9_68F1C455D819__INCLUDED_


#include "xxBase.h"

class CDiagonaliseMatrix  : public xxBase
{
public:
	CDiagonaliseMatrix(long order);
	virtual ~CDiagonaliseMatrix();
 
 
    long Diagonalise(const zDoubleVector vMatrix);    // Return value is no of rotations

    inline double GetEigenvalue(long i) const {return m_Eigenvalues.at(i);}

private:

    const long        m_Order;          // Order of symmetric matrix

    zDoubleVector    m_Matrix;          // Vector holding the matrix elements
    zDoubleVector    m_Eigenvectors;    // Vector holding the matrix eigenvectors
    zDoubleVector    m_Eigenvalues;     // Vector holding the eigenvalues

};

#endif // !defined(AFX_DIAGONALISEMATRIX_H__58B6B488_EB6D_4BB7_B2B9_68F1C455D819__INCLUDED_)

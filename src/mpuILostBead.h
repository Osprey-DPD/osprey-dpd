// mpuILostBead.h: interface for the mpuILostBead class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MPUILOSTBEAD_H__5AA0BEAA_F83A_4129_ABBA_176CC66BF79D__INCLUDED_)
#define AFX_MPUILOSTBEAD_H__5AA0BEAA_F83A_4129_ABBA_176CC66BF79D__INCLUDED_


// Foward declarations

class mpsBorder;


// Interface class that wraps an mpsBorder instance giving access only to
// the functions required to store beads that leave the Border region's space.

class mpuILostBead  
{
public:

	mpuILostBead(const mpsBorder* pBorder);

	virtual ~mpuILostBead();


    // Functions passed through to the mpsSimBox instance

	long GetNeighbourPidFromCoordinateDifferences(long alpha, long beta, long gamma) const;

private:

    const mpsBorder* const m_pBorder;
};

#endif // !defined(AFX_MPUILOSTBEAD_H__5AA0BEAA_F83A_4129_ABBA_176CC66BF79D__INCLUDED_)

// MDBeadStructure.h: interface for the CMDBeadStructure class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MDBEADSTRUCTURE_H__2CE23035_D27E_11D3_BF1F_004095086186__INCLUDED_)
#define AFX_MDBEADSTRUCTURE_H__2CE23035_D27E_11D3_BF1F_004095086186__INCLUDED_


#include "BeadStructure.h"

class CMDBeadStructure : public CBeadStructure  
{
public:
	CMDBeadStructure(zDoubleVector ljDepth, zDoubleVector ljRange,
					 zDoubleVector scDepth, zDoubleVector scRange);
	virtual ~CMDBeadStructure();

	const zDoubleVector& GetLJDepth() const;
	const zDoubleVector& GetLJRange() const;

	const zDoubleVector& GetSCDepth() const;
	const zDoubleVector& GetSCRange() const;

private:
	// The only structure to MD beads is their interactions with other beads:
	// these are Lennard-Jones potentials with a depth and range parameter
	// for each pair of beads. However, we allow for two types of potential 
	// to allow comparison with Goetz and Lipowsky, J Chem Phys 1998.

	zDoubleVector m_vLJDepth;
	zDoubleVector m_vLJRange;
	zDoubleVector m_vSCDepth;
	zDoubleVector m_vSCRange;
};

#endif // !defined(AFX_MDBEADSTRUCTURE_H__2CE23035_D27E_11D3_BF1F_004095086186__INCLUDED_)

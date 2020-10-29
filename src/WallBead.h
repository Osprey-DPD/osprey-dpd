// WallBead.h: interface for the CWallBead class.
//
// This class represents the beads that make up the fixed walls around the
// simulation box if they are specified as part of the boundary conditions.
// A wall exerts a force on all fluid beads that approach it, but the 
// beads that compose the wall are fixed in position. Wall beads may form 
// part of a polymer species and, if so, the other beads in the polymer
// are ordinary CBeads that may move around. 
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WALLBEAD_H__12767EC1_3849_11D3_820E_0060088AD300__INCLUDED_)
#define AFX_WALLBEAD_H__12767EC1_3849_11D3_820E_0060088AD300__INCLUDED_


#include "AbstractBead.h"

class CWallBead : public CAbstractBead  
{
	friend class CPolymer;	// Needs to create wall beads


public:

	virtual ~CWallBead();

	virtual bool SetMovable();
	virtual bool SetNotMovable();
	virtual bool SetFrozen();
	virtual bool SetNotFrozen();

protected:

	CWallBead(CAbstractBead& oldBead);	// Converts a CAbstractBead to a CWallBead

};

#endif // !defined(AFX_WALLBEAD_H__12767EC1_3849_11D3_820E_0060088AD300__INCLUDED_)

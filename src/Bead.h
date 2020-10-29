// Bead.h: interface for the CBead class.
//
// This class represents the fluid beads in a simulation that are free to move
// under the actions of local forces. They are the monomers from which
// polymers are constructed and may be connected by bonds. The CBond class
// is a friend of the CBead class to allow the bond forces to be added
// to the forces on the beads.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BEAD_H__45E558C0_DA21_11D2_B61A_0060088AD300__INCLUDED_)
#define AFX_BEAD_H__45E558C0_DA21_11D2_B61A_0060088AD300__INCLUDED_


#include "AbstractBead.h"

class CBead : public CAbstractBead
{
	// friend classes need access to bead coordinates in order to
	// calculate forces between bonded beads and observables

	friend class CBond;
	friend class CBondPair;
	friend class CCNTCell;
	friend class CMonitor;

public:

	CBead();
	CBead(long type);
	CBead(long id, long type, bool movable, double x0[3], double v0[3]);
	CBead(long id, long type, bool movable, double radius, double x0[3], double v0[3]);
	CBead(const CBead& oldBead);
	virtual ~CBead();

	friend bool operator <( const CBead &a, const CBead &b);
	friend bool operator ==(const CBead &a, const CBead &b);

	virtual bool SetMovable();
	virtual bool SetNotMovable();
	virtual bool SetFrozen();
	virtual bool SetNotFrozen();

private:

};

#endif // !defined(AFX_BEAD_H__45E558C0_DA21_11D2_B61A_0060088AD300__INCLUDED_)

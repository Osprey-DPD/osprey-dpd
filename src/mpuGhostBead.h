// mpuGhostBead.h: interface for the mpuGhostBead class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MPUGHOSTBEAD_H__6F0733AA_9350_4383_AB52_E33AC62242C9__INCLUDED_)
#define AFX_MPUGHOSTBEAD_H__6F0733AA_9350_4383_AB52_E33AC62242C9__INCLUDED_


// Forward declarations

class CAbstractBead;

#include "SimMPSFlags.h"

class mpuGhostBead
{
	// friend classes need access to bead coordinates in order to
	// calculate forces between bonded beads and observables

	friend class CBond;
	friend class CBondPair;
	friend class CCNTCell;
	friend class CMonitor;

	// ****************************************
	// Construction/Destruction: 
public:

	mpuGhostBead();

#if EnableParallelSimBox == SimMPSEnabled
    // Constructor for a ghost that wraps a concrete bead 
	mpuGhostBead(CAbstractBead* const pBead);

    // Constructor for a ghost that wraps a bead that has just moved to a neighbouring processor:
	// we must not alter the passed-in bead pointer so we make it const.
	mpuGhostBead(long newPid, const CAbstractBead* const pBead);
#endif

    // Constructor for a ghost that represents a missing bead belonging to another processor
	mpuGhostBead(long beadId, long polyId);

	mpuGhostBead(const mpuGhostBead& oldBead);

	virtual ~mpuGhostBead();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend bool operator <( const mpuGhostBead &a, const mpuGhostBead &b);
	friend bool operator ==(const mpuGhostBead &a, const mpuGhostBead &b);

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:


	// ****************************************
	// Public access functions
public:

    // Function to return the pid of the processor that owns the bead represented by the ghost bead
	inline long GetPid() const {return m_Pid;}
	
	// Function to return the wrapped bead instance
	inline CAbstractBead* GetBead() const {return m_pBead;}
	
	// Function to show if the ghost is empty
	inline bool IsEmpty() const {return !m_pBead;}

	// Forwarding functions to the wrapped bead instance
	long GetId() const;
	long GetType() const;
	long GetPolymerId() const;
	
	double GetXPos() const;
	double GetYPos() const;
	double GetZPos() const;

	double GetUnPBCXPos() const;
	double GetUnPBCYPos() const;
	double GetUnPBCZPos() const;

    void SetXPos(double x);
    void SetYPos(double y);
    void SetZPos(double z);
	
	void AddXForce(double fx);
	void AddYForce(double fy);
	void AddZForce(double fz);
	void AddForce(double fx, double fy, double fz);
	
	// Function to insert a concrete bead instance into the ghost
	void AddBead(long oldPid, CAbstractBead* const pBead);
	
	// Function that removes the wrapped bead instance after updating the ghost's data
	void RemoveBead(long newPid); 

	// ****************************************
	// Protected local functions
protected:


	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

	CAbstractBead* m_pBead;    // Pointer to the wrapped bead instance when it is still within the owning processor's Space
	
	// Data belonging to a bead that has left the owning processor's Space
	
    long   m_Pid;              // Owning processor's pid	
	long   m_id;               // Data belonging to a bead instance that has left the owning processor's Space
	long   m_Type;             // Bead type
    long   m_PolymerId;        // Owning polymer's id
	bool   m_bIsVisible;
	bool   m_bIsMovable;
	bool   m_bIsFrozen;
	
	double m_Radius;
	
	double m_X;
	double m_Y;               // Coordinates of the ghost bead translated into the Space ofits polymer-owning processor
	double m_Z; 
	

};

#endif // !defined(AFX_MPUGHOSTBEAD_H__6F0733AA_9350_4383_AB52_E33AC62242C9__INCLUDED_)

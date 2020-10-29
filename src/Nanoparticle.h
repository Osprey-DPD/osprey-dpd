// Nanoparticle.h: interface for the CNanoparticle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NANOPARTICLE_H__aecc5fe1_4539_4e64_a482_4a1b9353dd94__INCLUDED_)
#define AFX_NANOPARTICLE_H__aecc5fe1_4539_4e64_a482_4a1b9353dd94__INCLUDED_


// Forward declarations
class CBond;
class mpuExtendedNPBond;

// Include file to gain access to the typedefs for beads and bonds

#include "xxBase.h"
#include "SimMPSFlags.h"



class CNanoparticle : public xxBase
{
	
	// ****************************************
	// Construction/Destruction: 
public:

	CNanoparticle(BondVector vBonds);
	CNanoparticle(const CNanoparticle& oldNanoparticle);

	~CNanoparticle();

	// ****************************************
	// Global functions, static member functions and variables
public:

	static long GetNanoparticleTotal();

private:

	static long m_NanoparticleTotal;	// Number of nanoparticles created

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:


	// ****************************************
	// Public access functions
public:

    inline long GetId()           const {return m_id;}
    inline long GetBondTotal()    const {return m_vBonds.size();}
    long        GetBeadTotal()    const {return m_vBeads.size();}
    bool        IsBorder(double lx, double ly, double lz, double dx, double dy, double dz);
    bool        IsComplete() const;


    inline double GetMaxWidth()   const {return m_MaxWidth;}
    inline double GetXCM()        const {return m_XCM;}
    inline double GetYCM()        const {return m_YCM;}
    inline double GetZCM()        const {return m_ZCM;}

    double GetFirstBondHeadXPos() const;
    double GetFirstBondHeadYPos() const;
    double GetFirstBondHeadZPos() const;

    void AddBond(CBond* const pBond);  
    CBond* RemoveBond(CBond* pBond);  
    CBond* RemoveBond(long bondId);  

    bool IsBeadPresent(long beadId);

    inline BondVector& GetBonds() {return m_vBonds;}
    inline BeadVector& GetBeads() {return m_vBeads;}

    void AddBondForces();    // Function to add the NP's bond forces to their beads

    void CalculateCM();

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

    const long m_id;         // Unique id for the nanoparticle
    double     m_MaxWidth;   // Maximum width of the NP

    double    m_XCM;        // Centre of Mass of the NP
    double    m_YCM;        // Centre of Mass of the NP
    double    m_ZCM;        // Centre of Mass of the NP

    BondVector  m_vBonds;    // Set of bonds used to polymerise the nanoparticle
    BeadVector  m_vBeads;    // Set of concrete beads in the NP's bonds

//    ExtendedNPBondVector m_vExtBonds;  // Set of bonds that cross processor boundaries

};

#endif // !defined(AFX_NANOPARTICLE_H__aecc5fe1_4539_4e64_a482_4a1b9353dd94__INCLUDED_)

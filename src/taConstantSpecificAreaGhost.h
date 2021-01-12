// taConstantSpecificAreaGhost.h: interface for the taConstantSpecificAreaGhost class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TACONSTANTSPECIFICAREAGHOST_H__a7bca50d_8dc4_4726_99c5_ad5ea30a9bdd__INCLUDED_)
#define AFX_TACONSTANTSPECIFICAREAGHOST_H__a7bca50d_8dc4_4726_99c5_ad5ea30a9bdd__INCLUDED_
 


// Forward declarations

class CCommandTargetPolymer;
class ISimState;


#include "taCumulateDecorator.h"

class taConstantSpecificAreaGhost : public taCumulateDecorator
{
public:
	// ****************************************
	// Construction/Destruction
public:

	taConstantSpecificAreaGhost(const zString decLabel, CCommandTargetNode* const pReservoirTarget, CCommandTargetNode* const pMembraneTarget, 
                                ISimState* pISimState,  long beadType, long start, long end, long sampleRate,double targetAN);

	virtual ~taConstantSpecificAreaGhost();

    // ****************************************
	// Global functions, static member functions and variables
public:

    static const zString GetType();		// return the target's type

private:

	static const zString m_Type;

	// ****************************************
	// PVFs that must be implemented by all instantiated derived classes 
public:

    const zString GetTargetType() const;    // return the target's type

    // Implementation of the ISerialiseInclusiveRestartState interface
    // to allow this class to read/write data that can be modified
    // for restarts.

    virtual zInStream& Read(zInStream& is);
    virtual zOutStream& Write(zOutStream& is) const;


	virtual void Execute(long simTime);


	// ****************************************
	// Public access functions
public:



	// ****************************************
	// Protected local functions
protected:


	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:

    void CalculateArea();   // Function to calculate the current membrane area

	// ****************************************
	// Data members

protected:


private:

    long           m_BeadType;                    // Numeric type of the bead in the membrane lipids to use in triangulating the surface area: there must only be a single instance of this bead type in a lipid
    long           m_SampleRate;                  // Number of steps between invocations of this decorator
    double         m_TargetAN;                    // Target area per molecule in the membrane that is to be maintained

    CCommandTargetPolymer*  m_pReservoirTarget;   // Pointer to the reservoir target
    CCommandTargetPolymer*  m_pMembraneTarget;    // Pointer to the membrane target  
    ISimState* const        m_pISimState;         // Pointer to the ISimState to allow us to issue commands

    PolymerVector  m_vBulkReservoir;              // Container for polymers in the bulk solvent
    PolymerVector  m_vMembraneReservoir;          // Container for the polymers in the membrane
    
    BeadVector     m_vGhostLipidBeads;            // Container for the beads in an exemplar ghost lipid       
    BeadVector     m_vMembraneLipidBeads;         // Container for the beads in an exemplar membrane lipid       
    BeadVector     m_vBeads;                      // Container for all the single beads in membrane lipids that are used in triangulating the surface area        
    
    const long     m_DeltaPolymerTotal;           // Number of lipids to transfer between bulk and membrane reservoirs on each invocation (fixed to 1 to avoid accumulation of modified lipids)
    long           m_CurrentPolymerTotal;         // Current number of molecules in the membrane (both initial and ghosts)
    double         m_CurrentArea;                 // Current membrane area
    double         m_DeltaArea;                   // Change in memrane area required to maintain the target A/N
	    
    CPolymer*      m_pGhostLipid;                 // Pointer to a membrane lipid for use when changing ghost lipid types
    CPolymer*      m_pMembraneLipid;              // Pointer to a membrane lipid for use when changing ghost lipid types
    CPolymer*      m_pGhost;                      // Temporary pointer to a ghost polymer moving between the reservoirs
    
    
};

#endif // !defined(AFX_TACONSTANTSPECIFICAREAGHOST_H__a7bca50d_8dc4_4726_99c5_ad5ea30a9bdd__INCLUDED_)

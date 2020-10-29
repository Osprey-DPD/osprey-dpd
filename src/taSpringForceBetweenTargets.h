// taSpringForceBetweenTargets.h: interface for the taSpringForceBetweenTargets class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASPRINGFORCEBETWEENTARGETS_H__2b556d8d_f22f_437e_8004_4fffd79377bd__INCLUDED_)
#define AFX_TASPRINGFORCEBETWEENTARGETS_H__2b556d8d_f22f_437e_8004_4fffd79377bd__INCLUDED_



#include "taForceDecorator.h"

class taSpringForceBetweenTargets : public taForceDecorator
{
public:
	// ****************************************
	// Construction/Destruction
public:

	taSpringForceBetweenTargets(const zString label, CCommandTargetNode* const pDec, CCommandTargetNode* const pTarget2, 
			  	  double mag, double deltax, long period);

    taSpringForceBetweenTargets(const zString label);

	virtual ~taSpringForceBetweenTargets();

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

	virtual double GetXForce(long simTime);
	virtual double GetYForce(long simTime);
	virtual double GetZForce(long simTime);

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

    void CalculateCM(BeadVector vBeads, double* pXCM, double* pYCM, double* pZCM);
    CBead* FindCMBead(BeadVector vBeads);

	// ****************************************
	// Data members

protected:


private:

    CCommandTargetNode* const m_pTarget2;  // Secondary command target to which force is applied
    
	double m_keff;	        // Coefficient of the spring force (= 2*k for harmonic potential)
    double m_DeltaX;        // Step change in the target separation every "m_Period" time steps
    long   m_Period;        // Number of time steps between step changes in the targets' separation
    
    
    CBead*  m_pTarget1Bead; // Bead at or near CM in target 1
    CBead*  m_pTarget2Bead; // Bead at or near CM in target 2
    
    long   m_Counter;       // Counter to know when to increment the searation
    
    double m_Separation;         // Instantaneous separation of the two CM beads in the tar
    double m_TargetSeparation;   // Desired separation of the two beads in each period
    
	double m_FX;
	double m_FY;		    // Force components
	double m_FZ;

	// Local data

	double m_RelOX;
	double m_RelOY;			// Bead position vector relative to force origin
	double m_RelOZ;

};

#endif // !defined(AFX_TASPRINGFORCEBETWEENTARGETS_H__2b556d8d_f22f_437e_8004_4fffd79377bd__INCLUDED_)

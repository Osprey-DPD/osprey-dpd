// aeReceptorNetwork.h: interface for the aeReceptorNetwork class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AERECEPTORNETWORK_H__1b9473ab_fb92_498a_ba97_78b5e908a751__INCLUDED_)
#define AFX_AERECEPTORNETWORK_H__1b9473ab_fb92_498a_ba97_78b5e908a751__INCLUDED_



#include "IModifyReceptorNetwork.h"

class aeReceptorNetwork : public IModifyReceptorNetwork
{
	// ****************************************
	// Construction/Destruction
public:

	aeReceptorNetwork(IActiveSimBox* const pShadow, long xn, long yn, long zn,
					double xw, double yw, double zw);

	virtual ~aeReceptorNetwork();

	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();		// return the ACN name

private:

	static const zString m_Type;		// Identifier used to route commands

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	virtual void Evolve();

	virtual const zString GetACNType() const;

	void AttachProcess(xxProcess* pProcess);

	// Functions to modify the network

	virtual void ActivateReceptorNucleation();
	virtual void InactivateReceptorNucleation();
	virtual void DisassembleReceptorFilaments();
	virtual void SeverReceptorFilaments();

	virtual void SetFixedHeadOffRate(double rate);
	virtual void SetFixedTailOffRate(double rate);

	virtual void SetProximityOnSeparation(double max);
	virtual void SetProximityOffSeparation(double max);
	virtual void SetProximityHeadOnSeparation(double max);
	virtual void SetProximityHeadOffSeparation(double max);
	virtual void SetProximityTailOnSeparation(double max);
	virtual void SetProximityTailOffSeparation(double max);


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

	void Nucleate();		// Function to try and create new active polymers out of a receptor and a ligand
	
	void BindNext();        // Function to try and bind another ligand to an existing activated receptor


	// ****************************************
	// Data members
private:

	bool m_bNucleationActive;	// Flag showing if new receptor-ligand bond formation is allowed

};


#endif // !defined(AFX_AERECEPTORNETWORK_H__1b9473ab_fb92_498a_ba97_78b5e908a751__INCLUDED_)

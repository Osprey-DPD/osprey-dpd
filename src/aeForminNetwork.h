// aeForminNetwork.h: interface for the aeForminNetwork class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AEFORMINNETWORK_H__D48C4D04_B4B4_4359_9A42_B8A2EF63EDC7__INCLUDED_)
#define AFX_AEFORMINNETWORK_H__D48C4D04_B4B4_4359_9A42_B8A2EF63EDC7__INCLUDED_



#include "IModifyForminNetwork.h"

class aeForminNetwork : public IModifyForminNetwork
{
	// ****************************************
	// Construction/Destruction
public:

	aeForminNetwork(IActiveSimBox* const pShadow, long xn, long yn, long zn,
					double xw, double yw, double zw);

	virtual ~aeForminNetwork();

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

	virtual void ActivateForminNucleation();
	virtual void InactivateForminNucleation();
	virtual void DisassembleForminFilaments();
	virtual void SeverForminFilaments();

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

	void Nucleate();		// Function to try and create new active polymers
	void Transfer();		// Function to transfer an mActin from Tail to Head


	// ****************************************
	// Data members
private:

	bool m_bNucleationActive;	// Flag showing if new filament formation is allowed	

};

#endif // !defined(AFX_AEFORMINNETWORK_H__D48C4D04_B4B4_4359_9A42_B8A2EF63EDC7__INCLUDED_)

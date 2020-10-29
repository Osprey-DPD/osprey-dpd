// aefActinNetwork.h: interface for the aefActinNetwork class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AEFACTINNETWORK_H__EB0C848C_39D8_469E_A644_ED05A72E591F__INCLUDED_)
#define AFX_AEFACTINNETWORK_H__EB0C848C_39D8_469E_A644_ED05A72E591F__INCLUDED_



#include "IModifyfActinNetwork.h"

class aefActinNetwork : public IModifyfActinNetwork
{
	// Declare the access control classes that can modify this network

	friend class IfActinAccess;


	// ****************************************
	// Construction/Destruction
public:

	aefActinNetwork(IActiveSimBox* const pShadow, long xn, long yn, long zn,
					double xw, double yw, double zw);

	virtual ~aefActinNetwork();

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

	virtual void AttachProcess(xxProcess* pProcess);

// Functions to modify the network

	virtual void SetFixedHeadOffRate(double rate);
	virtual void SetFixedTailOffRate(double rate);

	virtual void SetProximityOnSeparation(double max);
	virtual void SetProximityOffSeparation(double max);
	virtual void SetProximityHeadOnSeparation(double max);
	virtual void SetProximityHeadOffSeparation(double max);
	virtual void SetProximityTailOnSeparation(double max);
	virtual void SetProximityTailOffSeparation(double max);

	virtual void SetATPHydrolysisProbability(double rate);
	virtual void SetADPReleasePiProbability(double rate);
	virtual void SetADPPhosphorylationProbability(double rate);

	virtual void SetHeadADPPiMultiplier(double factor);
    virtual	void SetTailADPPiMultiplier(double factor);
	virtual void SetHeadADPMultiplier(double factor);
	virtual void SetTailADPMultiplier(double factor);

	virtual void ColourBondsByATPState(long ATPId, long ADPPiId, long ADPId);

	// ****************************************
	// Public access functions
public:

	// ****************************************
	// Protected local functions
protected:

	void UpdateBondDisplayStatus();

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:

	void Nucleate();		// Function to try and create new active polymers


	// ****************************************
	// Data members
private:

    bool m_bRefreshDisplayStatus; // Flag showing if the ACN's bonds should have their display status refreshed

    long m_ATPDisplayId;    // Display Id for monomers with bound ATP
    long m_ADPPiDisplayId;  //    "                               ADP-Pi
    long m_ADPDisplayId;    //    "                               ADP


};

#endif // !defined(AFX_AEFACTINNETWORK_H__EB0C848C_39D8_469E_A644_ED05A72E591F__INCLUDED_)

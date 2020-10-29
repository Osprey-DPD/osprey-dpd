// aefActinBond.h: interface for the aefActinBond class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AEFACTINBOND_H__A4724BFC_477C_4B32_8284_8AF6D778829B__INCLUDED_)
#define AFX_AEFACTINBOND_H__A4724BFC_477C_4B32_8284_8AF6D778829B__INCLUDED_



#include "aeActiveBond.h"

class aefActinBond : public aeActiveBond  
{
    friend class aevBondUnbindsFromPolymerHead;
    friend class aevBondUnbindsFromPolymerTail;
    friend class aevPolymerDissolves;


	// ****************************************
	// Construction/Destruction
public:

	aefActinBond(CPolymer* const pTail, double k2, double l0, double k3, double phi0);
	virtual ~aefActinBond();


	// ****************************************
	// Global functions, static member functions and variables
public:

    static void SetATPHydrolysisProbability(double prob);
	static void SetADPReleasePiProbability(double prob);
	static void SetADPPhosphorylationProbability(double prob);

	static void SetHeadADPPiMultiplier(double factor);
	static void SetTailADPPiMultiplier(double factor);
	static void SetHeadADPMultiplier(double factor);
	static void SetTailADPMultiplier(double factor);

private:

	static long	m_ActiveBondsPerPolymer;

    static double m_ATPHydrolysisProb;       // Probability of ATP hydrolysis
    static double m_ADPReleasePiProb;        // Probability of releasing Pi
    static double m_ADPPhosphorylationProb;  // Probability of phosphorylation

    static double m_HeadBasalOffRate;         // Probability for ATP monomer to detach from filament head
    static double m_TailBasalOffRate;         // Probability for ATP monomer to detach from filament tail
    static double m_HeadADPPiMultiplier;      // Multiplier applied to basal rate when ATP is hydrolysed at head
    static double m_TailADPPiMultiplier;      // Multiplier applied to basal rate when ATP is hydrolysed at tail
    static double m_HeadADPMultiplier;        // Multiplier applied to basal rate when Pi is released at head
    static double m_TailADPMultiplier;        // Multiplier applied to basal rate when Pi is released at tail

    // ****************************************
	// PVFs that must be overridden by all derived classes
public:

	virtual long GetActiveBondsPerPolymer() const;
	virtual bool Activate(aeActiveBond* const pTargetBond);
	virtual bool Deactivate();

	// ****************************************
	// Public access functions
public:

    void SetFixedHeadOffRate(double rate);
	void SetFixedTailOffRate(double rate);
	void SetCurrentTailOffRate(double rate);
	void SetCurrentHeadOffRate(double rate);

	void SetOnRateSeparation(double max);
	void SetOffRateSeparation(double max);
	void SetHeadOnRateSeparation(double max);
	void SetHeadOffRateSeparation(double max);
	void SetTailOnRateSeparation(double max);
	void SetTailOffRateSeparation(double max);


	// Functions giving access to the bond's internal state


	bool IsATPBound() const {return m_bATP;}
	bool IsPiBound()  const {return m_bPi;}


    // Functions modifying the bond's internal state

	bool Hydrolyse();		// ATP    --> ADP-Pi
	bool ReleasePi();		// ADP-Pi --> ADP
	bool Phosphorylate();	// ADP    --> ATP

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


	// Data representing the bond's internal state

	bool m_bATP;	// Monomer has bound ATP
	bool m_bPi;		// Monomer has bound ADP-Pi

    double m_HeadCurrentOffRate;  // Current detachment probability at a bond's head end
    double m_TailCurrentOffRate;  // Current detachment probability at a bond's tail end

};

#endif // !defined(AFX_AEFACTINBOND_H__A4724BFC_477C_4B32_8284_8AF6D778829B__INCLUDED_)

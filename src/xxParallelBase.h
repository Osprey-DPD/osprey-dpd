// xxParallelBase.h: interface for the xxParallelBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXPARALLELBASE_H__D3A4CFA9_3681_4B7C_8A33_2A6A7C866296__INCLUDED_)
#define AFX_XXPARALLELBASE_H__D3A4CFA9_3681_4B7C_8A33_2A6A7C866296__INCLUDED_


// Include file that allows derived classes to distinguish between the standard
// code and the parallel version.

#include "SimMPSFlags.h"

#include "xxBase.h"

class xxParallelBase : public xxBase
{
public:
	// ****************************************
	// Construction/Destruction: protected constructor declared below
public:

	virtual ~xxParallelBase();

	// ****************************************
	// Global functions, static member functions and variables
public:

    // Functions returning information about a parallel experiment

    static bool GlobalIsParallel();
    static long GlobalGetWorld();   
    static long GlobalGetRank();   

    static double GlobalGetSimBoxXOrigin();
    static double GlobalGetSimBoxYOrigin();
    static double GlobalGetSimBoxZOrigin();

protected:

    static long m_World;  // Number of processors used for a parallel experiment
    static long m_Rank;   // Rank of this instance of the program

    static double m_SimBoxXOrigin;
    static double m_SimBoxYOrigin;  // Coordinates of processor's origin in Space
    static double m_SimBoxZOrigin;


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	

	// ****************************************
	// Public access functions
public:

    // Member functions returning information about a parallel experiment

	bool IsParallel() const;
	bool IsProcessZero() const;

	long GetWorld() const;
	long GetRank() const;
	
	double GetSimBoxXOrigin() const;
	double GetSimBoxYOrigin() const;
	double GetSimBoxZOrigin() const;

	// Functions used to identify the processor that sends/receives a message and allow loops over processors
	inline long       GetProcId() const {return m_ProcId;}
	inline long GetReceiverRank() const {return m_ReceiverRank;}
	inline long   GetSenderRank() const {return m_SenderRank;}

	void SetSenderRank(long pid);


	// ****************************************
	// Protected local functions
protected:

	// Protected constructor ensures that only subclasses can be instantiated

	xxParallelBase();

    xxParallelBase(const xxParallelBase& oldBase);

	// ****************************************
	// Implementation
	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
protected:

    long m_ProcId;        // Counter used in loops over processors in derived classes
    long m_ReceiverRank;  // Rank of receiving processor
    long m_SenderRank;    // Rank of sending processor

};

#endif // !defined(AFX_XXPARALLELBASE_H__D3A4CFA9_3681_4B7C_8A33_2A6A7C866296__INCLUDED_)

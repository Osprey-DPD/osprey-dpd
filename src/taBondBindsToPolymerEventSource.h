// taBondBindsToPolymerEventSource.h: interface for the taBondBindsToPolymerEventSource class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABONDBINDSTOPOLYMEREVENTSOURCE_H__D295CD41_51D7_456D_8B4B_9448FC2092A8__INCLUDED_)
#define AFX_TABONDBINDSTOPOLYMEREVENTSOURCE_H__D295CD41_51D7_456D_8B4B_9448FC2092A8__INCLUDED_



// Forward declarations

class aeActiveBond;
class aevActiveEvent;


#include "taEventSourceDecorator.h"

class taBondBindsToPolymerEventSource : public taEventSourceDecorator
{
public:
	// ****************************************
	// Construction/Destruction
public:

    taBondBindsToPolymerEventSource(const zString label, CCommandTargetNode* const pDec);

	virtual ~taBondBindsToPolymerEventSource();

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

	// ****************************************

	// Functions used by other decorator classes

    virtual void Execute(long simTime);

    // Function used by an ACN to pass event data for analysis

    virtual void BroadcastEventState(const aevActiveEvent* const pEvent);


    // ****************************************
    // IEADSerialiseBondCoordinates interface

    // Interface used by  Event Analysis Decorators to get access to the
    // data stream relating to an active bond.

public:

    virtual double GetXCoordinate() const;
    virtual double GetYCoordinate() const;
    virtual double GetZCoordinate() const;


	// ****************************************
	// Public access functions


protected:

	void SetBond(const aeActiveBond* const pBond);


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

protected:

private:

    const aeActiveBond* m_pBond;  // Pointer to an active bond belonging to an active polymer

};

#endif // !defined(AFX_TABONDBINDSTOPOLYMEREVENTSOURCE_H__D295CD41_51D7_456D_8B4B_9448FC2092A8__INCLUDED_)

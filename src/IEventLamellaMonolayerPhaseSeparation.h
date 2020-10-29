// IEventLamellaMonolayerPhaseSeparation.h: interface for the IEventLamellaMonolayerPhaseSeparation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IEVENTLAMELLAMONOLAYERPHASESEPARATION_H__6E9520ED_8944_46FA_8B26_B1A819F36244__INCLUDED_)
#define AFX_IEVENTLAMELLAMONOLAYERPHASESEPARATION_H__6E9520ED_8944_46FA_8B26_B1A819F36244__INCLUDED_


class IEventLamellaMonolayerPhaseSeparation  
{
public:
	// Member function indicating that a derived class supports this event

	virtual bool LamellaMonolayerPhaseSeparationSupported() const;

protected:

    virtual ~IEventLamellaMonolayerPhaseSeparation();


public:
	// Access functions that must be overridden by derived classes that support this event

	virtual bool HasPhaseSeparated() const;

};

#endif // !defined(AFX_IEVENTLAMELLAMONOLAYERPHASESEPARATION_H__6E9520ED_8944_46FA_8B26_B1A819F36244__INCLUDED_)

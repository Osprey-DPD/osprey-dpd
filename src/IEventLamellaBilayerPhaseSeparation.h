// IEventLamellaBilayerPhaseSeparation.h: interface for the IEventLamellaBilayerPhaseSeparation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IEVENTLAMELLABILAYERPHASESEPARATION_H__D51B3E41_9D2A_40FF_B7E8_D4A36B25086A__INCLUDED_)
#define AFX_IEVENTLAMELLABILAYERPHASESEPARATION_H__D51B3E41_9D2A_40FF_B7E8_D4A36B25086A__INCLUDED_


class IEventLamellaBilayerPhaseSeparation  
{
public:
	// Member function indicating that a derived class supports this event

	virtual bool LamellaBilayerPhaseSeparationSupported() const;

protected:

    virtual ~IEventLamellaBilayerPhaseSeparation();


public:
	// Access functions that must be overridden by derived classes that support this event

};

#endif // !defined(AFX_IEVENTLAMELLABILAYERPHASESEPARATION_H__D51B3E41_9D2A_40FF_B7E8_D4A36B25086A__INCLUDED_)

// IBilayerFusion.h: interface for the IBilayerFusion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IBILAYERFUSION_H__493C96BD_62C6_48E9_9BC5_C5874B8C9D73__INCLUDED_)
#define AFX_IBILAYERFUSION_H__493C96BD_62C6_48E9_9BC5_C5874B8C9D73__INCLUDED_


template <class T>
class IBilayerFusion  
{
public:
	IBilayerFusion(const T* const pBilayer): m_pBilayer(pBilayer) {};
	~IBilayerFusion() {};

	// Access functions to the wrapped CBilayer object

double GetCMXPos()				const {return m_pBilayer->GetCMXPos();}
double GetCMYPos()				const {return m_pBilayer->GetCMYPos();}
double GetCMZPos()				const {return m_pBilayer->GetCMZPos();}

short int   GetXNormal()		const {return static_cast<short int>(m_pBilayer->GetXNormal());}
short int   GetYNormal()		const {return static_cast<short int>(m_pBilayer->GetYNormal());}
short int   GetZNormal()		const {return static_cast<short int>(m_pBilayer->GetZNormal());}

long   GetMajorLipidType()		const {return m_pBilayer->GetMajorLipidType();}
long   GetMajorLipidHeadType()	const {return m_pBilayer->GetMajorLipidHeadType();}
long   GetMajorLipidTailType()	const {return m_pBilayer->GetMajorLipidTailType();}

double GetBilayerHeight()		const {return m_pBilayer->GetBilayerHeight();}
double GetInnerHeadHeight()		const {return m_pBilayer->GetInnerHeadHeight();}
double GetOuterHeadHeight()		const {return m_pBilayer->GetOuterHeadHeight();}
double GetInnerTailHeight()		const {return m_pBilayer->GetInnerTailHeight();}
double GetOuterTailHeight()		const {return m_pBilayer->GetOuterTailHeight();}

bool   IsPoreOpen()				const {return m_pBilayer->IsPoreOpen();}
long   GetPoreSize()			const {return m_pBilayer->GetPoreSize();}

// Storing the aggregate pointer as a pointer to T allows multiple versions 
// of bilayer aggregates to be contained in this template class.
// But such aggregates are all assumed to be some kind of planar bilayer membrane.


private:
	const T* const m_pBilayer;
};

#endif // !defined(AFX_IBILAYERFUSION_H__493C96BD_62C6_48E9_9BC5_C5874B8C9D73__INCLUDED_)

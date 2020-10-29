// IBilayerRupture.h: interface for the IBilayerRupture class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IBILAYERRUPTURE_H__E21CB63C_3C9B_4FA3_8929_5D847081412A__INCLUDED_)
#define AFX_IBILAYERRUPTURE_H__E21CB63C_3C9B_4FA3_8929_5D847081412A__INCLUDED_


template <class T>
class IBilayerRupture  
{
public:
	IBilayerRupture(const T* const pBilayer): m_pBilayer(pBilayer) {};
	~IBilayerRupture() {};

	// Access functions to the wrapped CBilayer object

double GetCMXPos()				const {return m_pBilayer->GetCMXPos();}
double GetCMYPos()				const {return m_pBilayer->GetCMYPos();}
double GetCMZPos()				const {return m_pBilayer->GetCMZPos();}

long   GetXNormal()				const {return m_pBilayer->GetXNormal();}
long   GetYNormal()				const {return m_pBilayer->GetYNormal();}
long   GetZNormal()				const {return m_pBilayer->GetZNormal();}

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
// But such aggregates are all assumed to be some kind of planar bilayer membrane
// and must implement the above functions.


private:
	const T* const m_pBilayer;
};

#endif // !defined(AFX_IBILAYERRUPTURE_H__E21CB63C_3C9B_4FA3_8929_5D847081412A__INCLUDED_)



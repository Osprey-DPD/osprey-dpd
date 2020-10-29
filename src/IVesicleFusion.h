// IVesicleFusion.h: interface for the IVesicleFusion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IVESICLEFUSION_H__B359AEEB_5149_4916_A257_B622DDAD7D17__INCLUDED_)
#define AFX_IVESICLEFUSION_H__B359AEEB_5149_4916_A257_B622DDAD7D17__INCLUDED_


template <class T>
class IVesicleFusion  
{
public:
	IVesicleFusion(const T* const pVesicle): m_pVesicle(pVesicle) {};
	~IVesicleFusion() {};

	// Access functions to the wrapped CVesicle object

double GetCMXPos() const {return m_pVesicle->GetCMXPos();}
double GetCMYPos() const {return m_pVesicle->GetCMYPos();}
double GetCMZPos() const {return m_pVesicle->GetCMZPos();}
double GetRadius() const {return m_pVesicle->GetRadius();}

long   GetMajorLipidType()	   const {return m_pVesicle->GetMajorLipidType();}
long   GetMajorLipidHeadType() const {return m_pVesicle->GetMajorLipidHeadType();}
long   GetMajorLipidTailType() const {return m_pVesicle->GetMajorLipidTailType();}



// Storing the aggregate pointer as a pointer to T allows multiple versions 
// of vesicle aggregates to be contained in this template class.
// But such aggregates are all assumed to be some kind of quasi-spherical vesicle.

private:
	const T* const m_pVesicle;
};

#endif // !defined(AFX_IVESICLEFUSION_H__B359AEEB_5149_4916_A257_B622DDAD7D17__INCLUDED_)

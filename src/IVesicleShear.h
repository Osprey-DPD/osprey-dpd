// IVesicleShear.h: interface for the IVesicleShear class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IVESICLESHEAR_H__73C2303B_9C9B_4804_82C5_E3B78CC4E82D__INCLUDED_)
#define AFX_IVESICLESHEAR_H__73C2303B_9C9B_4804_82C5_E3B78CC4E82D__INCLUDED_


template <class T>
class IVesicleShear  
{
public:
	IVesicleShear(const T* const pVesicle): m_pVesicle(pVesicle) {};
	~IVesicleShear() {};


	// Access functions to the wrapped CVesicle object

double GetCMXPos() const {return m_pVesicle->GetCMXPos();}
double GetCMYPos() const {return m_pVesicle->GetCMYPos();}
double GetCMZPos() const {return m_pVesicle->GetCMZPos();}
double GetRadius() const {return m_pVesicle->GetRadius();}


// Storing the aggregate pointer as a pointer to T allows multiple versions 
// of vesicle aggregates to be contained in this template class.
// But such aggregates are all assumed to be some kind of quasi-spherical vesicle.

private:
	const T* const m_pVesicle;

};

#endif // !defined(AFX_IVESICLESHEAR_H__73C2303B_9C9B_4804_82C5_E3B78CC4E82D__INCLUDED_)

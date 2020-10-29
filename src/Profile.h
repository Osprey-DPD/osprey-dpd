// Profile.h: interface for the CProfile<> class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROFILE_H__C2278A16_AC86_11D3_BF15_004095086186__INCLUDED_)
#define AFX_PROFILE_H__C2278A16_AC86_11D3_BF15_004095086186__INCLUDED_


// Include file to gain access to the STL container classes and the xxBasevector
// class that adds member functions not provided by some compilers.

#include "xxBase.h"

// **********************************************************************
// I removed the "using namespapce std" and qualified the STL elements here
// as I don't want to include all the standard components in a header file.
//
// I also moved the constructor and destructor definitions into the class
// definitions to avoid having to move them from the .h to the .cpp file 
// when porting the code to the Dec Alpha machines.
//
// **********************************************************************


template <class R> class CProfile 
{
public:
    // Typedef needed to avoid errors using the g++ compiler:
    // "typename X is implicitly a typename. Implicit typename is deprecated."

    typedef typename xxBasevector<R*>::iterator xxBasevectorIterator;

	virtual ~CProfile()
	{
		// Delete the vector of CRegion-derived objects if they were created

		if(m_vRegions.size() > 0)
		{
			for(xxBasevectorIterator iterR=m_vRegions.begin(); iterR!=m_vRegions.end(); iterR++)
			{
				delete *iterR;
			}
			m_vRegions.clear();
		}
	}


	// Access functions
	//
	// The CRegion-derived objects stored here must be created on the heap 
	// because they will be destroyed when the CProfile's destructor is called.

	inline long Size() const                    {return m_vRegions.size();}
	inline void AddRegion(R* pRegion)           {m_vRegions.push_back(pRegion);}
	inline R* GetRegion(long index)				{return m_vRegions.at(index);}
	inline xxBasevectorIterator Begin()	        {return m_vRegions.begin();}
	inline xxBasevectorIterator End()		    {return m_vRegions.end();}


	// Protected constructor ensures that only the derived classes can create
	// the template class objects

protected:
	CProfile()
	{
		// Not all derived classes use the same data for initialisation
		// (CNTCells, CSlice or CRow) so we don't want to do any specific data 
		// allocation here.

		m_vRegions.clear();
	}


private:
	xxBasevector<R*> m_vRegions;		// Vector of CRegion-derived object pointers

};

#endif // !defined(AFX_PROFILE_H__C2278A16_AC86_11D3_BF15_004095086186__INCLUDED_)



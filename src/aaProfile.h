// aaProfile.h: interface for the aaProfile<> class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AAPROFILE_H__7F4A21B8_B13C_11D3_BF19_004095086186__INCLUDED_)
#define AFX_AAPROFILE_H__7F4A21B8_B13C_11D3_BF19_004095086186__INCLUDED_


// Include file to gain access to the STL container classes and the xxBasevector class
// that adds member functions missing from some compiler implementations.

#include "xxBase.h"


template <class T> class aaProfile
{
public:
    // Typedef needed to avoid errors using the g++ compiler:
    // "typename X is implicitly a typename. Implicit typename is deprecated."

    typedef typename xxBasevector<T*>::iterator xxBasevectorIterator;

	virtual ~aaProfile()
	{
		// Delete the data if it exists 

		for(xxBasevectorIterator iterT=m_vData.begin(); iterT!=m_vData.end(); iterT++)
			{
				if(*iterT)
					delete *iterT;
			}
			m_vData.clear();
	}

	inline long Size() const {return m_vData.size();}

	inline xxBasevectorIterator Begin() {return m_vData.begin();}
	inline xxBasevectorIterator End()   {return m_vData.end();}

	// Protected constructor and SetSize(), GetDataSet(), DeleteData() 
	// functions ensure that only derived classes can be created and 
	// add or remove data except using the Begin() and End() functions.
	// Note that DeleteData() only deletes the data but keeps the vector's
	// size the same.

protected:
	aaProfile(long size)
	{
		m_vData.resize(size);
	}

	inline const xxBasevector<T*>& GetDataSet() const {return m_vData;}

	// Function to delete the data stored in the profile but leave the size
	// of the vector unchanged so that the function objects can insert their
	// data into it.

	inline void DeleteData()
	{
		for(xxBasevectorIterator iterT=m_vData.begin(); iterT!=m_vData.end();iterT++)
		{
			if(*iterT)
			{
				delete *iterT;
				*iterT = NULL;
			}
		}
	}

private:
	xxBasevector<T*> m_vData;		// Vector of data object pointers

};

#endif // !defined(AFX_AAPROFILE_H__7F4A21B8_B13C_11D3_BF19_004095086186__INCLUDED_)


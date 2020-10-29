// aaSingle.h: interface for the aaSingle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AASINGLE_H__7F4A21BC_B13C_11D3_BF19_004095086186__INCLUDED_)
#define AFX_AASINGLE_H__7F4A21BC_B13C_11D3_BF19_004095086186__INCLUDED_


//////////////////////////////////////////////////////////////////////
// Because this is a template class we have to include the function 
// definitions with the class definition so that they are visible to 
// any other class that uses them.
//
//////////////////////////////////////////////////////////////////////

template <class T> class aaSingle
{
public:
	virtual ~aaSingle() {}

protected:
	aaSingle(const T& value) : m_Data(value) {}

	// "Set" here is a misnomer as there is a single data item.
	// We use it for compatibility with the aaProfile classes that
	// do hold more than one item

	inline const T& GetDataSet() const {return m_Data;}

private:
	const T m_Data;

};

#endif // !defined(AFX_AASINGLE_H__7F4A21BC_B13C_11D3_BF19_004095086186__INCLUDED_)




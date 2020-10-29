// xxTool.h: interface for the xxTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXTOOL_H__8C722835_CAD4_4188_BE49_D84CF3C58144__INCLUDED_)
#define AFX_XXTOOL_H__8C722835_CAD4_4188_BE49_D84CF3C58144__INCLUDED_

class xxTool
{
public:
	// ****************************************
	// Construction/Destruction

	virtual ~xxTool();



	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be implemented by all instantiated derived classes 
public:


	// ****************************************
	// Public access functions
public:


	// ****************************************
	// Protected local functions: protected constructor ensures only derived
	// classes can be instantiated

protected:

	xxTool();

	// Explicitly disallow the copy constructor and assignment operators
	// by declaring them private and providing NO definitions.

	xxTool(const xxTool& xxTool);
	xxTool& operator=(const xxTool& rhs);

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members

protected:



private:

};

#endif // !defined(AFX_XXTOOL_H__8C722835_CAD4_4188_BE49_D84CF3C58144__INCLUDED_)

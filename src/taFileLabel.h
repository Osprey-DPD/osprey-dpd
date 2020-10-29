// taFileLabel.h: interface for the taFileLabel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TAFILELABEL_H__BD4A81C9_F403_437C_BD9D_04D2D4BBA5F3__INCLUDED_)
#define AFX_TAFILELABEL_H__BD4A81C9_F403_437C_BD9D_04D2D4BBA5F3__INCLUDED_



#include "taLabelDecorator.h"
#include "xxFile.h"

class taFileLabel : public taLabelDecorator, public xxFile
{
public:
	// ****************************************
	// Construction/Destruction - protected constructor declared below
public:

	virtual ~taFileLabel();


	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be implemented by all instantiated derived classes 
public:

    virtual bool Serialize() = 0;


	// ****************************************
	// Public access functions
public:


	// ****************************************
	// Protected local functions
protected:

    // Protected constructor ensures that only derived classes can be
    // created. This class only adds the ability to write data to a file
    // to an existing label.

	taFileLabel(const zString fileName, const zString label, 
                CCommandTargetNode* const pDec);


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

#endif // !defined(AFX_TAFILELABEL_H__BD4A81C9_F403_437C_BD9D_04D2D4BBA5F3__INCLUDED_)

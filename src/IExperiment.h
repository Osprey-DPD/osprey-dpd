// IExperiment.h: interface for the IExperiment class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IEXPERIMENT_H__94763FA4_8246_4ABD_A127_CC8C28E93AA7__INCLUDED_)
#define AFX_IEXPERIMENT_H__94763FA4_8246_4ABD_A127_CC8C28E93AA7__INCLUDED_


// Forward declarations

class ISecureExperiment;
class IDocumentHandler;
class ICommandDocument;
class IProtocolDocument;


#include "xxParallelBase.h"

class IExperiment : public xxParallelBase  
{
	// Main interface implemented by the experiment object

	// ****************************************
	// Construction/Destruction:
public:


	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:


	virtual bool ReadProtocolDocument() = 0;
	virtual bool RunPrevis(StringSequence excludedPolymers) = 0;
	virtual bool Run() = 0;

	virtual ISecureExperiment* GetISecureExperiment() = 0;
	virtual IDocumentHandler*  GetIDocumentHandler()  = 0;
	virtual ICommandDocument*  GetICommandDocument()  = 0;
	virtual IProtocolDocument* GetIProtocolDocument() = 0;

	// ****************************************
	// Public access functions
public:


	// ****************************************
	// Protected local functions
protected:

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

};

#endif // !defined(AFX_IEXPERIMENT_H__94763FA4_8246_4ABD_A127_CC8C28E93AA7__INCLUDED_)

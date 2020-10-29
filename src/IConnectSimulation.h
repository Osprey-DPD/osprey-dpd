// IConnectSimulation.h: interface for the IConnectSimulation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICONNECTSIMULATION_H__33B8F4C4_E3CD_4165_9D00_F5506780C68B__INCLUDED_)
#define AFX_ICONNECTSIMULATION_H__33B8F4C4_E3CD_4165_9D00_F5506780C68B__INCLUDED_


// Forward declarations

class IExperiment;
class IDocumentHandler;


class IConnectSimulation  
{
	// ****************************************
	// Construction/Destruction: no ctor as this is an interface class
public:


	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	virtual IExperiment* IIExperiment() const = 0;

	virtual IDocumentHandler* GetIDocumentHandler() = 0;

	// ****************************************
	// Public access functions
public:


	// ****************************************
	// Protected local functions
protected:

	virtual ~IConnectSimulation();

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

};

#endif // !defined(AFX_ICONNECTSIMULATION_H__33B8F4C4_E3CD_4165_9D00_F5506780C68B__INCLUDED_)

/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// InputDPDLGFile.cpp: implementation of the CInputDPDLGFile class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMPSFlags.h"
#include "InputDPDLGFile.h"
#include "BeadType.h"
#include "BondType.h"
#include "BondPairType.h"
#include "PolymerType.h"
#include "InitialStateObject.h"
#include "InitialStateData.h"
#include "GravityData.h"
#include "WallData.h"
#include "AnalysisObject.h"
#include "xxProcessObject.h"
#include "xxEventObject.h"
#include "xxCommandObject.h"
#include "DPDBeadStructure.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//
// To create a CInputDPDLGFile object we must supply a filename and a flag
// showing whether we are reading (IOFlag = false) or writing (IOFlag = true)
// to the file, and a valid CInitialStateData-derived object pointer.
//
// If the control data is being written to file the reference rISD is used
// and no new pISD is created. But if control data is being read then 
// the a new pISD is created and filled with the new data. This is then
// used by the CInputData object to replace the original initial state pointer.
// If a read error occurs, the pISD is deleted if it has been created.
//
// We allow both standard DPD and DPDLG parameters to be serialised by this file.
// The distinction is propagated to the rest of the simulation via the
// IsLG() function that returns true for an LG run. This flag can only be
// used in conjunction with the EnableDPDLG flag in ExperimentDefs.h, and
// is only read in from a CDF: the default CDF is always a standard one.
	
CInputDPDLGFile::CInputDPDLGFile(zString iFileName, bool IOFlag, 
							 const CInitialStateData& rISD) : xxFile(iFileName, IOFlag),
															  pISD(NULL),
															  pGD(NULL),
															  pWD(NULL),
															  m_rISD(rISD),
															  m_pISO(NULL),
                                                              m_bLG(false)
{
	vProcessObjects.clear();
	vAnalysisObjects.clear();
	vEventObjects.clear();
	vCommandObjects.clear();

	mNewBeadTypes.clear();
	mNewBondTypes.clear();
	mNewBondPairTypes.clear();
	mNewPolymerTypes.clear();

	// Zero the static counters for bead, bond, bondpair and polymer types stored 
	// in the CBeadType, CBondType, CBondPairType and CPolymerType classes. 
	// This is to ensure that separate simulations start counting the objects 
	// they create from 0.

	CBeadType::ZeroTotal();
	CBondType::ZeroTotal();
	CBondPairType::ZeroTotal();
	CPolymerType::ZeroTotal();
}

CInputDPDLGFile::~CInputDPDLGFile()
{

	// delete the bead, bond, bondpair and polymer type objects that were stored in maps

	for(LongBeadTypeIterator iterBeadType = mNewBeadTypes.begin(); iterBeadType != mNewBeadTypes.end(); iterBeadType++)
	{
		CBeadType* pBead = (*iterBeadType).second;
		delete pBead;
	}

	for(LongBondTypeIterator iterBondType = mNewBondTypes.begin(); iterBondType != mNewBondTypes.end(); iterBondType++)
	{
		CBondType* pBond = (*iterBondType).second;
		delete pBond;
	}

	for(LongBondPairTypeIterator iterBondPairType = mNewBondPairTypes.begin(); iterBondPairType != mNewBondPairTypes.end(); iterBondPairType++)
	{
		CBondPairType* pBondPair = (*iterBondPairType).second;
		delete pBondPair;
	}

	for(LongPolymerTypeIterator iterPolymerType = mNewPolymerTypes.begin(); iterPolymerType != mNewPolymerTypes.end(); iterPolymerType++)
	{
		CPolymerType* pPolymer = (*iterPolymerType).second;
		delete pPolymer;
	}

	// If there is a CGravityData object delete it

	if(pGD)
	{
		delete pGD;
		pGD = NULL;
	}

	// and if there is a CWallData object delete it

	if(pWD)
	{
		delete pWD;
		pWD = NULL;
	}

	// if any xxProcessObjects have been created we delete them here

	if(!vProcessObjects.empty())
	{
		for(ProcessObjectIterator iterProc=vProcessObjects.begin(); iterProc!=vProcessObjects.end(); iterProc++)
		{
			delete *iterProc;
		}
		vProcessObjects.clear();
	}

	// CAnalysisObjects

	if(!vAnalysisObjects.empty())
	{
		for(AnalysisObjectIterator iterAD=vAnalysisObjects.begin(); iterAD!=vAnalysisObjects.end(); iterAD++)
		{
			delete *iterAD;
		}
		vAnalysisObjects.clear();
	}

	// xxEventObjects

	if(!vEventObjects.empty())
	{
		for(EventObjectIterator iterEvent=vEventObjects.begin(); iterEvent!=vEventObjects.end(); iterEvent++)
		{
			delete *iterEvent;
		}
		vEventObjects.clear();
	}

	// xxCommandObjects

	if(!vCommandObjects.empty())
	{
		for(CommandObjectIterator iterCO=vCommandObjects.begin(); iterCO!=vCommandObjects.end(); iterCO++)
		{
			delete *iterCO;
		}
		vCommandObjects.clear();
	}

	// If there is a (private) CInitialStateObject then delete it. It takes care of 
	// deleting the contained CInitialStateData-derived object.

	if(m_pISO)
	{
		delete m_pISO;
		m_pISO = NULL;
	}

}

// Function to read/write the data in the CInputDPDLGFile to a file. The read/write 
// options are selected by the member variable "m_IOFlag", and when reading data 
// new bead, bond and polymer types are created and stored in maps for passing
// back to the CInputData object.
//
// Note that strings that consist of tokens separated by whitespace
// must be enclosed in " " using \" to output the " character and a space
// between the tokens and the quotes. This ensures that the parser can
// identify the beginning and end of such strings.

bool CInputDPDLGFile::Serialize()
{
	// **********************************************************************
    // We conditionally compile out this function to make it inaccessible when
    // a different simulation type is being used.

#if SimIdentifier == DPD

	if(m_IOFlag)	
	{
		// First token identifies simulation type in the data file

        if(IsLG())
        {
		    m_outStream << "dpdlg" << zEndl << zEndl;
        }
        else
        {
		    m_outStream << "dpd" << zEndl << zEndl;
        }

		m_outStream << "Title	" << "\" " << Title.c_str() << " \"" << zEndl;
		m_outStream << "Date	" << Date.c_str()   << zEndl;
		m_outStream << "Comment	" << "\" " << Comment.c_str() << " \"" << zEndl << zEndl;

		// Write data that defines the default initial state. To allow for reading
		// in a CInitialStateData-derived object without knowing in advance what
		// type it is we have to write out a keyword showing which object to create
		// to hold the data. This must be the first token in the data written out.

		m_outStream << m_rISD;

		// Next the bead, bond, bondpair and polymer types are printed

		for(LongBeadTypeIterator iterBead=mNewBeadTypes.begin(); iterBead!=mNewBeadTypes.end(); iterBead++ )
		{
			CBeadType* const pBead = (*iterBead).second;

			m_outStream << "Bead	" << pBead->GetName().c_str() << zEndl;

            if(IsLG())
            {
			    m_outStream << "    	" << pBead->GetDPDStructure()->GetRadius() << " ";
			    m_outStream << "    	" << pBead->GetDPDStructure()->GetLGRadius() << zEndl;
            }
            else
            {
			    m_outStream << "    	" << pBead->GetDPDStructure()->GetRadius() << zEndl;
            }

            m_outStream << "    	";
			for(czDoubleVectorIterator iter1=pBead->GetDPDStructure()->GetConsInt().begin(); iter1!=pBead->GetDPDStructure()->GetConsInt().end(); iter1++)
			{
				m_outStream << *iter1 << "	";
			}
			m_outStream << zEndl;

			m_outStream << "    	";
			for(czDoubleVectorIterator iter2=pBead->GetDPDStructure()->GetDissInt().begin(); iter2!=pBead->GetDPDStructure()->GetDissInt().end(); iter2++)
			{
				m_outStream << *iter2 << "	";
			}

            if(IsLG())
            {
			    m_outStream << zEndl;

			    m_outStream << "    	";
			    for(czDoubleVectorIterator iter3=pBead->GetDPDStructure()->GetLGInt().begin(); iter3!=pBead->GetDPDStructure()->GetLGInt().end(); iter3++)
			    {
				    m_outStream << *iter3 << "	";
			    }
            }
			m_outStream << zEndl << zEndl;
		}

		for(cLongBondTypeIterator iterBond=mNewBondTypes.begin(); iterBond!=mNewBondTypes.end(); iterBond++ )
		{
			CBondType* const pBond = (*iterBond).second;

			m_outStream << "Bond	";
			m_outStream << pBond->GetHeadName().c_str() << " "
						<< pBond->GetTailName().c_str() << "	"
						<< pBond->GetSpringConst() << " " << pBond->GetLength() << zEndl;
		}
		m_outStream << zEndl;

		for(cLongBondPairTypeIterator iterBP=mNewBondPairTypes.begin(); iterBP!=mNewBondPairTypes.end(); iterBP++ )
		{
			CBondPairType* const pBondPair = (*iterBP).second;

			m_outStream << "BondPair    ";
			m_outStream << pBondPair->GetFirstName().c_str()  << " "
						<< pBondPair->GetSecondName().c_str() << " "
						<< pBondPair->GetThirdName().c_str()  << "    "
						<< pBondPair->GetModulus() << " " << pBondPair->GetPhi0() << zEndl;
		}
		m_outStream << zEndl;

		for(LongPolymerTypeIterator iterPoly=mNewPolymerTypes.begin(); iterPoly!=mNewPolymerTypes.end(); iterPoly++ )
		{
			CPolymerType* const pPolymer = (*iterPoly).second;

			// If the CPolymerType's head and tail are specified then output them
			m_outStream << "Polymer	";
			m_outStream << pPolymer->GetName().c_str()  << " "
						<< pPolymer->GetFraction()      << " "
						<< "\" " << pPolymer->GetShape().c_str() << " \"";
			if( pPolymer->GetHeadName() != "")
			{
				m_outStream << " Head " << pPolymer->GetHeadName();
				m_outStream << " Tail " << pPolymer->GetTailName();
			}
			m_outStream << zEndl;
		}
		m_outStream << zEndl;

		// Rest of simulation data

        // If this is a parallel run, write the number of processors in each dimension.

#if EnableParallelSimBox == SimMPSEnabled
		m_outStream << "Processors	" << ProcXNo		<< " " << ProcYNo	<< " " << ProcZNo << zEndl;
#endif

        m_outStream << "Box			" << CNTXCellNo		<< " " << CNTYCellNo	<< " " << CNTZCellNo;
		m_outStream << "			" << CNTXCellWidth	<< " " << CNTYCellWidth << " " << CNTZCellWidth << zEndl;

		m_outStream << "Density		" << AverageBeadDensity << zEndl;
		m_outStream << "Temp		" << kT					<< zEndl;
		m_outStream << "RNGSeed		" << RNGSeed			<< zEndl;
		m_outStream << "Lambda		" << Lambda				<< zEndl;
		m_outStream << "Step		" << StepSize			<< zEndl;
		m_outStream << "Time		" << TotalTime			<< zEndl;

		m_outStream << "SamplePeriod		" << SamplePeriod   << zEndl;
		m_outStream << "AnalysisPeriod		" << AnalysisPeriod << zEndl;
		m_outStream << "DensityPeriod		" << DensityPeriod  << zEndl;
		m_outStream << "DisplayPeriod		" << DisplayPeriod  << zEndl;
		m_outStream << "RestartPeriod		" << RestartPeriod  << zEndl;
		m_outStream << "Grid		" << GridXCellNo	<< " " << GridYCellNo	<< " " << GridZCellNo << zEndl;

		// If new data is added to the input file remember to add a final newline
		// so that the stream state does now show eof and fail the m_inStream.good()
		// test on reading the data.

		// Output a CBilayerData analysis object to match the default control data 
		// file. I removed this on 1/7/04 as it writes in XML but the control data
		// file should be ascii. If it is added back in, the header file must
        // be included above.

//		CBilayerData localBilayerData;

//		m_outStream << localBilayerData;
	}
	else
	{
	// **********************************************************************
	// Read data from control data file.
	//
	// First token must be an allowed simulation type:  dpd or dpdlg
	// All other tokens must appear in the order specified so that we can check
	// that a complete set of data has been read in.

		zString token;	// used to hold the identifier for each input value

		m_inStream >> sSimType;
		if(!m_inStream.good() || (sSimType != "dpd" && sSimType != "dpdlg"))
			return IOError("Unknown simulation type");

        if(sSimType == "dpdlg")
            m_bLG = true;
        else
            m_bLG = false;

		m_inStream >> token;
		if(!m_inStream.good() || token != "Title")
			return IOError("Error reading Title");
		else
		{
			m_inStream >> token;
			if(token != "\"")
				return IOError("Error reading start of title");
			else
			{
				Title = "";
				m_inStream >> token;
				while(m_inStream.good() && token != "\"")
				{
					Title+= token + " ";
					m_inStream >> token;
				}
				Title = RemoveTerminalSpaces(Title);
			}
		}
			

		m_inStream >> token;
		if(!m_inStream.good() || token != "Date")
			return IOError("Error reading Date");
		else
			m_inStream >>  Date;

		m_inStream >> token;
		if(!m_inStream.good() || token != "Comment")
			return IOError("Error reading Comment");
		else
		{
			m_inStream >> token;
			if(token != "\"")
				return IOError("Error reading start of comment");
			else
			{
				m_inStream >> token;
				while(m_inStream.good() && token != "\"")
				{
					Comment+= token + " ";
					m_inStream >> token;
				}
				Comment = RemoveTerminalSpaces(Comment);
			}
		}

		// Read data defining the initial state. As we don't know what type of
		// object to create yet to hold the data we read its type as a keyword.
		// This is used to initialise a private CInitialStateObject member that 
		// selects the appropriate CInitialStateData object to hold the input data.
		// This approach wraps the details about different initial states so that 
		// we don't have to edit this file whenever a new type of initial state 
		// is invented.
		//
		// A valid state specification is: 
		//
		//	State	restart
		//			RunId	999
		//			StateId 50000

		m_inStream >> token;
		if(!m_inStream.good() || token != "State")
			return IOError("Error reading State");

		zString sState;

		m_inStream >> sState;

		if(!m_inStream.good())
			return IOError("Error reading initial state type");
		else
		{
			m_pISO = new CInitialStateObject(sState);
			m_inStream >> *m_pISO;

			if(!m_inStream.good() || !m_pISO->IsDataValid())
				return IOError("Error reading initial state data");

			// Copy the CInitalStateData object pointer to a public member variable
			// for access by the CInputData class

			pISD = m_pISO->GetISD();
		}

// If the next keyword is "Wall" then read the CWallData object, or if it is
// "Gravity" read the CGravityData object, otherwise pass onto the bead data read statements. 

		m_inStream >> token;
		if(!m_inStream.good())
			return IOError("Error reading Wall, Gravity or Bead");
		else 
		{
			while(token != "Bead")
			{
				if(token == "Wall" && m_inStream.good())
				{
					if(pWD)
					{
						return IOError("Error Wall data repeated");
					}
					else
					{
						pWD = new CWallData;
						m_inStream >> *pWD;
					}
					if(!m_inStream.good() || !pWD->IsDataValid())
						return IOError("Error reading wall data");
				}
				else if (token == "Gravity" && m_inStream.good())
				{
					if(pGD)
					{
						return IOError("Error Gravity data repeated");
					}
					else
					{
						pGD = new CGravityData;
						m_inStream >> *pGD;
					}
					if(!m_inStream.good() || !pGD->IsDataValid())
						return IOError("Error reading gravity data");
				}
				else
				{
					return IOError("Error reading Gravity or Wall data");
				}

				m_inStream >> token;
			}
		}
			


// Loop over all bead definitions indicated by token == "Bead". At least
// one bead type must be defined for a valid simulation.

		if(!m_inStream.good() || token != "Bead")
			return IOError("Error reading Bead");
		else
		{
			zString beadName;
			double beadRadius;
			double beadLGRadius;
			double intValue;
			zDoubleVector consInt, dissInt, lgInt;

			long beadCount;
			long beadTotal = 0;

			while(token == "Bead")
			{
				beadTotal++;
				consInt.clear();
				dissInt.clear();
				lgInt.clear();

				m_inStream >> beadName;
				if(!m_inStream.good())
					return IOError("Error reading bead name");

				m_inStream >> beadRadius;
				if(!m_inStream.good() || beadRadius < 0.0)
					return IOError("Error reading bead interaction radius");

                if(IsLG())
                {
				    m_inStream >> beadLGRadius;
				    if(!m_inStream.good() || beadLGRadius < 0.0 || beadLGRadius > beadRadius)
					    return IOError("Error reading bead density-dependent force interaction radius");
                }

				beadCount = 0;
				while(m_inStream.good() && beadCount != beadTotal)
				{
					m_inStream >> intValue;
					if(!m_inStream.good())
						return IOError("Error reading bead conservative value");
					consInt.push_back(intValue);
					beadCount++;
				}

				beadCount = 0;
				while(m_inStream.good() && beadCount != beadTotal)
				{
					m_inStream >> intValue;
					if(!m_inStream.good())
						return IOError("Error reading bead dissipation value");
					dissInt.push_back(intValue);
					beadCount++;
				}

                if(IsLG())
                {
				    beadCount = 0;
				    while(m_inStream.good() && beadCount != beadTotal)
				    {
					    m_inStream >> intValue;
					    if(!m_inStream.good())
						    return IOError("Error reading bead density-dependent interaction value");
					    lgInt.push_back(intValue);
					    beadCount++;
				    }

				    AddNewBeadType(beadName, beadRadius, beadLGRadius, consInt, dissInt, lgInt);
                }
                else
                {
				    AddNewBeadType(beadName, beadRadius, consInt, dissInt);
                }

				m_inStream >> token;
				if(!m_inStream.good())
					return IOError("Error reading Bead token");
			}
		}

// Loop over all bond definitions indicated by token == "Bond". Note that if
// no bonds are specified in the control data file then all polymers must
// only contain single beads.

		zString headName, tailName;
		double SprConst, UnStrLen;

		while(token == "Bond")
		{
			m_inStream >> headName;
			if(!m_inStream.good())
				return IOError("Error reading bond head name");

			m_inStream >> tailName;
			if(!m_inStream.good())
				return IOError("Error reading bond tail name");

			m_inStream >> SprConst;
			if(!m_inStream.good() || SprConst < 0.0)
				return IOError("Error reading bond spring constant");

			m_inStream >> UnStrLen;
			if(!m_inStream.good() || UnStrLen < 0.0)
				return IOError("Error reading bond unstretched length");

			AddNewBondType(headName, tailName, SprConst, UnStrLen);

			m_inStream >> token;
			if(!m_inStream.good())
				return IOError("Error reading Bond token");
		}

// Loop over all bondpair definitions indicated by token == "BondPair". If no
// bond pairs are specified in the control data file then the bonds in polymers
// have no stiffness but still have a Hookean bond energy.

		zString beadName1, beadName2, beadName3;
		double Strength, Phi0;

		while(token == "BondPair")
		{
			m_inStream >> beadName1;
			if(!m_inStream.good())
				return IOError("Error reading bond pair first bead name");

			m_inStream >> beadName2;
			if(!m_inStream.good())
				return IOError("Error reading bond pair second bead name");

			m_inStream >> beadName3;
			if(!m_inStream.good())
				return IOError("Error reading bond pair third bead name");

			m_inStream >> Strength;
			if(!m_inStream.good() || Strength < 0.0)
				return IOError("Error reading bond pair bending modulus");

			m_inStream >> Phi0;
			if(!m_inStream.good() || Phi0 < 0.0 || Phi0 > 180.0)
				return IOError("Error reading bond pair relaxed angle");

			AddNewBondPairType(beadName1, beadName2, beadName3, Strength, Phi0);

			m_inStream >> token;
			if(!m_inStream.good())
				return IOError("Error reading BondPair token");
		}

// Loop over all polymer definitions indicated by token == "Polymer". Note that
// at least one type of polymer must be defined for a valid simulation.

		if(token != "Polymer")
			return IOError("Error reading Polymer token");
		else
		{
			zString polymerName, polymerShape;
			zString polymerHeadName, polymerTailName;
			double fraction    = 0.0;
			double fractionSum = 0.0;
			bool bFirstPolymer = true;

			while(token == "Polymer")
			{
				m_inStream >> polymerName;
				if(!m_inStream.good())
					return IOError("Error reading polymer name");

				m_inStream >> fraction;
				if(!m_inStream.good() || fraction < 0.0 || fraction > 1.0 ||
					(bFirstPolymer && fraction == 0.0))
					return IOError("Error reading polymer number fraction");

				fractionSum += fraction;

				if(fractionSum > 1.00000001)
				{
					return IOError("Error polymer fractions sum > 1.00000001");
				}

				m_inStream >> token;
				if(!m_inStream.good() || token != "\"")
					return IOError("Error reading polymer shape");
				else
				{
					polymerShape = "";

					m_inStream >> token;
					while(m_inStream.good() && token != "\"")
					{
						if(token.at(0) == '\"' || token.at(token.length()-1) == '\"')
							return IOError("Error polymer shape contains a \" ");
						polymerShape+= token + " ";
						m_inStream >> token;
					}
					polymerShape = RemoveTerminalSpaces(polymerShape);

					bFirstPolymer = false;
				}

				m_inStream >> token;
				if(m_inStream.good() && token != "Head")		// default head and tail assignments
					AddNewPolymerType(polymerName, polymerShape, fraction);
				else if(m_inStream.good() && token == "Head")	// head and tail beads are specified
				{
					m_inStream >> polymerHeadName;
					if(!m_inStream.good())
						return IOError("Error reading polymer head");

					m_inStream >> token;
					if(!m_inStream.good() || token != "Tail")
						return IOError("Error reading polymer Tail token");

					m_inStream >> polymerTailName;
					if(!m_inStream.good() || polymerTailName == polymerHeadName)	// check head and tail are distinct
						return IOError("Error reading polymer tail");
					
					AddNewPolymerType(polymerName, polymerShape, fraction, polymerHeadName, polymerTailName);

					m_inStream >> token;	// next token should be Polymer or Box
				}
				else
				{
					return IOError("Error reading polymer Head token");
				}
			}
		}

// Rest of simulation data

#if EnableParallelSimBox == SimMPSEnabled
		    if(!m_inStream.good() || token != "Processors")
			    return IOError("Error reading Processors token");
		    else
		    {
			    m_inStream >> ProcXNo >> ProcYNo >> ProcZNo;
			    if(!m_inStream.good() || ProcXNo < 1 || ProcYNo < 1 || ProcZNo < 1)
				    return IOError("Error reading number of processors in each dimension");
		    }     
#endif

		if(!m_inStream.good() || token != "Box")
			return IOError("Error reading Box token");
		else
		{
			m_inStream >> CNTXCellNo    >> CNTYCellNo    >> CNTZCellNo;
			m_inStream >> CNTXCellWidth >> CNTYCellWidth >> CNTZCellWidth;
			if(!m_inStream.good() || 
				CNTXCellNo < 1 || CNTYCellNo < 1 || CNTZCellNo < 1 ||
				CNTXCellWidth < 1.0 || CNTYCellWidth < 1.0 || CNTZCellWidth < 1.0)
				return IOError("Error reading simulation box size");
		}

		m_inStream >> token;
		if(token != "Density")
			return IOError("Error reading Density token");
		else
		{
			m_inStream >> AverageBeadDensity;
			if(!m_inStream.good() || AverageBeadDensity <= 0.0)
				return IOError("Error reading bead density");
		}

		m_inStream >> token;
		if(token != "Temp")
			return IOError("Error reading Temp token");
		else
		{
			m_inStream >> kT;
			if(!m_inStream.good() || kT < 0.0)
				return IOError("Error reading temperature");
		}

		m_inStream >> token;
		if(token != "RNGSeed")
			return IOError("Error reading RNGSeed token");
		else
		{
			m_inStream >> RNGSeed;
			if(!m_inStream.good())
				return IOError("Error reading RNG seed");
		}

		m_inStream >> token;
		if(token != "Lambda")
			return IOError("Error reading Lambda token");
		else
		{
			m_inStream >> Lambda;
			if(!m_inStream.good() || Lambda < 0.0 || Lambda > 1.0)
				return IOError("Error reading lambda");
		}

		m_inStream >> token;			
		if(token != "Step")
			return IOError("Error reading Step token");
		else
		{
			m_inStream >> StepSize;
			if(!m_inStream.good() || StepSize < 0.000001)
				return IOError("Error reading step size");
		}

		m_inStream >> token;
		if(token != "Time")
			return IOError("Error reading Time token");
		else
		{
			m_inStream >> TotalTime;
			if(!m_inStream.good() || TotalTime < 1)
				return IOError("Error reading total time");
		}

		m_inStream >> token;
		if(token != "SamplePeriod")
			return IOError("Error reading SamplePeriod token");
		else
		{
			m_inStream >> SamplePeriod;
			if(!m_inStream.good() || SamplePeriod < 1 || SamplePeriod > TotalTime)
				return IOError("Error reading sampling period");
		}

		m_inStream >> token;
		if(token != "AnalysisPeriod")
			return IOError("Error reading AnalysisPeriod token");
		else
		{
			m_inStream >> AnalysisPeriod;
			if(!m_inStream.good() || AnalysisPeriod < 1 ||
									(AnalysisPeriod%SamplePeriod != 0)
								  || AnalysisPeriod > TotalTime)
				return IOError("Error reading analysis period");
		}

		m_inStream >> token;
		if(token != "DensityPeriod")
			return IOError("Error reading DensityPeriod token");
		else
		{
			m_inStream >> DensityPeriod;
			if(!m_inStream.good() || DensityPeriod < 1
								  ||(DensityPeriod%SamplePeriod != 0)
								  || DensityPeriod > TotalTime)
				return IOError("Error reading density period");
		}

		m_inStream >> token;
		if(token != "DisplayPeriod")
			return IOError("Error reading DisplayPeriod token");
		else
		{
			m_inStream >> DisplayPeriod;
			if(!m_inStream.good() || DisplayPeriod < 1
								  || DisplayPeriod > TotalTime)
				return IOError("Error reading display period");
		}

		m_inStream >> token;
		if(token != "RestartPeriod")
			return IOError("Error reading RestartPeriod token");
		else
		{
			m_inStream >> RestartPeriod;
			if(!m_inStream.good() || RestartPeriod < 1
								  || RestartPeriod > TotalTime)
				return IOError("Error reading restart period");
		}

		m_inStream >> token;
		if(token != "Grid")
			return IOError("Error reading Grid token");
		else
		{
			m_inStream >> GridXCellNo >> GridYCellNo >> GridZCellNo;
			if(!m_inStream.good() || 
				GridXCellNo < 1 || GridYCellNo < 1 || GridZCellNo < 1)
				return IOError("Error reading analysis grid size");
		}

		// **********************************************************************
		// Now read in the processes to follow during the simulation, the 
		// analysis object specifications if any, the events and the commands 
		// to execute during the simulation. We have to take care of the cases 
		// in which no processes, analysis or event objects are present and/or 
		// no commands.

		// Using while loops allows us to read the token and only execute the
		// read statements for analysis, events or commands when required.

		m_inStream >> token;
		if(!m_inStream.eof())
		{
			if(m_inStream.good())
			{
				while(token == "Process")
				{
					zString typeKeyword("");
					zString type("");

					m_inStream >> typeKeyword >> type;

					if(m_inStream.good() && typeKeyword == "Type")
					{
						xxProcessObject* pProcess = new xxProcessObject(type);
						m_inStream >> *pProcess;

						if(!m_inStream.good() || !pProcess->IsProcessValid()  ||
							pProcess->GetStartTime() > pProcess->GetEndTime()   ||
							pProcess->GetStartTime() > TotalTime           || 
							pProcess->GetEndTime()   > TotalTime)
							return IOError("Error reading process data");

						vProcessObjects.push_back(pProcess);
					}
					else
					{
						return IOError("Error reading process Type");
					}

// The next line is needed because the SGI platforms don't seem to read in a blank
// and overwrite the previous value, but leave it as it was before the read statement.
// It is also needed on the DEC Alpha platforms so I removed the restriction to SGI.

					token = "";
					m_inStream >> token;
				}
			}
			else
			{
				return IOError("Error reading Process token");
			}

			// Now read the analysis objects

			if(!m_inStream.eof())
			{
				if(m_inStream.good())
				{
					while(token == "Analysis")
					{
						zString typeKeyword("");
						zString type("");

						m_inStream >> typeKeyword >> type;

						if(m_inStream.good() && typeKeyword == "Type")
						{
							CAnalysisObject* pAO = new CAnalysisObject(type);
							m_inStream >> *pAO;

							if(!m_inStream.good() || !pAO->IsDataValid()  ||
								pAO->GetStartTime() > pAO->GetEndTime()   ||
								pAO->GetStartTime() > TotalTime           || 
								pAO->GetEndTime()   > TotalTime           ||
								pAO->GetStartTime() % AnalysisPeriod != 0 ||
								pAO->GetEndTime()   % AnalysisPeriod != 0)
								return IOError("Error reading analysis data");

							vAnalysisObjects.push_back(pAO);
						}
						else
						{
							return IOError("Error reading analysis Type");
						}

						token = "";
						m_inStream >> token;
					}
				}
				else
				{
					return IOError("Error reading Analysis token");
				}

					// Read in the events for the simulation if any. Their format is:
					//
					// Event
					//		Type
					//		Times
					//		<event-specific data>
					//

				if(!m_inStream.eof())
				{
					if(m_inStream.good())
					{
						while(token == "Event")
						{
							zString typeKeyword("");
							zString type("");

							m_inStream >> typeKeyword >> type;

							if(m_inStream.good() && typeKeyword == "Type")
							{
								xxEventObject* pEO = new xxEventObject(type);
								m_inStream >> *pEO;

								if(!m_inStream.good() || !pEO->IsEventValid() ||
									pEO->GetStartTime() > pEO->GetEndTime()   ||
									pEO->GetStartTime() > TotalTime           || 
									pEO->GetEndTime()   > TotalTime)
									return IOError("Error reading event data");

								vEventObjects.push_back(pEO);
							}
							else
							{
								return IOError("Error reading Event Type");
							}

							token = "";
							m_inStream >> token;
						}
					}
					else
					{
						return IOError("Error reading Event token");
					}

					// Read in the commands for the simulation if any. Their format is:
					//
					// Command WallOff 10000 data1 data2 ...
					//
					// where data1, etc, are the specific data needed by the command. The
					// name and execution time are read by the xxCommandObject while the
					// remainder of the data is read by the xxCommand-derived class.
					// Commands must be entered in order of increasing execution time,
					// although several commands may be executed at the same time.
					// No further ordering is done on the command sequence.

					if(!m_inStream.eof())
					{
						if(m_inStream.good())
						{
							long lastExecutionTime = 1;	// Initialise here to avoid reinitialisation

							while(token == "Command")
							{
								zString commandName("");
								long executionTime = 0;

								m_inStream >> commandName >> executionTime;

								if(m_inStream.good() && executionTime >= lastExecutionTime)
								{
									lastExecutionTime = executionTime;

									xxCommandObject* pCO = new xxCommandObject(commandName, executionTime);
									m_inStream >> *pCO;

									if(!m_inStream.good() || !pCO->IsCommandValid())
										return IOError("Error reading Command data");

									vCommandObjects.push_back(pCO);
								}
								else
								{
									return IOError("Error reading Command name/time");
								}
								token = "";
								m_inStream >> token;
							}
						}
						else
						{
							return IOError("Error reading Command token");
						}

						// If token is not empty we have an error. If further keywords 
						// are added after the Commands, we have to check that a 
						// misspelled Process/Analysis/Event/Command keyword is caught 
						// as an error before going on to read further data.

						if(!token.empty())
							return IOError("Error unknown Analysis, Event or Command token");

					} // eof after event token
				} // eof afer analysis token
			} // eof after process token
		}
    }
#endif

	return true;
}

// Overloaded function to add a new bead type that includes the interaction radius.

void CInputDPDLGFile::AddNewBeadType(zString name, double radius, zDoubleVector consInt, zDoubleVector dissInt)
{
	CBeadType* pBead = CBeadType::AddDPDBeadType(name, radius, consInt, dissInt);

	mNewBeadTypes.insert(zPairLongBeadType(CBeadType::GetTotal(), pBead));
}


// Overloaded function to add a new bead type that includes the interaction radius 
// and the density-dependent force interaction radius.

void CInputDPDLGFile::AddNewBeadType(zString name, double radius, double lgRadius, zDoubleVector consInt, zDoubleVector dissInt, zDoubleVector lgInt)
{
	CBeadType* pBead = CBeadType::AddDPDBeadType(name, radius, lgRadius, consInt, dissInt, lgInt);

	mNewBeadTypes.insert(zPairLongBeadType(CBeadType::GetTotal(), pBead));
}

// Function to add a new bond type to the mNewBondTypes map. Note that we check for 
// duplicate bond types in the CInputData::AddBondType() routine.

void CInputDPDLGFile::AddNewBondType(zString name1, zString name2, double SprConst, double UnStrLen)
{
	CBondType* pBond = CBondType::AddBondType(name1, name2, SprConst, UnStrLen);

	mNewBondTypes.insert(zPairLongBondType(CBondType::GetTotal(), pBond));
}

// Function to add a new bond pair type to the mNewBondPairTypes. This represents
// the 3-body bending potential for adjacent bonds. It is identified by the names
// of the three beads that make up the pair of bonds.

void CInputDPDLGFile::AddNewBondPairType(zString name1, zString name2, zString name3, double strength, double phi0)
{
	CBondPairType* pBondPair = CBondPairType::AddBondPairType(name1, name2, name3, strength, phi0);

	mNewBondPairTypes.insert(zPairLongBondPairType(CBondPairType::GetTotal(), pBondPair));
}

// Function to add a new polymer type to the mNewPolymerTypes map.

void CInputDPDLGFile::AddNewPolymerType(zString name, zString shape, double fraction)
{
	CPolymerType* pPolymer = CPolymerType::AddPolymerType(name, shape, fraction);

	mNewPolymerTypes.insert(zPairLongPolymerType(CPolymerType::GetTotal(), pPolymer));
}

// Overloaded function to add a new polymer type given that the head and tail beads
// in the polymer are specified by unique bead names.

void CInputDPDLGFile::AddNewPolymerType(zString name, zString shape, double fraction, zString head, zString tail)
{
	CPolymerType* pPolymer = CPolymerType::AddPolymerType(name, shape, fraction, head, tail);

	mNewPolymerTypes.insert(zPairLongPolymerType(CPolymerType::GetTotal(), pPolymer));
}


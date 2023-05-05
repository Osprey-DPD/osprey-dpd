/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// TimeSeriesData.cpp: implementation of the CTimeSeriesData class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimXMLFlags.h"
#include "TimeSeriesData.h"

	using std::setw;
	using std::setprecision;



//////////////////////////////////////////////////////////////////////
// Global functions for serialization
//////////////////////////////////////////////////////////////////////

zOutStream& operator<<(zOutStream& os, const CTimeSeriesData& rTSD)
{
	// If XML output is enabled wrap the data in start and end tags and place each
	// data item inside an element whose name is given by the item's label.
	// Note that the label strings are not used if XML output is disabled.

#if EnableXMLProcesses == SimXMLEnabled
	cStringSequenceIterator iterLabel = rTSD.m_vDataLabels.begin();

	os << "<Data>";
	for(czDoubleVectorIterator iterData=rTSD.m_vDataSet.begin(); iterData!=rTSD.m_vDataSet.end(); iterData++)
	{
		os << "<" << (*iterLabel) << ">";
		os << setw(12) << setprecision(6) << zLeft << (*iterData);
		os << "</" << (*iterLabel) << ">";
		iterLabel++;
	}
	os << "</Data>" << zEndl;

#elif EnableXMLProcesses == SimXMLDisabled
    
    czDoubleVectorIterator iterSDev=rTSD.m_vSDevSet.begin();
    
	for(czDoubleVectorIterator iterData=rTSD.m_vDataSet.begin(); iterData!=rTSD.m_vDataSet.end(); iterData++)
	{
		os << setw(12) << setprecision(6) << zLeft << (*iterData) << " " << (*iterSDev++) <<zEndl;
	}

#endif

	return os;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTimeSeriesData::CTimeSeriesData(long dataSetSize)
{
	// Fix the size of the data set here and add data in the locations later.
	// This avoids having to allocate memory every time a data item is added
	// to the set. Note that resize() fills the vector with the specified
	// default element until its "size()" is given by the first parameter.
	// This is different from reserve() that only allocates memory to the 
	// vector but does not change its "size()".

    m_vDataSet.resize(dataSetSize, 0.0);
    m_vSDevSet.resize(dataSetSize, 0.0);
	m_vDataLabels.resize(dataSetSize);
}

CTimeSeriesData::~CTimeSeriesData()
{

}

// Function to set the value of a data member in the current data set.
// If the id is not in the range of the data set vector an error occurs.
// It might be that later we want to allow the data set to grow dynamically
// during the simulation but this leads to problems identifying the data in 
// the output file by position. Because I don't want to have to label each
// output value I need to use the position to identify them.
//
// Note that the default value of the label is set in the header file.

void CTimeSeriesData::SetValue(long id, double value, zString label)
{
	m_vDataSet.at(id)	 = value;
	m_vDataLabels.at(id) = label;
}

// Overloaded function to set the value of a data member and its stdandard deviation in the current data set.

void CTimeSeriesData::SetValue(long id, double value, double sdev, zString label)
{
    m_vDataSet.at(id)     = value;
    m_vSDevSet.at(id)     = sdev;
    m_vDataLabels.at(id)  = label;
}


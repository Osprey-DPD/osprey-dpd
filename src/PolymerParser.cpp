/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// PolymerParser.cpp: implementation of the CPolymerParser class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "PolymerParser.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPolymerParser::CPolymerParser() : m_SpaceChar(" "),
									m_BranchChar("*"),
									m_LoopChar("/"),
									m_ValidNameChars("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_"),
									m_ValidNumOpChars("0123456789"),
									m_ValidSeparatorOpChars("-_"),
									m_ValidChars(m_ValidNameChars+m_ValidNumOpChars+m_ValidSeparatorOpChars+m_BranchChar+m_LoopChar+m_SpaceChar),
									m_Shape("<Shape>"),
									m_CloseShape("</Shape>"),
									m_BeadName("<BeadName>"),
									m_CloseBeadName("</BeadName>"),
									m_BeadGroup("<BeadGroup>"),
									m_CloseBeadGroup("</BeadGroup>"),
									m_BeadGroupMulti("<BeadGroup multiple="),
									m_BeadGroupBranch("<BeadGroup branch=\"true\">"),
									m_BeadGroupLoop("<BeadGroup loop=\"true\">")

{

}

CPolymerParser::~CPolymerParser()
{

}

// Convert a polymer's shape string to tags: we use the following correspondence:

// (		=	<BeadGroup>
// )		=	</BeadGroup>
//
// (N (		=   <Beadgroup multiple="N">
// ))		=   </BeadGroup>
//
// (* (		=   <Beadgroup branch="true">
// ))		=   </BeadGroup>
//
// (/ (		=   <Beadgroup loop="true">
// ))		=   </BeadGroup>
//
// Note that the closing pairs of brackets must match with the opening ones
// for a multiple, branch or loop. Also, note the a "multiple" attribute must 
// be completed using the actual number found in the polymer's shape and the 
// closing angle bracket added.
//
// Because the shape strings have already been validated by the CInputData
// object, we can safely assume they only contain allowed elements. In particular,
// we assume that the string is wrapped by a single space as provided by the
// CAnalysisState::GetSpaceWrappedPolymerShapeFromType() function.

const zString CPolymerParser::ConvertToTags(const zString shape)
{
	zString tagShape = m_Shape;

	tagShape += ParseElement(true, shape);

	tagShape += m_CloseShape;

	return tagShape;
}

// Private function to parse an element recursively and return a set
// of tags wrapping the "element" shape string.

const zString CPolymerParser::ParseElement(bool bNewBeadGroup, const zString element)
{
	zString tagElement = "";

	zString head = CAR(element);
	zString tail = CDR(element);

	if(IsBeadName(head))			// Simple bead name
	{		
		if(bNewBeadGroup)
		{
			bNewBeadGroup = false;
			tagElement += m_BeadGroup;
		}

		tagElement += BeadNameTag(head);

		if(IsTailEmpty(tail))
		{
			tagElement += m_CloseBeadGroup;
		}
		else
		{
			tagElement += ParseElement(bNewBeadGroup, tail);
		}
	}
	else if(head == m_BranchChar)	// Branch element
	{
		tagElement += m_BeadGroupBranch;
		tagElement += ParseElement(bNewBeadGroup, tail);
		tagElement += m_CloseBeadGroup;
	}
	else if(head == m_LoopChar)		// Loop element  NOT IMPLEMENTED YET
	{
		tagElement += m_BeadGroupLoop;
		tagElement += ParseElement(bNewBeadGroup, tail);
		tagElement += m_CloseBeadGroup;
	}
	else if(!(head.find_first_not_of(m_ValidNumOpChars) < head.length()))	// Multiplier
	{
		tagElement += m_BeadGroupMulti;
		tagElement += "\"" + head + "\">";
		tagElement += ParseElement(bNewBeadGroup, tail);
		tagElement += m_CloseBeadGroup;
	}
	else							// Compound head
	{
		if(!bNewBeadGroup)
		{
			bNewBeadGroup = true;
			tagElement += m_CloseBeadGroup;
		}

		tagElement += ParseElement(bNewBeadGroup, head);

		bNewBeadGroup = true;

		if(!IsTailEmpty(tail))
		{
			tagElement += ParseElement(bNewBeadGroup, tail);
		}
	}


	return tagElement;
}

// Private function to return the first element of a shape string.
// It is named after the LISP function CAR.
//
// It is assumed that the string to be parsed is a valid shape, no
// checking is performed.

zString CPolymerParser::CAR(const zString oldString)
{
	char endChar = ' ';

	zString::const_iterator iterFirst= oldString.begin();
	iterFirst++;									// ignore opening bracket
	zString::const_iterator iterLast = oldString.end();
	iterLast--;										// ignore closing bracket

	zString::const_iterator iterChar = iterFirst;

	bool bFound = false;
	while(!bFound && iterChar != iterLast)
	{
		if(*iterChar == ' ')
		{
			iterChar++;
		}
		else if(*iterChar == '(')
		{
			bFound = true;
			endChar = ')';	// terminating character is right bracket
		}
		else
		{
			bFound = true;		// first alphanumeric character found
			endChar = ' ';		// terminating character is a space
		}
	}

	if( !bFound)
		return oldString;	// return the empty zString

	// now we have the ending condition and the first character find the 
	// end of the first element in the whole zString and return it. But make
	// sure that we match the number of left and right brackets if ")" is the
	// terminating character.

	int leftBrackets = 0;
	bFound = false;
	zString head = "";

	while(!bFound && iterChar != iterLast)
	{
		if(endChar == ')')
		{
			if(*iterChar == endChar)
			{
				leftBrackets--;
				if(leftBrackets == 0)
					bFound = true;
				head+= *iterChar;			// include endChar
				iterChar++;
			}
			else							// include spaces
			{
				if(*iterChar == '(')		// count left brackets to find matching
					leftBrackets++;			// right bracket to end element
				head+= *iterChar;
				iterChar++;
			}
		}
		else if( endChar == ' ')
		{
			if(*iterChar == endChar || *iterChar == '(')
				bFound = true;			// don't include spaces
			else
			{
				head+= *iterChar;
				iterChar++;
			}
		}
	}

// If bFound is still false this means that there were no spaces after the 
// characters before the end. This is not an error.

	return head;
}

// Private function to return the back of a shape string after its head
// has been removed. It is named after the LISP function CDR.
//
// It is assumed that the string to be parsed is a valid shape, no
// checking is performed.

zString CPolymerParser::CDR(const zString oldString)
{
	zString head = CAR(oldString);
	zString tail	= oldString;
	long startPos;

	startPos = oldString.find(head);
	startPos+= head.length();

#if Platform == CRAYJ90
	tail = '(' + tail.remove(0,startPos);
#else
	tail = '(' + tail.erase(0,startPos);	// add opening bracket back
#endif

// The Modena string class does not have the erase() member function nor an
// iterator: we have to use remove() to remove the leading white space.

#if Platform == CRAYJ90

	long j = 1;					// We leave the leading ( alone
	bool bWhiteSpace = true;

	while(bWhiteSpace && j<tail.size())
	{
		if(tail[j] == ' ')
			tail.remove(j,1);
		else
			bWhiteSpace = false;
		j++;
	}

#else

	zStringIterator iterChar=tail.begin();	// remove leading whitespace
	iterChar++;
	while(*iterChar == ' ')
	{
		tail.erase(iterChar);
	}

#endif

	return tail;
}

// Function to return a new <BeadName></BeadName> pair wrapped around
// the supplied bead name string.

const zString CPolymerParser::BeadNameTag(const zString beadName) const
{
	return m_BeadName + beadName + m_CloseBeadName;
}

// Private function to strip the outermost brackets off an element.
// If there are no brackets the element is returned unchanged. Note that
// only a single pair of brackets is removed.

zString CPolymerParser::RemoveOuterBrackets(zString element)
{
	if( (*element.begin() != '(') || (*element.rbegin() != ')') )
		return element;
	else
	{
#if Platform == CRAYJ90
		element.remove(0,1);
		element.remove(element.size()-1,1);
#else
		element.erase(element.begin());
		zStringIterator iterLast = element.end();
		iterLast--;
		element.erase(iterLast);
#endif
	}

	return element;
}

// Function to check if a string is a simple bead name or not. We simply
// look for the presence of any character not allowed in bead names and
// ensure that the first character is a letter.

bool CPolymerParser::IsBeadName(const zString name) const
{
	if(name.empty())
		return false;
	else if(name.find_first_not_of(m_ValidNameChars) < name.length())
		return false;
    else if(name.find_first_of(m_ValidNumOpChars) == 0)
		return false;
    else if(name.find_first_of(m_ValidSeparatorOpChars) == 0)
		return false;

	return true;
}

// Function to check if a string is just an empty pair of brackets "()".
// It uses the helper function SpaceStrippedString() to remove all spaces
// from the string, and then checks if what is left are just a single pair
// of brackets. 

bool CPolymerParser::IsTailEmpty(const zString tail) const
{
	if(SpaceStrippedString(tail) == "()")
		return true;
	else
		return false;
}

// Function to check if a string is empty or contains only spaces. Any other 
// characters mean that the string is not "empty".

bool CPolymerParser::IsStringEmpty(const zString oldString) const
{
	if(oldString.empty())
		return true;
	else
		return (oldString.find_first_not_of(m_SpaceChar) < oldString.length());
}

// Function to delete all spaces from a string and return what is left.

zString CPolymerParser::SpaceStrippedString(const zString oldString) const
{
	zString newString = oldString;

	zStringIterator iterFor	= newString.begin();

	for(long j=0; j<newString.size(); j++)
	{
		if(*iterFor == ' ')
			newString.erase(iterFor);	// erase automatically increments iterator
		else
			iterFor++;
	}

	return newString;
}


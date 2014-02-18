

#pragma once


#include "ichar.h"


class CTokenizer
{
public:
    // If pEvents is used, then sDelimiters is ignored.
    bool Reset(CTSTRING& sString, CTSTRING& sDelimiters);
    bool Reset();
    bool Next(TSTRING& sOut);
    bool TheRest(TSTRING& sOut);// gets the rest of the string, starting at the next token

private:
    TSTRING m_sDelim;
    TSTRING m_s;

    TSTRING::size_type m_pos;
    TSTRING::iterator m_it;
};


class CTokenBuilder
{
public:
    CTokenBuilder(){ m_sDelim = _T(", "); }

    bool SetDelimiter(CTSTRING& s);
    bool Clear();
    bool Append(CTSTRING& s);
    CTSTRING& str();

private:
    TSTRING m_sDelim;
    TSTRING m_s;
};





#include "Stdafx.h"
#include "tokenizer.h"


bool CTokenizer::Reset(CTSTRING& sString, CTSTRING& sDelimiters)
{
    m_s = sString;
    m_sDelim = sDelimiters;

    Reset();

    return true;
}


bool CTokenizer::Reset()
{
    m_it = m_s.begin();
    m_pos = 0;
    return true;
}


bool CTokenizer::TheRest(TSTRING& sOut)
{
    bool r = false;

    // find the next token
    TSTRING::size_type tokenstart = 0;

    if(m_pos != TSTRING::npos)
    {
        tokenstart = m_s.find_first_not_of(m_sDelim, m_pos);
        if(tokenstart == TSTRING::npos)
        {
            m_pos = tokenstart;
        }
        else
        {
            sOut = m_s.substr(tokenstart);
            m_pos = TSTRING::npos;
            r = true;
        }
    }

    return r;
}

bool CTokenizer::Next(TSTRING& sOut)
{
    bool r = false;

    // find the next token
    TSTRING::size_type tokenstart = 0;
    TSTRING::size_type tokenend = 0;

    if(m_pos != TSTRING::npos)
    {
        tokenstart = m_s.find_first_not_of(m_sDelim, m_pos);
        if(tokenstart == TSTRING::npos)
        {
            m_pos = tokenstart;
        }
        else
        {
            tokenend = m_s.find_first_of(m_sDelim, tokenstart);
            if(tokenend == TSTRING::npos)
            {
                sOut = m_s.substr(tokenstart);
            }
            else
            {
                sOut = m_s.substr(tokenstart, tokenend - tokenstart);
            }

            m_pos = tokenend;
            r = true;
        }
    }

    return r;
}


bool CTokenBuilder::SetDelimiter(CTSTRING& sDelim)
{
    m_sDelim = sDelim;
    return true;
}


bool CTokenBuilder::Clear()
{
    m_s.clear();
    return true;
}


bool CTokenBuilder::Append(CTSTRING& s)
{
    if(m_s.length())
    {
        m_s.append(m_sDelim);
    }

    m_s.append(s);

    return true;
}


CTSTRING& CTokenBuilder::str()
{
    return m_s;
}


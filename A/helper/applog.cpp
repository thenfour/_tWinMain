

#include "stdafx.h"
#include "applog.h"


CAppLogBase* g_pLog = 0;


CAppLogBase::CAppLogBase()
{
}


CAppLogBase::~CAppLogBase()
{
}


bool CAppLogBase::CloseLogFile()
{
    return false;
}


bool CAppLogBase::SetLogFile(CTSTRING& sFileName)
{
    return false;
}


bool CAppLogBase::_msg(CTSTRING& s)
{
    return true;
}


bool CAppLogBase::_msgf(PCTSTR sFormat, ...)
{
    bool r = false;
    va_list argptr;
    va_start(argptr, sFormat);
    r = _msgfv(sFormat, argptr);
    va_end(argptr);
    return r;
}


#define MAX_MSGLEN 4096
bool CAppLogBase::_msgfv(PCTSTR sFormat, va_list arglist)
{
    bool r = false;
    PTSTR wszFormatted = 0;

    // Temporary buffer
    wszFormatted = (PTSTR)malloc(MAX_MSGLEN);
    if(!wszFormatted) return false;

    _vsntprintf(wszFormatted, MAX_MSGLEN, sFormat, arglist);

    // Remove nutso-characters.
    TCHAR* wszTemp = 0;
    for(wszTemp = wszFormatted; *wszTemp; wszTemp++)
    {
        switch(*wszTemp)
        {
        case '\r':
        case '\n':
            *wszTemp = '|';
            break;
        }
    }

    r = _msg(wszFormatted);

    free(wszFormatted);

    return r;
}


bool CAppLogBase::_msgerrfv(int gle, PCTSTR wszFormat, va_list arglist)
{
    bool r = false;
    TSTRING s;
    HFormatMessage(s, gle);
    if(_msgfv(wszFormat, arglist))
    {
        r = _msg(s);
    }
    return r;
}


bool CAppLogBase::msg(PCTSTR sFormat, ...)
{
    bool r = false;
    va_list argptr;
    va_start(argptr, sFormat);
    r = _msgfv(sFormat, argptr);
    va_end(argptr);
    return r;
}


bool CAppLogBase::msgerr(int gle, PCTSTR wszFormat, ...)
{
    bool r = false;
    va_list argptr;
    va_start(argptr, wszFormat);
    r = _msgerrfv(gle, wszFormat, argptr);
    va_end(argptr);
    return r;
}


bool HLogSetBuildLog(CAppLogBase* pLog)
{
    g_pLog = pLog;
    return true;
}


bool HFormatMessage(TSTRING& s, int gle)
{
    TCHAR* lpMsgBuf = 0;

    if(FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM | 
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        gle,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (PTSTR)&lpMsgBuf,
        0,
        NULL 
        ))
    {
        //
    }

    if(lpMsgBuf)
    {
        s = lpMsgBuf;
        LocalFree(lpMsgBuf);
    }

    return true;
}


TSTRING HFormatMessage(int gle)
{
    TCHAR* lpMsgBuf = 0;
    TSTRING r;

    if(FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM | 
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        gle,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (PTSTR)&lpMsgBuf,
        0,
        NULL 
        ))
    {
        //
    }

    if(lpMsgBuf)
    {
        r = lpMsgBuf;
        LocalFree(lpMsgBuf);
    }

    return r;
}


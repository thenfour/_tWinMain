/*
    This pure-virtual base class is used to report build messages.
    The inheriting class will be responsible for dispatching the messages
    and handling the error/success codes.
*/


#pragma once


#include "stdafx.h"
#include "ichar.h"


extern class CAppLogBase* g_pLog;


bool HFormatMessage(TSTRING& s, int gle);
TSTRING HFormatMessage(int gle);


class CAppLogBase
{
public:
    CAppLogBase();
    virtual ~CAppLogBase();

    // Handle this function to support outputting the log to a text file.  It's up to the
    // inheriting class to implement actually writing anything to a file.
    virtual bool SetLogFile(CTSTRING& sFileName);
    virtual bool CloseLogFile();

    // The MAIN function - everything else propogates down to this one.  Make sure you override this one :)
    virtual bool _msg(CTSTRING& s);

    // Helper functions.
    bool _msgf(PCTSTR wszFormat, ...);
    bool _msgfv(PCTSTR wszFormat, va_list arglist);
    bool _msgerrfv(int gle, PCTSTR wszFormat, va_list arglist);

    bool msgerr(int gle, PCTSTR wszFormat, ...);
    bool msg(PCTSTR wszFormat, ...);
};


bool HLogSetBuildLog(CAppLogBase* pLog);



inline void Err_OutOfMemory()
{
    g_pLog->msg(_T("Error - out of memory"));
}
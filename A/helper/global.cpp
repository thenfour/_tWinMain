

#include "stdafx.h"
#include "global.h"




// error on ADODB::Connection::Open()
// Error while opening connection %s. Description: '%s' HRESULT: 0x%08x ErrorMessage: '%s' Source: '%s'
void Err_DbOpen(_com_error& e)
{
    TSTRING s(_T("Error while opening the database connection: %s, hr=0x%08x, Err=%s Source=%s"));
    g_pLog->msg(s.c_str(), (PCTSTR)e.Description(), e.Error(), (PCTSTR)e.ErrorMessage(), (PCTSTR)e.Source());
}

// Error while opening a recordset from connection %s. Description: '%s' HRESULT: 0x%08x ErrorMessage: '%s' Source: '%s'
void Err_DbExecute(_com_error& e)
{
    TSTRING s(_T("Error while executing a SQL statement: %s, hr=0x%08x, Err=%s Source=%s"));
    g_pLog->msg(s.c_str(), (PCTSTR)e.Description(), e.Error(), (PCTSTR)e.ErrorMessage(), (PCTSTR)e.Source());
}

// Error while closing connection %s. Description: '%s' HRESULT: 0x%08x ErrorMessage: '%s' Source: '%s'
void Err_DbClose(_com_error& e)
{
    TSTRING s(_T("Error while closing the database connection: %s, hr=0x%08x, Err=%s Source=%s"));
    g_pLog->msg(s.c_str(), (PCTSTR)e.Description(), e.Error(), (PCTSTR)e.ErrorMessage(), (PCTSTR)e.Source());
}


// ADO Provider specific error for connection %s: [%d] %s (%s)
void Err_DbProviderSpecific(CTSTRING& sDescription, long lNumber, CTSTRING& sSource)
{
    TSTRING s(_T("ADO Provider specific error: %s, number=0x%08x, Source=%s"));
    g_pLog->msg(s.c_str(), sDescription.c_str(), lNumber, sSource.c_str());
}


// Error while moving the cursor for connection %s: Description: '%s' HRESULT: 0x%08x ErrorMessage: '%s' Source: '%s'
void Err_DbMoveNext(_com_error& e)
{
    TSTRING s(_T("Error while moving the recordset cursor: %s, hr=0x%08x, Err=%s Source=%s"));
    g_pLog->msg(s.c_str(), (PCTSTR)e.Description(), e.Error(), (PCTSTR)e.ErrorMessage(), (PCTSTR)e.Source());
}

// Error while accessing field %s for connection %s: Description: '%s' HRESULT: 0x%08x ErrorMessage: '%s' Source: '%s'
void Err_DbFieldItem(CTSTRING& sFieldName, _com_error& e)
{
    TSTRING s(_T("Error while accessing field %s for the recordset: %s, hr=0x%08x, Err=%s Source=%s"));
    g_pLog->msg(s.c_str(), sFieldName.c_str(), (PCTSTR)e.Description(), e.Error(), (PCTSTR)e.ErrorMessage(), (PCTSTR)e.Source());
}






#include "StdAfx.h"
#include "ccado.h"
#include "global.h"


#import "msado15.dll" rename("EOF", "adoEOF") implementation_only


namespace CCADO
{

//////////////////////////////////////////////////////////////////////////////
Recordset::Recordset()
{
}


Recordset::~Recordset()
{
    _Free();
}


void Recordset::_Free()
{
    if(m_rst != NULL) m_rst.Release();
}


void Recordset::FlushProviderSpecificErrors()
{
    _variant_t vtConn = m_rst->GetActiveConnection();

    if(vtConn.vt == VT_DISPATCH)
    {
        ADODB::_ConnectionPtr cnn = vtConn;
        CCADO::FlushProviderSpecificErrors(cnn);
    }
}


bool Recordset::MoveNext()
{
    bool r = false;

    if(m_rst)
    {
        try
        {
            if(SUCCEEDED(m_rst->MoveNext()))
            {
                r = true;
            }
        }
        catch(_com_error& e)
        {
            Err_DbMoveNext(e);
            FlushProviderSpecificErrors();
        }
    }

    return r;
}


bool Recordset::Field(CTSTRING& sFieldName, TSTRING& sOut)
{
    bool r = false;
    _variant_t val;

    if(Field(sFieldName, val))
    {
        // convert it to a string.
        try
        {
            val.ChangeType(VT_BSTR);
            HW2T(sOut, val.bstrVal);
            TSTRING::size_type nLast = sOut.find_last_not_of(_T(' '));
            TSTRING::size_type nNewSize = nLast+1;
            if((nLast != TSTRING::npos) && (nNewSize != sOut.length()))
            {
                sOut.resize(nNewSize);
            }
            r = true;
        }
        catch(_com_error& e)
        {
            Err_DbFieldItem(sFieldName, e);
            FlushProviderSpecificErrors();
        }
    }
    return r;
}


bool Recordset::Field(CTSTRING& sFieldName, _variant_t& sOut)
{
    bool r = false;

    if(m_rst)
    {
        try
        {
            ADODB::FieldPtr pf;
            pf = m_rst->GetFields()->GetItem(sFieldName.c_str());
            sOut = pf->GetValue();
            r =true;
        }
        catch(_com_error& e)
        {
            Err_DbFieldItem(sFieldName, e);
            FlushProviderSpecificErrors();
        }
    }

    return r;
}


bool Recordset::IsEOF()
{
    bool r = false;

    if(m_rst)
    {
        try
        {
            if(VARIANT_TRUE == m_rst->adoEOF)
            {
                r = true;
            }
        }
        catch(_com_error&)
        {
            FlushProviderSpecificErrors();
        }
    }

    return r;
}
//////////////////////////////////////////////////////////////////////////////
Command::Command() :
  m_pconn(0)
{
  m_cmd.CreateInstance(L"ADODB.Command");
}

Command::~Command()
{
  m_cmd.Release();
}

bool Command::AddParam(CTSTRING& name, ADODB::DataTypeEnum type, long size, ADODB::ParameterDirectionEnum direction, const _variant_t& value)
{
  bool r = false;
  try
  {
    m_cmd->Parameters->Append(m_cmd->CreateParameter(name.c_str(), type, direction, size, value));
    r = true;
  }
  catch(_com_error e)
  {
    m_pconn->FlushProviderSpecificErrors();
  }
  return r;
}

bool Command::GetParam(CTSTRING& name, _variant_t& result)
{
  bool r = false;
  try
  {
    result = m_cmd->Parameters->Item[name.c_str()]->Value;
    r = true;
  }
  catch(_com_error e)
  {
    m_pconn->FlushProviderSpecificErrors();
  }
  return r;
}

bool Command::ExecuteSP(CTSTRING& sproc)
{
  bool r = false;
  try
  {
    m_cmd->CommandText = sproc.c_str();
    m_cmd->CommandType = ADODB::adCmdStoredProc;
    m_cmd->Execute(0, 0, ADODB::adExecuteNoRecords);
    r = true;
  }
  catch(_com_error e)
  {
    m_pconn->FlushProviderSpecificErrors();
  }
  return r;
}

bool Command::SetConnection(Connection& conn)
{
  bool r = false;
  try
  {
    m_pconn = &conn;
    m_cmd->ActiveConnection = conn.m_conn;
    r = true;
  }
  catch(_com_error e)
  {
    m_pconn->FlushProviderSpecificErrors();
  }
  return r;
}


//////////////////////////////////////////////////////////////////////////////
Connection::Connection()
{
}


Connection::~Connection()
{
    Close();
}


bool Connection::Open(CTSTRING& sConnectionString)
{
    bool r = false;

    Close();

    if(SUCCEEDED(m_conn.CreateInstance(L"ADODB.Connection")))
    {
        try
        {
            m_conn->Errors->Clear();
            if(SUCCEEDED(m_conn->Open(sConnectionString.c_str(), "", "", ADODB::adConnectUnspecified)))
            {
                r = true;
            }
        }
        catch(_com_error e)
        {
            Err_DbOpen(e);
            FlushProviderSpecificErrors();
        }
    }

    return r;
}


bool Connection::Close()
{
    bool r = false;
    if(m_conn != NULL)
    {
        try
        {
            m_conn->Errors->Clear();
            m_conn->Close();
        }
        catch(_com_error e)
        {
            Err_DbClose(e);
            FlushProviderSpecificErrors();
        }
    }

    return r;
}


bool Connection::GetLastIdentity(_variant_t& v)
{
    bool r = false;
    if(m_conn != NULL)
    {
        CCADO::Recordset rst;
        if(Execute(_T("SELECT @@Identity AS 'Identity'"), rst))
        {
            // grab the value from rst.
            r = rst.Field(_T("Identity"), v);
        }
    }
    return r;
}


bool Connection::Execute(CTSTRING& sql, Recordset& rst)
{
    bool r = false;
    if(m_conn != NULL)
    {
        try
        {
            rst._Free();
            m_conn->Errors->Clear();
            if(SUCCEEDED(rst.m_rst = m_conn->Execute(sql.c_str(), 0, ADODB::adCmdText)))
            {
                r = true;
            }
        }
        catch(_com_error e)
        {
            Err_DbExecute(e);
            FlushProviderSpecificErrors();
        }
    }
    return r;
}


bool Connection::_GetLastIdentity(int& n)
{
    _variant_t v;
    bool r = false;
    if(GetLastIdentity(v))
    {
        v.ChangeType(VT_I4);
        n = v.intVal;
        r = true;
    }
    return r;
}


bool Connection::Execute(CTSTRING& sql)
{
    bool r = false;
    if(m_conn)
    {
        try
        {
            m_conn->Errors->Clear();
            if(SUCCEEDED(m_conn->Execute(sql.c_str(), 0, ADODB::adCmdUnspecified)))
            {
                r = true;
            }
        }
        catch(_com_error e)
        {
            Err_DbExecute(e);
            FlushProviderSpecificErrors();
        }
    }
    return r;
}
//
//
//bool Connection::ExecuteOutputParamString(CTSTRING& sql, CTSTRING& paramname, TSTRING& sOut)
//{
//    bool r = false;
//    if(m_conn)
//    {
//        try
//        {
//          ADODB::_CommandPtr cmd;
//          cmd.CreateInstance(L"ADODB.Command");
//          cmd->ActiveConnection = m_conn;
//          cmd->CommandText = sql.c_str();
//          cmd->CommandType = ADODB::adCmdText;
//          cmd->Parameters->Append(cmd->CreateParameter(paramname.c_str(), ADODB::adVarChar, ADODB::adParamOutput, 50));
//          cmd->Execute(0, 0, ADODB::adExecuteNoRecords);
//        }
//        catch(_com_error e)
//        {
//            Err_DbExecute(e);
//            FlushProviderSpecificErrors();
//        }
//    }
//    return r;
//}
//
//
bool Connection::ExecuteAndReturnFirstValue(CTSTRING& sql, CTSTRING& fieldname, _variant_t& vOut)
{
    bool r = false;
    CCADO::Recordset rst;
    if(Execute(sql, rst))
    {
        r = rst.Field(fieldname, vOut);
    }
    return r;
}


void FlushProviderSpecificErrors(ADODB::_ConnectionPtr& conn)
{
    ADODB::ErrorPtr pe;

    long nErrors = conn->Errors->Count;

    if(nErrors)
    {
        while(nErrors)
        {
            nErrors--;
            pe = conn->Errors->GetItem(nErrors);
            Err_DbProviderSpecific((PCTSTR)pe->Description, pe->Number, (PCTSTR)pe->Source);
        }

        conn->Errors->Clear();
    }

    return;
}


void Connection::FlushProviderSpecificErrors()
{
    CCADO::FlushProviderSpecificErrors(m_conn);
}


}// namespace CCADO
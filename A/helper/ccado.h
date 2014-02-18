

#pragma once


#include "ichar.h"
#include "applog.h"
#include <vector>
#include "insertion.h"
#include "update.h"
#include "selection.h"


#import "msado15.dll" rename("EOF", "adoEOF") no_implementation


namespace CCADO
{
    class Recordset
    {
        friend class Connection;
    public:
        Recordset();
        ~Recordset();

        bool MoveNext();
        bool Field(CTSTRING& sFieldName, TSTRING& sOut);
        bool Field(CTSTRING& sFieldName, _variant_t&);

        bool IsEOF();

    private:
        void _Free();
        void FlushProviderSpecificErrors();
        ADODB::_RecordsetPtr m_rst;
    };

    void FlushProviderSpecificErrors(ADODB::_ConnectionPtr& p);

    class Command
    {
    public:
      Command();
      ~Command();

      bool AddParam(CTSTRING& name, ADODB::DataTypeEnum type, long size, ADODB::ParameterDirectionEnum direction, const _variant_t& value);
      bool GetParam(CTSTRING& name, _variant_t& result);
      bool ExecuteSP(CTSTRING& sproc);
      bool SetConnection(Connection& conn);

    private:
      CCADO::Connection* m_pconn;
      ADODB::_CommandPtr m_cmd;
    };


    class Connection
    {
        friend class Recordset;
        friend class Command;
    public:
        Connection();
        ~Connection();

        // sConnectionName is just a symbolic name so we can refer to this thing by name.
        bool Open(CTSTRING& sConnectionString);
        bool Close();
        bool Execute(CTSTRING& sql, Recordset& r);
        bool Execute(CTSTRING& sql);
        bool ExecuteAndReturnFirstValue(CTSTRING& sql, CTSTRING& fieldname, _variant_t& vOut);
        //bool ExecuteOutputParamString(CTSTRING& sql, CTSTRING& paramname, TSTRING& sOut);
        bool GetLastIdentity(_variant_t&);
        bool _GetLastIdentity(int& n);

    private:
        // PRIVATE FUNCTIONS WILL NOT CHECK IF m_conn IS VALID.
        void FlushProviderSpecificErrors();

        ADODB::_ConnectionPtr m_conn;
    };
}


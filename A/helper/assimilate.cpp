

#include "StdAfx.h"
#include "assimilate.h"
#include "oldhelp.h"

namespace Winprog
{

/*
    Populates the WinprogEntities table with information from WinprogUsers.
*/
bool PopulateEntities(CCADO::Connection& x)
{
    CCADO::Recordset rstUsers;
    
    // First delete all entities.
    x.Execute(_T("DELETE WinprogEntities"));

    if(x.Execute(_T("SELECT UserID, UserNick, UserIdent, UserHost FROM WinprogUsers"), rstUsers))
    {
        TSTRING sNick, sIdent, sHost;
        int nUserID;
        _variant_t var;
        bool bExists = false;

        while(!rstUsers.IsEOF())
        {
            rstUsers.Field(_T("UserNick"), sNick);
            rstUsers.Field(_T("UserIdent"), sIdent);
            rstUsers.Field(_T("UserHost"), sHost);
            rstUsers.Field(_T("UserID"), var);
            nUserID = var.intVal;
            _tcout << sNick << _T("!") << sIdent << _T("@") << sHost << std::endl;

            // make sure the nick doesn't already exist
            CCADO::Recordset rstTempEntities;
            bExists = false;
            if(x.Execute(_T("SELECT TOP 1 * FROM WinprogEntities WHERE EntityName=" + CCADO::ToSQL(sNick).s), rstTempEntities))
            {
                if(!rstTempEntities.IsEOF())
                {
                    bExists = true;
                    _tcout << _T("This nick already exists.") << std::endl;
                }
            }

            if(!bExists)
            {
                // The new entity name will be the nick... it may not 
                CCADO::Insertion i;
                i.SetTableName(_T("WinprogEntities"));
                i.AddPair(_T("EntityName"), CCADO::ToSQL(sNick));

                if(!x.Execute(i.ToString()))
                {
                    _tcout << _T("FAILED TO INSERT THE NEW RECORD!!!!!! RESTORE A FUCKING BACKUP SON!") << std::endl;
                    break;
                }
            }
            rstUsers.MoveNext();
        }
    }

    // go through each nick and populate Entity IDs
    if(x.Execute(_T("SELECT UserID, UserNick FROM WinprogUsers"), rstUsers))
    {
        TSTRING sNick;
        _variant_t var;
        int nUserID, nEntityID;

        while(!rstUsers.IsEOF())
        {
            // first get the nick.
            rstUsers.Field(_T("UserNick"), sNick);
            rstUsers.Field(_T("UserID"), var);
            nUserID = var.intVal;

            // find the entity
            if(!x.ExecuteAndReturnFirstValue(_T("SELECT EntityID FROM WinprogEntities WHERE EntityName=" +
                CCADO::ToSQL(sNick).s), _T("EntityID"), var))
            {
                _tcout << _T("Failed to retreive entity ID.") << std::endl;
                break;
            }

            nEntityID = var.intVal;

            CCADO::Update u;
            CCADO::Where w;
            w.SetCriteria(CCADO::WhereEq(_T("UserID"), nUserID));
            u.SetTable(_T("WinprogUsers"));
            u.SetWhere(w);
            u.AddValue(_T("UserEntityID"), CCADO::ToSQL(nEntityID));
            if(!x.Execute(u.ToString()))
            {
                _tcout << _T("FAILED TO UPDATE THE USER's entity id.") << std::endl;
            }

            rstUsers.MoveNext();
        }
    }
    return true;
}

bool Assimilate(CTSTRING& _xml, db& sql)
{
    bool r = false;
    xml::CHelpDB xmldb;
    TSTRING xml = _xml;
    if(xmldb.Open(xml))
    {
        xml::CMessageEnum e;
        if(e.Reset(&xmldb))
        {
            xml::CHelpMessage msg;
            TSTRING sMessage;
            Winprog::Message m;
            Alias a;
            bool bLevel1Break = false;

            while(1)
            {
                if(!e.Next(msg))
                {
                    r = true;
                    break;
                }
                // Create a new message
                TransactInfo ti;
                ti.Channel = _T("#winprog");
                CopyMemory(&ti.DateTime, &msg.Created, sizeof(SYSTEMTIME));
                ti.Nick = msg.Creator;
                ti.Ident = _T("assimil");// Nick!assimil@tion
                ti.Host = _T("ted");

                _tcout << msg.Alias << std::endl;

                // Create the message
                if(!sql.NewMessage(ti, msg.Text, m))
                {
                    break;
                }

                // Create the alias
                if(!sql.NewAlias(ti, m, msg.Alias, a))
                {
                    break;
                }

                // correct the usage on that sucker
                for(int i=0;i<msg.nUsage;i++)
                {
                    if(!sql.FetchMessage(ti, msg.Alias, sMessage))
                    {
                        bLevel1Break = true;
                        break;
                    }
                }
                if(bLevel1Break)
                {
                    break;
                }
            }
        }
    }

    return r;
}


}
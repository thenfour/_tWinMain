/*
    2003-12-20:
        There's a discrepancy between !aliasset and !msgset, regarding deleted messages.
        aliasset will allow you to set an alias that already exists, if it's been cleared.
        msgset will not allow you to overwrite a message whether it's been cleared or not.

        The ACTUAL behavior SHOULD be that both will allow you to change cleared ones.  If
        it's cleared, it will be revived and changed.

        This will eliminate having any duplicates in the tables too.
*/

// helper.cpp : Defines the entry point for the DLL application.
// this file contains the external API

#include "stdafx.h"
#include "helper_i.h"
#include "apputil.h"
#include "global.h"
#include <shlwapi.h>
#include "helpdb.h"
#include "dbhelper.h"
#include "assimilate.h"

#pragma comment(lib, "shlwapi.lib")

// The TSTRING local api 
#define MIRCAPI2 __stdcall
typedef void (MIRCAPI2 *MIRCPROC2)(const Winprog::TransactInfo&, CTSTRING&, TSTRING&);
#define MAX_RESULT_LENGTH       400


HINSTANCE g_hInstance = 0;


TSTRING GetINISetting(CTSTRING& name, CTSTRING& defaul = _T(""))
{
	TSTRING ret;
	TCHAR szFile[MAX_PATH+1] = {0};
	GetModuleFileName(g_hInstance, szFile, MAX_PATH);
	PathRemoveFileSpec(szFile);
	PathAppend(szFile, _T("_tWinMain.ini"));
	CBuffer<TCHAR> r;
	r.Alloc(4096);
	r.Lock();
  r.Unlock();
	GetPrivateProfileString(_T("_tWinMain"), name.c_str(), defaul.c_str(), r.locked_value, 4096, szFile);
	r.Unlock();
	ret.assign(r.buf());
  return ret;
}

TSTRING GetConnectionString()
{
	// first create the filename
	TCHAR szFile[MAX_PATH+1] = {0};
	GetModuleFileName(g_hInstance, szFile, MAX_PATH);
	PathRemoveFileSpec(szFile);
	PathAppend(szFile, _T("_tWinMain.ini"));

	TSTRING ret;

	CBuffer<TCHAR> r;
	r.Alloc(4096);
	r.Lock();

	GetPrivateProfileString(_T("_tWinMain"),
#ifdef _DEBUG
		_T("ConnectionStringDev"),
#else
		_T("ConnectionStringProd"),
#endif
		_T(""), r.locked_value, 4096, szFile);

	r.Unlock();
	ret.assign(r.buf());
	return ret;
}

////////////////////////////////////////////////////////////////////////////////////////////////
int DbAssimilate(const char* xml)
{
    int r = 0;
    TSTRING sxml;
    HA2T(sxml, xml);
    Winprog::db sql;
    if(sql.Connect(GetConnectionString()))
    {
        r = Winprog::Assimilate(sxml, sql) ? 1 : 0;
    }

    return r;
}

int DbPopulateEntities()
{
    int r = 0;
    CCADO::Connection cnn;
    if(cnn.Open(GetConnectionString()))
    {
        r = Winprog::PopulateEntities(cnn) ? 1 : 0;
    }

    return r;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// DATABASE INTERFACE
////////////////////////////////////////////////////////////////////////////////////////////////
class Bot
{
public:
    Bot()
    {
    }

    ~Bot()
    {
    }

    bool Connect()
    {
			TSTRING connectionString = GetConnectionString();
			g_pLog->msg(_T("Connection string: %s"), connectionString.c_str());
        return m_db.Connect(connectionString);
    }

    bool Disconnect()
    {
        return m_db.Disconnect();
    }

    bool MsgSet3(const Winprog::TransactInfo& ti, CTSTRING& sAlias, CTSTRING& sText)
    {
        bool r = false;
        Winprog::Message m;
        Winprog::Alias a;
        // make sure we can set this message.
        if(!m_db.AliasEnabled(sAlias))
        {
            if(m_db.NewMessage(ti, sText, m))
            {
                if(m_db.NewAlias(ti, m, sAlias, a))
                {
                    r = true;
                }
            }
        }
        return r;
    }

    bool Msg3(const Winprog::TransactInfo& ti, CTSTRING& sAlias, TSTRING& sOut)
    {
        bool r = false;

        if(!m_db.FetchMessage(ti, sAlias, sOut))
        {
            // failed to retreive it... let's try with an S
            TSTRING sAliasPlural = sAlias;
            sAliasPlural.push_back('s');
            r = m_db.FetchMessage(ti, sAliasPlural, sOut);
        }
        else
        {
            r = true;
        }
        return r;
    }

    bool MsgClr3(const Winprog::TransactInfo& ti, CTSTRING& sAlias)
    {
        bool r = false;
        Winprog::Alias a;
        if(m_db.GetAliasID(sAlias, a))
        {
            r = m_db.EnableAlias(ti, a, false);
        }
        return r;
    }

    bool AliasNfo3(const Winprog::TransactInfo& ti, CTSTRING& sAlias, TSTRING& sOut)
    {
        bool r = false;

        // created by on, last modified by on, message usage, alias usage
        Winprog::Alias a;
        Winprog::dbHelper h(m_db);

        // Parts of the result
        TSTRING strCreated, strModified, strUsed;

        if(m_db.GetAliasID(sAlias, a))
        {
            //////////////////////////////////////////////////// GENERATE LASTUSED STRING
            long aUsage;
            if(h.GetAliasTotalUsage(a, aUsage))
            {
                sprintf(strUsed, _T("Used %d time(s)"), aUsage);
            }
            else
            {
                strUsed = _T("Never used.");
            }

            //////////////////////////////////////////////////// GENERATE CREATED STRING
            TSTRING aUserCreated;
            SYSTEMTIME aCreated;
            if(h.GetAliasCreatedBy(a, aUserCreated))
            {
                if(h.GetAliasCreatedOn(a, aCreated))
                {
                    sprintf(strCreated,
                        _T("Created by %s on %04d-%02d-%02d at %02d:%02d:%02d"),
                        aUserCreated.c_str(),
                        (DWORD)aCreated.wYear, (DWORD)aCreated.wMonth, (DWORD)aCreated.wDay,
                        (DWORD)aCreated.wHour, (DWORD)aCreated.wMinute, (DWORD)aCreated.wSecond
                        );
                }
                else
                {
                    strCreated = _T("Error getting creation data");
                }
            }
            else
            {
                strCreated = _T("Error getting creation data");
            }

            //////////////////////////////////////////////////// GENERATE MODIFIED STRING
            TSTRING aUserModified;
            SYSTEMTIME aModified;
            if(h.GetAliasLastModifiedBy(a, aUserModified))
            {
                if(h.GetAliasLastModifiedOn(a, aModified))
                {
                    sprintf(strModified,
                        _T("Last Modified by %s on %04d-%02d-%02d at %02d:%02d:%02d"),
                        aUserCreated.c_str(),
                        (DWORD)aCreated.wYear, (DWORD)aCreated.wMonth, (DWORD)aCreated.wDay,
                        (DWORD)aCreated.wHour, (DWORD)aCreated.wMinute, (DWORD)aCreated.wSecond
                        );
                }
                else
                {
                    strModified = _T("Never modified");
                }
            }
            else
            {
                strModified = _T("Never modified");
            }

            //////////////////////////////////////////////////// PUT EM ALL TOGETHER
            sprintf(sOut, _T("%s, %s, %s."), strUsed.c_str(), strCreated.c_str(), strModified.c_str());
            r = true;
        }

        return r;
    }

    bool MsgNfo3(const Winprog::TransactInfo& ti, CTSTRING& sAlias, TSTRING& sOut)
    {
        bool r = false;

        // created by on, last modified by on, message usage, alias usage
        Winprog::Alias a;
        Winprog::dbHelper h(m_db);

        // Parts of the result
        TSTRING strCreated, strModified, strUsed;

        if(m_db.GetAliasID(sAlias, a))
        {
            Winprog::Message m;
            if(h.GetAliasMessageID(a, m))
            {
                //////////////////////////////////////////////////// GENERATE LASTUSED STRING
                long aUsage;
                if(h.GetMessageTotalUsage(m, aUsage))
                {
                    sprintf(strUsed, _T("Used %d time(s)"), aUsage);
                }
                else
                {
                    strUsed = _T("Never used.");
                }

                //////////////////////////////////////////////////// GENERATE CREATED STRING
                TSTRING aUserCreated;
                SYSTEMTIME aCreated;
                if(h.GetMessageCreatedBy(m, aUserCreated))
                {
                    if(h.GetMessageCreatedOn(m, aCreated))
                    {
                        sprintf(strCreated,
                            _T("Created by %s on %04d-%02d-%02d at %02d:%02d:%02d"),
                            aUserCreated.c_str(),
                            (DWORD)aCreated.wYear, (DWORD)aCreated.wMonth, (DWORD)aCreated.wDay,
                            (DWORD)aCreated.wHour, (DWORD)aCreated.wMinute, (DWORD)aCreated.wSecond
                            );
                    }
                    else
                    {
                        strCreated = _T("Error getting creation data");
                    }
                }
                else
                {
                    strCreated = _T("Error getting creation data");
                }

                //////////////////////////////////////////////////// GENERATE MODIFIED STRING
                TSTRING aUserModified;
                SYSTEMTIME aModified;
                if(h.GetMessageLastModifiedBy(m, aUserModified))
                {
                    if(h.GetMessageLastModifiedOn(m, aModified))
                    {
                        sprintf(strModified,
                            _T("Last Modified by %s on %04d-%02d-%02d at %02d:%02d:%02d"),
                            aUserCreated.c_str(),
                            (DWORD)aModified.wYear, (DWORD)aModified.wMonth, (DWORD)aModified.wDay,
                            (DWORD)aModified.wHour, (DWORD)aModified.wMinute, (DWORD)aModified.wSecond
                            );
                    }
                    else
                    {
                        strModified = _T("Never modified");
                    }
                }
                else
                {
                    strModified = _T("Never modified");
                }

                //////////////////////////////////////////////////// PUT EM ALL TOGETHER
                sprintf(sOut, _T("%s, %s, %s."), strUsed.c_str(), strCreated.c_str(), strModified.c_str());
                r = true;
            }
        }

        return r;
    }

    bool AliasList3(const Winprog::TransactInfo& ti, CTSTRING& sAlias, TSTRING& sResult)
    {
        bool r = false;
        Winprog::dbHelper h(m_db);
        Winprog::Alias a;
        std::vector<TSTRING> items;
        CTokenBuilder tb;

        m_db.GetAliasID(sAlias, a);
        tb.SetDelimiter(_T(" "));

        if(h.GetAliasList(a, items))
        {
            long nCount = 0;

            std::vector<TSTRING>::iterator it;
            for(it=items.begin();it!=items.end();it++)
            {
                nCount ++;
                tb.Append(*it);
            }

            if(nCount)
            {
                r = true;
                sprintf(sResult, _T("%d aliases: %s"), nCount, tb.str().c_str());
            }
            else
            {
                sResult = _T("No matches.");
            }
        }

        return r;
    }

    bool Alias3(const Winprog::TransactInfo& ti, CTSTRING& _strNew, CTSTRING& _strExisting, TSTRING& strNew)
    {
        Winprog::Alias aExisting;
        bool bStage1 = false;
        bool r = false;
        {//////////////////////////////////////////////////
            if(m_db.GetAliasID(_strExisting, aExisting))
            {
                // the existing one exists, we're fine.
                strNew = _strNew;
                bStage1 = true;
            }
            else
            {
                // existing doesn't exist?  See if they swapped the two
                if(m_db.GetAliasID(_strNew, aExisting))
                {
                    bStage1 = true;
                    strNew = _strExisting;
                }
            }
        }//////////////////////////////////////////////////

        Winprog::Alias aNew;
        if(bStage1)
        {//////////////////////////////////////////////////
            // Use aExisting and strNew to generate the new alias.
            Winprog::dbHelper h(m_db);
            Winprog::Message m;
            if(h.GetAliasMessageID(aExisting, m))
            {
                r = m_db.NewAlias(ti, m, strNew, aNew);
            }
        }//////////////////////////////////////////////////

        return r;
    }

    bool MsgChange3(const Winprog::TransactInfo& ti, CTSTRING& sAlias, CTSTRING& sText)
    {
        bool r = false;
        Winprog::Alias a;
        if(m_db.GetAliasID(sAlias, a))
        {
            Winprog::Message m;
            Winprog::dbHelper h(m_db);
            if(h.GetAliasMessageID(a, m))
            {
                r = m_db.UpdateMessage(ti, m, sText);
            }
        }
        return r;
    }

    bool MsgRevive3(const Winprog::TransactInfo& ti, CTSTRING& sAlias)
    {
        bool r = false;
        Winprog::Alias a;
        if(m_db.GetAliasID(sAlias, a, false))
        {
            r = m_db.EnableAlias(ti, a, true);
        }
        return r;
    }

#define MAX_MSGQUERY_RESULTS 100

    bool MsgQuery3(const Winprog::TransactInfo& ti, CTSTRING& _sQuery, TSTRING& sOut)
    {
        bool r = false;
        Winprog::AliasList al;
        CTokenBuilder tb;
        tb.SetDelimiter(_T(" "));
        TSTRING sQuery(_T("%"));
        sQuery.append(_sQuery);
        sQuery.push_back('%');

        if(m_db.QueryMessages(ti, sQuery, al, MAX_MSGQUERY_RESULTS))
        {
            long lMatches = 0;
            al.Reset();
            Winprog::Alias a;
            TSTRING s;
            while(al.Next(a, s))
            {
                lMatches ++;
                tb.Append(s);
            }

            if(lMatches == 0)
            {
                // no results
                sprintf(sOut, _T("No matches found for [%s]."), _sQuery.c_str());
            }
            else if(lMatches == 1)
            {
                // get the text.
                TSTRING sMsg;
                if(m_db.FetchMessage(ti, s, sMsg))
                {
                    sprintf(sOut, _T("1 match for [%s]: %s: %s"), _sQuery.c_str(), s.c_str(), sMsg.c_str());
                }
                else
                {
                    sprintf(sOut, _T("1 match for [%s]: %s"), _sQuery.c_str(), tb.str().c_str());
                }
            }
            else if(lMatches >= MAX_MSGQUERY_RESULTS)
            {
                sprintf(sOut, _T("%d+ matches for [%s]: %s"), lMatches, _sQuery.c_str(), tb.str().c_str());
            }
            else
            {
                sprintf(sOut, _T("%d matches for [%s]: %s"), lMatches, _sQuery.c_str(), tb.str().c_str());
            }

            r = true;
        }
        else
        {
            sOut = _T("Error querying");
        }

        return r;
    }

    Winprog::db m_db;
};


Bot* g_pBot = 0;


////////////////////////////////////////////////////////////////////////////////////////////////
// DEFAULT APP LOG
////////////////////////////////////////////////////////////////////////////////////////////////
class CAppLog : public CAppLogBase
{
public:
    CAppLog()
    {
        m_hFile = 0;
    }
    ~CAppLog()
    {
        CloseLogFile();
    }
    bool SetLogFile(CTSTRING& sFileName)
    {
        bool r = false;
        CloseLogFile();
        m_hFile = CreateFile(sFileName.c_str(), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0,
            OPEN_ALWAYS, 0, 0);
        if(IsBadHandle(m_hFile))
        {
            m_hFile = 0;
        }
        else
        {
            r = true;
        }
        return r;
    }
    bool CloseLogFile()
    {
        if(m_hFile) CloseHandle(m_hFile);
        m_hFile = 0;
        return true;
    }
    bool _msg(CTSTRING& x)
    {
        TSTRING ts;
        std::string s;
        DWORD br = 0;

        SYSTEMTIME st;
        GetLocalTime(&st);

        sprintf(ts, _T("[%04d-%02d-%02d %02d:%02d:%02d] %s"),
            st.wYear, st.wMonth, st.wDay,
            st.wHour, st.wMinute, st.wSecond,
            x.c_str());

        TSTRING::iterator It;
        TCHAR ch;
        for(It=ts.begin();It!=ts.end();It++)
        {
            ch = *It;
            switch(ch)
            {
            case '\r':
            case '\n':
                ch = '|';
            case 0:
                break;
            }

            s.push_back((char)ch);
        }

        SetFilePointer(m_hFile, 0, 0, FILE_END);
        WriteFile(m_hFile, s.c_str(), (DWORD)s.length(), &br, 0);
        WriteFile(m_hFile, "\r\n", 2, &br, 0);

        return true;
    }
private:
    HANDLE m_hFile;
};


////////////////////////////////////////////////////////////////////////////////////////////////
// STANDARD API
////////////////////////////////////////////////////////////////////////////////////////////////
int WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, PVOID)
{
    switch(dwReason)
    {
    case DLL_PROCESS_ATTACH:
        {
            g_hInstance = hInstance;
            break;
        }
    case DLL_PROCESS_DETACH:
        {
            break;
        }
    }
    return TRUE;
}

void __stdcall LoadDll(LOADINFO* p)
{
    g_pLog = new CAppLog;

    CBuffer<TCHAR> path;
    path.Alloc(MAX_PATH);
    path.Lock();
    GetModuleFileName(g_hInstance, path.locked_value, MAX_PATH);
    PathAppend(path.locked_value, _T("..\\helper.log"));
    path.Unlock();
    g_pLog->SetLogFile(path.locked_value);
    path.Free();

    g_pLog->msg(_T("LoadDll()"));

    g_pBot = new Bot;
    g_pBot->Connect();
}

int __stdcall UnloadDll(int mTimeout)
{
    if(mTimeout == 1) return 0;
    g_pBot->Disconnect();
    SAFE_DELETE(g_pBot);
    SAFE_DELETE(g_pLog);
    return 0;
}

bool GrabTransactInfoFromCmdLine(IN CTSTRING& cmdline, OUT Winprog::TransactInfo& ti, OUT CTokenizer& t)
{
    bool r = false;
    TSTRING hostmask;
    t.Reset(cmdline, _T(" "));
    if(t.Next(ti.Channel))
    {
        if(t.Next(hostmask))
        {
            if(ti.FromHostMask(hostmask))
            {
                r = true;
            }
        }
    }
    return r;
}

// Dispatches raw commands to local api
int ProcessMirc(char *data, TCHAR* tszName, MIRCPROC2 pProc)
{
    // "channel" "host"
    bool r = false;
    TSTRING sCmdLine;
    TSTRING sResult;
    string saResult;

    // convert the command line
    HA2T(sCmdLine, data);
    g_pLog->msg(_T("--- %s %s"), tszName, sCmdLine.c_str());

    Winprog::TransactInfo ti;
    bool bParsedOK = false;
    CTokenizer t;
    if(GrabTransactInfoFromCmdLine(sCmdLine, ti, t))
    {
        if(t.TheRest(sCmdLine))
        {
            bParsedOK = true;
            pProc(ti, sCmdLine, sResult);
        }
    }

    if(!bParsedOK)
    {
        sResult = _T("Malformed command line.");
        g_pLog->msg(_T("Malformed command line."));
    }

    // re-form the resulting string
    sResult.resize(MAX_RESULT_LENGTH);
    HT2A(saResult, sResult.c_str());
    strcpy(data, saResult.c_str());
    //g_pLog->msg(_T("---"));
    return 3;
}


////////////////////////////////////////////////////////////////////////////////////////////////
// API STUBS PART 2
// These ones take a TSTRING-friendly command line and 
////////////////////////////////////////////////////////////////////////////////////////////////
void MIRCAPI2 Upload2(const Winprog::TransactInfo& ti, CTSTRING& cmdline, TSTRING& sResult)
{
  // upload nick, size, [expiration]
  // create a upload cookie basically using the credentials "default"/"default".
  CCADO::Connection cnn;
  CTokenizer t;
  TSTRING sql;
  TSTRING nick, size, exp;
  long expminutes;
  long sizebytes;
  t.Reset(cmdline, _T(" "));

  // fall-through message (lets try to minimize this)
  sResult = _T("Error: Unspecified");

  if(t.Next(nick))
  {
    if(t.Next(size))
    {
      t.Next(exp);

      // now format the size correctly.  Allowable suffixes are
      // k / kb
      // m / meg / mb
      TSTRING::size_type n = size.find_last_of(_T("0123456789")) + 1;
      TSTRING ext = size.substr(n);
      HStrToLower(ext);
      if((ext == _T("m")) || (ext == _T("mb")) || (ext == _T("meg")) || (ext == _T("megs")))
      {
        sizebytes = 1024000 * stoul(size, 10);
      }
      else if((ext == _T("k")) || (ext == _T("kb")) || (ext.length() == 0))
      {
        sizebytes = 1024 * stoul(size, 10);
      }
      else
      {
        // o no!
        sizebytes = 0;
      }

      expminutes = stoul(exp, 10);
      Winprog::Transaction transid;

      if(!sizebytes)
      {
        sResult = _T("Error: The size specified is invalid. Syntax:  !upload nick size [exp_min]");
      }
      else
      {
        // go ahead with the transaction.
        if(cnn.Open(GetConnectionString()))
        {
          if(g_pBot->m_db.CreateNewTransaction(ti, transid))
          {
            CCADO::Command cmd;
            if(cmd.SetConnection(cnn))
            {
              if(cmd.AddParam(_T("@sCookie"), ADODB::adVarChar, 50, ADODB::adParamOutput, _T("")))
              {
                if(cmd.AddParam(_T("@User"), ADODB::adVarChar, 50, ADODB::adParamInput, _T("default")))
                {
                  if(cmd.AddParam(_T("@Password"), ADODB::adVarChar, 50, ADODB::adParamInput, _T("default")))
                  {
                    if(cmd.AddParam(_T("@Nick"), ADODB::adVarChar, 50, ADODB::adParamInput, nick.c_str()))
                    {
                      if(cmd.AddParam(_T("@Bytes"), ADODB::adInteger, 0, ADODB::adParamInput, sizebytes))
                      {
                        if(cmd.AddParam(_T("@TransactionID"), ADODB::adInteger, 0, ADODB::adParamInput, transid))
                        {
                          if(!expminutes)
                          {
                            // get default minutes
                            TSTRING exp = GetINISetting(_T("UploadDefaultExpirationMinutes"), _T("60"));
                            expminutes = stol(exp, 10);
                          }
                          cmd.AddParam(_T("@Minutes"), ADODB::adInteger, 0, ADODB::adParamInput, expminutes);

                          if(!cmd.ExecuteSP(_T("dbo.NewUpload")))
                          {
                            sResult = _T("Error: SQL errors in command text.  Check log for details.");
                          }
                          else
                          {
                            _variant_t r;
                            if(cmd.GetParam(_T("@sCookie"), r))
                            {
                              r.ChangeType(VT_BSTR);

                              // format the correct result.
                              TSTRING url = GetINISetting(_T("UploadMsg "), _T("http://tenfour.ath.cx/upload/?id=[id]"));

                              g_pBot->m_db.SetTransactionSuccess(transid, true);

                              // replace [id] with the guid
                              HStrReplace(url, _T("[size]"), HFormatFileSize(sizebytes));
                              HStrReplace(url, _T("[id]"), r.bstrVal);
                              TSTRING exp;
                              ultos(exp, expminutes, 10);
                              HStrReplace(url, _T("[exp]"), exp);
                              sResult = url;
                            }// get new cookie
                          }// execute
                        }// add transid
                      }// add bytes
                    }// add nick
                  }// add pass
                }// add user
              }// add cookie
            }// set conn
          }// create trans
        }// cnn.open
      }// sizebytes is good
    }// get size
  }// get nick
  return;
}

void MIRCAPI2 MsgSet2(const Winprog::TransactInfo& ti, CTSTRING& cmdline, TSTRING& sResult)
{
    // MsgSet("alias" "text...")
    CTokenizer t;
    bool bParsedOK = false;
    TSTRING sAlias;
    TSTRING sText;
    t.Reset(cmdline, _T(" "));
    if(t.Next(sAlias))
    {
        if(t.TheRest(sText))
        {
            bParsedOK = true;
            if(g_pBot->MsgSet3(ti, sAlias, sText))
            {
                sprintf(sResult, _T("%s added successfully."), sAlias.c_str());
            }
            else
            {
                sprintf(sResult, _T("%s failed to be added.  If it already exists, use !msgchange"), sAlias.c_str());
            }
        }
    }

    if(!bParsedOK)
    {
        sResult = _T("Malformed command line.");
    }
}

void MIRCAPI2 Msg2(const Winprog::TransactInfo& ti, CTSTRING& cmdline, TSTRING& sResult)
{
    // Msg("alias" "prefix...")
    CTokenizer t;
    TSTRING sAlias;
    TSTRING sPrefix;
    TSTRING sMessage;
    t.Reset(cmdline, _T(" "));
    if(t.Next(sAlias))
    {
        t.TheRest(sPrefix);
        if(g_pBot->Msg3(ti, sAlias, sMessage))
        {
            if(sPrefix.length())
            {
                sprintf(sResult, _T("%s: %s"), sPrefix.c_str(), sMessage.c_str());
            }
            else
            {
                sprintf(sResult, _T("%s"), sMessage.c_str());
            }
        }
        else
        {
            sprintf(sResult, _T("%s failed to be retrieved."), sAlias.c_str());
        }

    }
    else
    {
        sResult = _T("Malformed command line.");
    }
}

void MIRCAPI2 MsgClr2(const Winprog::TransactInfo& ti, CTSTRING& cmdline, TSTRING& sResult)
{
    // MsgClr("alias")
    CTokenizer t;
    TSTRING sAlias;
    t.Reset(cmdline, _T(" "));
    if(t.Next(sAlias))
    {
        if(g_pBot->MsgClr3(ti, sAlias))
        {
            sprintf(sResult, _T("%s cleared successfully."), sAlias.c_str());
        }
        else
        {
            sprintf(sResult, _T("%s failed to be cleared."), sAlias.c_str());
        }
    }
    else
    {
        sResult = _T("Malformed command line.");
    }
}

void MIRCAPI2 MsgNfo2(const Winprog::TransactInfo& ti, CTSTRING& cmdline, TSTRING& sResult)
{
    // MsgNfo("alias")
    CTokenizer t;
    TSTRING sAlias;
    t.Reset(cmdline, _T(" "));
    if(t.Next(sAlias))
    {
        if(!g_pBot->MsgNfo3(ti, sAlias, sResult))
        {
            sprintf(sResult, _T("Error retrieving info for %s."), sAlias.c_str());
        }
    }
    else
    {
        sResult = _T("Malformed command line.");
    }
}

void MIRCAPI2 MsgChange2(const Winprog::TransactInfo& ti, CTSTRING& cmdline, TSTRING& sResult)
{
    // MsgChange("alias text...")
    CTokenizer t;
    bool bParsedOK = false;
    TSTRING sAlias;
    TSTRING sText;
    t.Reset(cmdline, _T(" "));
    if(t.Next(sAlias))
    {
        if(t.TheRest(sText))
        {
            bParsedOK = true;
            if(g_pBot->MsgChange3(ti, sAlias, sText))
            {
                sprintf(sResult, _T("%s changed successfully."), sAlias.c_str());
            }
            else
            {
                sprintf(sResult, _T("%s failed to be changed.  If it doesn't already exist, use !msgset"), sAlias.c_str());
            }
        }
    }

    if(!bParsedOK)
    {
        sResult = _T("Malformed command line.");
    }
}

void MIRCAPI2 MsgQuery2(const Winprog::TransactInfo& ti, CTSTRING& cmdline, TSTRING& sResult)
{
    // MsgQuery("query...")
    CTokenizer t;
    if(!g_pBot->MsgQuery3(ti, cmdline, sResult))
    {
        sprintf(sResult, _T("Error querying '%s'"), cmdline.c_str());
    }
}

void MIRCAPI2 MsgRevive2(const Winprog::TransactInfo& ti, CTSTRING& cmdline, TSTRING& sResult)
{
    // MsgRevive("alias")
    CTokenizer t;
    TSTRING sAlias;
    t.Reset(cmdline, _T(" "));
    if(t.Next(sAlias))
    {
        if(g_pBot->MsgRevive3(ti, sAlias))
        {
            sprintf(sResult, _T("Successfully revived %s."), sAlias.c_str());
        }
        else
        {
            sprintf(sResult, _T("Failed to revive %s."), sAlias.c_str());
        }
    }
    else
    {
        sResult = _T("Malformed command line.");
    }
}

void MIRCAPI2 Alias2(const Winprog::TransactInfo& ti, CTSTRING& cmdline, TSTRING& sResult)
{
    // Alias("new" "existing")
    CTokenizer t;
    TSTRING sNew, sExisting, sNewOut;
    t.Reset(cmdline, _T(" "));
    if(t.Next(sNew))
    {
        if(t.Next(sExisting))
        {
            if(!g_pBot->Alias3(ti, sNew, sExisting, sNewOut))
            {
                sprintf(sResult, _T("Failed to create alias %s."), sNew.c_str());
            }
            else
            {
                sprintf(sResult, _T("Alias %s created successfully."), sNewOut.c_str());
            }
        }
        else
        {
            sResult = _T("Malformed command line.");
        }
    }
    else
    {
        sResult = _T("Malformed command line.");
    }
}

void MIRCAPI2 AliasNfo2(const Winprog::TransactInfo& ti, CTSTRING& cmdline, TSTRING& sResult)
{
    // AliasNfo("alias")
    CTokenizer t;
    TSTRING sAlias;
    t.Reset(cmdline, _T(" "));
    if(t.Next(sAlias))
    {
        if(!g_pBot->AliasNfo3(ti, sAlias, sResult))
        {
            sprintf(sResult, _T("Failed to retreive info on %s."), sAlias.c_str());
        }
    }
    else
    {
        sResult = _T("Malformed command line.");
    }
}

void MIRCAPI2 AliasList2(const Winprog::TransactInfo& ti, CTSTRING& cmdline, TSTRING& sResult)
{
    // AliasList("alias")
    CTokenizer t;
    TSTRING sAlias;
    t.Reset(cmdline, _T(" "));
    if(t.Next(sAlias))
    {
        if(!g_pBot->AliasList3(ti, sAlias, sResult))
        {
            sprintf(sResult, _T("Failed to list aliases for %s."), sAlias.c_str());
        }
    }
    else
    {
        sResult = _T("Malformed command line.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
// API STUBS PART 1
////////////////////////////////////////////////////////////////////////////////////////////////
int __stdcall MsgSet(HWND mWnd, HWND aWnd, char *data, char *parms, BOOL show, BOOL nopause)
{
    return ProcessMirc(data, _T("!msgset"), MsgSet2);
}

int __stdcall Msg(HWND mWnd, HWND aWnd, char *data, char *parms, BOOL show, BOOL nopause)
{
    return ProcessMirc(data, _T("!help"), Msg2);
}

int __stdcall MsgClr(HWND mWnd, HWND aWnd, char *data, char *parms, BOOL show, BOOL nopause)
{
    return ProcessMirc(data, _T("!msgclr"), MsgClr2);
}

int __stdcall MsgNfo(HWND mWnd, HWND aWnd, char *data, char *parms, BOOL show, BOOL nopause)
{
    return ProcessMirc(data, _T("!msgnfo"), MsgNfo2);
}

int __stdcall MsgChange(HWND mWnd, HWND aWnd, char *data, char *parms, BOOL show, BOOL nopause)
{
    return ProcessMirc(data, _T("!msgchange"), MsgChange2);
}

int __stdcall MsgQuery(HWND mWnd, HWND aWnd, char *data, char *parms, BOOL show, BOOL nopause)
{
    return ProcessMirc(data, _T("!msgquery"), MsgQuery2);
}

int __stdcall MsgRevive(HWND mWnd, HWND aWnd, char *data, char *parms, BOOL show, BOOL nopause)
{
    return ProcessMirc(data, _T("!msgrevive"), MsgRevive2);
}

int __stdcall Alias(HWND mWnd, HWND aWnd, char *data, char *parms, BOOL show, BOOL nopause)
{
    return ProcessMirc(data, _T("!msgalias"), Alias2);
}

int __stdcall AliasNfo(HWND mWnd, HWND aWnd, char *data, char *parms, BOOL show, BOOL nopause)
{
    return ProcessMirc(data, _T("!aliasnfo"), AliasNfo2);
}

int __stdcall AliasList(HWND mWnd, HWND aWnd, char *data, char *parms, BOOL show, BOOL nopause)
{
    return ProcessMirc(data, _T("!aliaslist"), AliasList2);
}

int __stdcall Upload(HWND mWnd, HWND aWnd, char *data, char *parms, BOOL show, BOOL nopause)
{
    return ProcessMirc(data, _T("!upload"), Upload2);
}




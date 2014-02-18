

#include "stdafx.h"
#include <tchar.h>
#include "main.h"
#include <map>
#include <string>
#include <iostream>
#include <comdef.h>


typedef std::map<TSTRING, MIRCPROC> CmdMapType;
CmdMapType g_Commands;


void RegisterCommand(CTSTRING& sName, MIRCPROC pProc)
{
    g_Commands.insert(CmdMapType::value_type(sName, pProc));
}


bool CallProc(MIRCPROC pProc, TSTRING& cmdline)
{
    char sz[1000] = {0};
    _bstr_t sA = cmdline.c_str();
    strcpy(sz, sA);
    pProc(0, 0, sz, 0, 0, 0);
    printf("<+_tWinMain> %s\n", sz);
    return true;
}


void EnterAssimilationMode()
{
    std::cout << std::endl << "YOU HAVE ENTERED ASSIMILATION MODE." << std::endl;
    std::cout << "Enter the FULL path of the xml file to assimilate, and don't get it wrong or you'll be sorry.  ";
    std::cout << "In fact, I'd back up your database before you do this!" << endl;
    std::string sLine;
    getline(cin, sLine);
    if(DbAssimilate(sLine.c_str()))
    {
        std::cout << "Assimilation succeeded." << endl;
    }
    else
    {
        std::cout << "Assimilation failed." << endl;
    }
}


int main()
{
    LOADINFO p;
    TSTRING sLine;
    TSTRING sTemp;
    CmdMapType::iterator It;

    CoInitialize(0);
    LoadDll(&p);

    TSTRING sNick, sIdent, sHost;

    // Register all the commands.
    RegisterCommand(_T("!help"), Msg);
    RegisterCommand(_T("!msgset"), MsgSet);
    RegisterCommand(_T("!msgclr"), MsgClr);
    RegisterCommand(_T("!msgrevive"), MsgRevive);
    RegisterCommand(_T("!msgnfo"), MsgNfo);
    RegisterCommand(_T("!msgchange"), MsgChange);
    RegisterCommand(_T("!msgquery"), MsgQuery);
    RegisterCommand(_T("!aliaslist"), AliasList);
    RegisterCommand(_T("!aliasset"), Alias);
    RegisterCommand(_T("!aliasnfo"), AliasNfo);
    RegisterCommand(_T("!upload"), Upload);

    std::cout << "Type 'q' to quit." << std::endl;
    std::cout << "Type !assimilate to enter that thing." << std::endl;
    std::cout << "Type !populateentities to populate the entities table." << std::endl;
    std::cout << "Other commands:" << std::endl;
    std::cout << "/nick [new nick]" << std::endl;
    std::cout << "/ident [new ident]" << std::endl;
    std::cout << "/host [new host]" << std::endl;
    std::cout << "/forger - turn into theForger" << std::endl;
    std::cout << "/redek - turn into redek" << std::endl;
    std::cout << "/wyatt - turn into Wyatt" << std::endl;
    std::cout << "/tenfour - turn into tenfour" << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    sNick = _T("tenfour");
    sIdent = _T("DrDrew");
    sHost = _T("evrtwa1-ar8-4-65-022-186.evrtwa1.dsl-verizon.net");

    std::cout << "Your host info is:" << std::endl;
    _tcout << sNick << _T("!") << sIdent << _T("@") << sHost << std::endl;

    printf("*** Now talking in #winprog\n");
    while(1)
    {
        _tcout << _T("<@") << sNick << _T("> ");
        getline(_tcin, sLine);
        if(!sLine.compare(_T("q"))) break;
        if(!sLine.compare(_T("exit"))) break;
        if(!sLine.compare(_T("quit"))) break;
        if(!sLine.compare(_T("/quit"))) break;

        if(sLine == _T("/forger"))
        {
            sNick = _T("theForger");
            sIdent = _T("brook");
            sHost = _T("coloc1.beyond.ca");
        }

        if(sLine == _T("/redek"))
        {
            sNick = _T("redec");
            sIdent = _T("redek");
            sHost = _T("is.teh.sux");
        }

        if(sLine == _T("/wyatt"))
        {
            sNick = _T("Wyatt");
            sIdent = _T("ryan");
            sHost = _T("my.satellite.sux.com");
        }

        if(sLine == _T("/tenfour"))
        {
            sNick = _T("tenfour");
            sIdent = _T("DrDrew");
            sHost = _T("evrtwa1-ar8-4-65-022-186.evrtwa1.dsl-verizon.net");
        }

        if(sLine.substr(0,6) == _T("/nick "))
        {
            sNick = sLine.substr(6);
            std::cout << "Your host info is:" << std::endl;
            _tcout << sNick << _T("!") << sIdent << _T("@") << sHost << std::endl;
        }

        if(sLine.substr(0,7) == _T("/ident "))
        {
            sIdent = sLine.substr(7);
            std::cout << "Your host info is:" << std::endl;
            _tcout << sNick << _T("!") << sIdent << _T("@") << sHost << std::endl;
        }

        if(sLine.substr(0,6) == _T("/host "))
        {
            sHost = sLine.substr(6);
            std::cout << "Your host info is:" << std::endl;
            _tcout << sNick << _T("!") << sIdent << _T("@") << sHost << std::endl;
        }

        if(sLine == _T("!assimilate"))
        {
            EnterAssimilationMode();
            break;
        }

        if(sLine == _T("!populateentities"))
        {
            DbPopulateEntities();
            break;
        }

        for(It = g_Commands.begin(); It != g_Commands.end(); It++)
        {
            CTSTRING& Key = (*It).first;
            TSTRING::size_type len = Key.length();

            sTemp = sLine.substr(0,len);
            if(!sTemp.compare(Key))
            {
                TSTRING sCmdLine = _T("#winprog ") + sNick + _T("!") + sIdent + _T("@") + sHost;

                if(sLine.length() > (len + 1))
                {
                    sCmdLine.append(sLine.substr(len));
                }

                CallProc((*It).second, sCmdLine);
                break;
            }
        }
    }

    UnloadDll(0);
    CoUninitialize();
    return 0;
}



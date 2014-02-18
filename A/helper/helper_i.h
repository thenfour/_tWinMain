// the file that external libraries include.


#pragma once


#include "ichar.h"


int DbAssimilate(const char* xml);
int DbPopulateEntities();


int __stdcall MsgClr(HWND mWnd, HWND aWnd, char *data, char *parms, BOOL show, BOOL nopause);
int __stdcall MsgNfo(HWND mWnd, HWND aWnd, char *data, char *parms, BOOL show, BOOL nopause);
int __stdcall MsgChange(HWND mWnd, HWND aWnd, char *data, char *parms, BOOL show, BOOL nopause);
int __stdcall MsgQuery(HWND mWnd, HWND aWnd, char *data, char *parms, BOOL show, BOOL nopause);
int __stdcall MsgRevive(HWND mWnd, HWND aWnd, char *data, char *parms, BOOL show, BOOL nopause);
int __stdcall MsgSet(HWND mWnd, HWND aWnd, char *data, char *parms, BOOL show, BOOL nopause);
int __stdcall Msg(HWND mWnd, HWND aWnd, char *data, char *parms, BOOL show, BOOL nopause);
int __stdcall Alias(HWND mWnd, HWND aWnd, char *data, char *parms, BOOL show, BOOL nopause);
int __stdcall AliasNfo(HWND mWnd, HWND aWnd, char *data, char *parms, BOOL show, BOOL nopause);
int __stdcall AliasList(HWND mWnd, HWND aWnd, char *data, char *parms, BOOL show, BOOL nopause);
int __stdcall Upload(HWND mWnd, HWND aWnd, char *data, char *parms, BOOL show, BOOL nopause);


#define CMD_MSG              _T("!help")
#define CMD_MSGSET           _T("!msgset")
#define CMD_MSGCLR           _T("!msgclr")
#define CMD_MSGREVIVE        _T("!msgrevive")
#define CMD_MSGNFO           _T("!msgnfo")
#define CMD_MSGCHANGE        _T("!msgchange")
#define CMD_MSGQUERY         _T("!msgquery")
#define CMD_ALIASLIST        _T("!aliaslist")
#define CMD_ALIASSET         _T("!aliasset")
#define CMD_ALIASNFO         _T("!aliasnfo")
#define CMD_UPLOAD           _T("!upload")



typedef struct
{
    DWORD  mVersion;
    HWND   mHwnd;
    BOOL   mKeep;
} LOADINFO;


void __stdcall LoadDll(LOADINFO* p);
int __stdcall UnloadDll(int mTimeout);

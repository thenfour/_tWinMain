

#include "stdafx.h"
#include "stringconstants.h"
#include "apputil.h"
#include <shlwapi.h>
#include <sstream>
#include <shlobj.h>
#include <iostream>
#include "applog.h"


#pragma comment(lib, "shlwapi.lib")


using namespace std;


// FORWARD DECLARATIONS:
bool HCleanDirectoryIsValidName(CTSTRING& wsz);


#define MAX_SPRINTF_SIZE 1024


bool HExpandEnvironmentVariables(CTSTRING& sIn, TSTRING& out)
{
    TCHAR try1[MAX_PATH] = {0};
    CBuffer<TCHAR> buf;
    TCHAR* tsz = 0;
    DWORD dwRet = 0;
    bool r = false;

    dwRet = ExpandEnvironmentStrings(sIn.c_str(), try1, MAX_PATH);
    if(dwRet != 0)
    {
        if(dwRet > MAX_PATH)
        {
            // It needs more space.
            if(buf.Alloc(dwRet+2))
            {
                tsz = buf.Lock();
                if(tsz)
                {
                    if(ExpandEnvironmentStrings(sIn.c_str(), tsz, (DWORD)buf.GetSize()))
                    {
                        r = true;
                    }

                    buf.Unlock();
                }

                buf.Free();
            }
        }
        else
        {
            // It fit correctly in the try1 string.
            r = true;
            out = try1;
        }
    }

    return r;
}


bool HGetCurrentDirectory(TSTRING& out)
{
    TCHAR try1[MAX_PATH] = {0};
    CBuffer<TCHAR> buf;
    TCHAR* tsz = 0;
    DWORD dwRet = 0;
    bool r = false;

    dwRet = GetCurrentDirectory(MAX_PATH, try1);
    if(dwRet != 0)
    {
        if(dwRet > MAX_PATH)
        {
            // It needs more space.
            if(buf.Alloc(dwRet+2))
            {
                tsz = buf.Lock();
                if(tsz)
                {
                    if(GetCurrentDirectory((DWORD)buf.GetSize(), tsz))
                    {
                        r = true;
                    }

                    buf.Unlock();
                }

                buf.Free();
            }
        }
        else
        {
            // It fit correctly in the try1 string.
            r = true;
            out = try1;
        }
    }

    return r;
}


bool HEnsureAbsolutePathCD(TSTRING& Out, CTSTRING& sFile)
{
    TSTRING sRoot;
    bool r = false;

    if(HGetCurrentDirectory(sRoot))
    {
        r = HEnsureAbsolutePath(Out, sFile, sRoot);
    }

    return r;
}


bool HEnsureAbsolutePath(TSTRING& Out, CTSTRING& sFile, CTSTRING& sRoot)
{
    bool r = false;// return val.

    if(PathIsRelative(sFile.c_str()) == TRUE)
    {
        // Allocate enough room to hold the root + path separator
        // + filename.
        CBuffer<TCHAR> buf;
        buf.Alloc((int)(sRoot.length()+sFile.length()+10));
        PTSTR wszTemp = buf.Lock();

        // Use wszRoot as the root directory and canonicalize it.
        _tcscpy(wszTemp, sRoot.c_str());
        PathAppend(wszTemp, sFile.c_str());

        Out = wszTemp;
        r = true;

        buf.Unlock();
        buf.Free();
    }
    else
    {
        // It's OK; just make a direct copy.
        Out = sFile;
        r = true;
    }

    return r;
}


/*
    ("c:\hats\are\good", "c:\hats") = "are\good"
    ("c:\a\x\y\file", "c:\a\b\dir") = "..\..\x\y\file"
*/
bool HEnsureRelativePath(TSTRING& Path, CTSTRING& sRoot)
{
    CBuffer<TCHAR> b;
    bool r = false;

    if(b.Alloc(Path.length() + sRoot.length()))
    {
        b.Lock();
        if(TRUE == PathRelativePathTo(b.locked_value,
                                      sRoot.c_str(), FILE_ATTRIBUTE_DIRECTORY,
                                      Path.c_str(), FILE_ATTRIBUTE_DIRECTORY))
        {
            Path = b.locked_value;
            r = true;
        }

        b.Unlock();
        b.Free();
    }

    return r;
}

void *OLAcquireResource(HMODULE hMod, PCTSTR pszName, PCTSTR pszType, DWORD *pdwSize) {

	void *r		= 0L;
	HRSRC hRes	= 0L;

	if(0 != (hRes = FindResource(hMod, pszName, pszType))) {
		
		HGLOBAL hMem = 0L;

		if(0 != (hMem = LoadResource(hMod, hRes))) {
			
			if(0 != (r = LockResource(hMem))) {
				*pdwSize = SizeofResource(hMod, hRes);
			}
		}
	}

	return r;
}

PCTSTR BoolToString(bool b)
{
    return b ? _T("True") : _T("False");
}


bool BoolFromString(CTSTRING& s)
{
    TISTRING si = s.c_str();
    if(!si.compare(_T("True"))) return true;
    if(!si.compare(_T("Yes"))) return true;
    if(!si.compare(_T("y"))) return true;
    if(!si.compare(_T("0"))) return false;
    if(!si.compare(_T("1"))) return true;

    return false;
}


TSTRING BoolToStringL(bool b)
{
    if(b) return STR_TRUE;
    return STR_FALSE;
}


bool BoolFromStringL(CTSTRING& s)
{
    TISTRING si = s.c_str();
    
    if(!si.compare(TISTRING(STR_TRUE))) return true;
    if(!si.compare(TISTRING(STR_FALSE))) return true;
    if(!si.compare(_T("0"))) return false;
    if(!si.compare(_T("1"))) return true;

    return false;
}


// This was jus a failed test function... works nicely, but only replaces
// one instance :)
void _StrReplace(TSTRING &s, CTSTRING& to_find, CTSTRING& repl_with)
{
    TSTRING::size_type pos;

    if((pos = s.find(to_find)) != TSTRING::npos)
    {
        s.replace(pos, to_find.size(), repl_with);
    }
}


TSTRING SearchAndReplace(CTSTRING& src, CTSTRING& search, CTSTRING& replace)
{
    typedef basic_ostringstream<TCHAR, std::char_traits<TCHAR>, std::allocator<TCHAR> > ThisOstreamType;

    TSTRING::size_type searchLength = search.length();
    ThisOstreamType os;
    TSTRING::size_type currentPos = 0;
    TSTRING::size_type findPos;

    while(TSTRING::npos != (findPos = src.find(search, currentPos)))
    {
        os << src.substr(currentPos, findPos - currentPos);
        os << replace;
        currentPos = findPos + searchLength;
    }

    os << src.substr(currentPos);

    return os.str();
}

void HStrToUpper(TSTRING &s) {

	TSTRING::iterator it = s.begin();

	for(; it != s.end(); it++) {
		(*it) = toupper((*it));
	}
}

void HStrToLower(TSTRING &s) {

	TSTRING::iterator it = s.begin();

	for(; it != s.end(); it++) {
		(*it) = tolower((*it));
	}
}

void HStrReplace(TSTRING &s, CTSTRING& to_find, CTSTRING& repl_with)
{
    s = SearchAndReplace(s, to_find, repl_with);
}


void HA2W(WSTRING& sDest, PCSTR szIn)
{
    size_t nSize = sizeof(WCHAR)*(strlen(szIn)+1);
    WCHAR* wszTemp = (WCHAR*)malloc(nSize);
    MultiByteToWideChar(CP_ACP, 0, szIn, -1, wszTemp, (int)nSize);
    sDest = wszTemp;
    free(wszTemp);
}


void HW2A(std::string& sDest, PCWSTR szIn)
{
    size_t nSize = sizeof(char)*(wcslen(szIn)+1);
    char* wszTemp = (char*)malloc(nSize);
    WideCharToMultiByte(CP_ACP, 0, szIn, -1, wszTemp, (int)nSize, 0, 0);
    sDest = wszTemp;
    free(wszTemp);
}


//
//size_t HA2WC(PWSTR wszDest, PCSTR szIn)
//{
//    mbstowcs(wszDest, szIn, strlen(szIn));
//}
//
//
//size_t HW2AC(PSTR szDest, PCWSTR wszIn)
//{
//    wcstombs(szDest, wszIn, wcslen(wszIn));
//}


void HA2WB(CBuffer<WCHAR>& Dest, PCSTR szIn, size_t n)
{
    int nSize = MultiByteToWideChar(CP_ACP, 0, szIn, (int)n, 0, 0);
    if(Dest.Realloc(nSize))
    {
        if(Dest.Lock())
        {
            MultiByteToWideChar(CP_ACP, 0, szIn, (int)n, Dest.locked_value, (int)Dest.GetSize());
            Dest.Unlock();
        }
    }
}


void HW2AB(CBuffer<char>& Dest, PCWSTR wszIn, size_t n)
{
    int nSize = WideCharToMultiByte(CP_ACP, 0, wszIn, -1, 0, 0, 0, 0);
    if(Dest.Realloc(nSize))
    {
        if(Dest.Lock())
        {
            WideCharToMultiByte(CP_ACP, 0, wszIn, -1, Dest.locked_value, (int)Dest.GetSize(), 0, 0);
            Dest.Unlock();
        }
    }
}


template<typename StdStringType>
void _TrimWhitespace(StdStringType& s)
{
    StdStringType sTemp;
    StdStringType::size_type nPos;

    nPos = s.find_first_not_of(_T(" \t"));
    if(nPos != StdStringType::npos)
    {
        // trim it.
        sTemp = s.substr(nPos);
        s = sTemp;

        nPos = s.find_last_not_of(_T(" \t"));
        if(nPos != StdStringType::npos)
        {
            s.resize(nPos+1);
        }
    }
    else
    {
        // do nothing.
    }
}

void TrimWhitespace(TSTRING& s)
{
    _TrimWhitespace<TSTRING>(s);
}

void TrimWhitespace(TISTRING& s)
{
    _TrimWhitespace<TISTRING>(s);
}


#define DTOW_STRSIZE 50
void dtos(TSTRING& sDest, double d)
{
    TCHAR wszTemp[DTOW_STRSIZE] = {0};
    _sntprintf(wszTemp, DTOW_STRSIZE, _T("%f"), d);
    sDest = wszTemp;
}


void btos(TSTRING& sDest, BYTE b)
{
    TCHAR wszTemp[DTOW_STRSIZE] = {0};
    _sntprintf(wszTemp, DTOW_STRSIZE, _T("%02.2x"), b);
    sDest = wszTemp;
}


void ltos(TSTRING& sDest, long v, int nBase)
{
    TCHAR wszTemp[DTOW_STRSIZE] = {0};
    _ltot(v, wszTemp, nBase);
    sDest = wszTemp;
}


void ultos(TSTRING& sDest, unsigned long v, int nBase)
{
    TCHAR wszTemp[DTOW_STRSIZE] = {0};
    _ultot(v, wszTemp, nBase);
    sDest = wszTemp;
}

unsigned long stoul(CTSTRING& s, int nBase)
{
    return _tcstoul(s.c_str(), 0, nBase);
}

long stol(CTSTRING& s, int nBase)
{
    return _tcstol(s.c_str(), 0, nBase);
}

double stod(CTSTRING& s)
{
    return _tcstod(s.c_str(), 0);
}



void HStrRemoveAll(TSTRING& s, TCHAR wc)
{
    TCHAR* wszNew = 0;
    TSTRING::size_type nSrc = 0;
    TSTRING::size_type nDest = 0;
    TSTRING::size_type nLength = s.length();
    TCHAR wcTest = 0;
    size_t nSize = (nLength+1)*sizeof(TCHAR);

    wszNew = (TCHAR*)malloc(nSize);
    if(!wszNew) return;

    memset(wszNew, 0, nSize);

    for(nSrc=0;nSrc<nLength;nSrc++)
    {
        wcTest = s[nSrc];
        if(wcTest != wc)
        {
            wszNew[nDest] = wcTest;
            nDest++;
        }
    }

    s.assign(wszNew);
    free(wszNew);
}


/*
    010515 Carl Corcoran
    chKey is the $ or % or whatever that will be inserted.
    wszChars is a string that contains all the characters to be encoded.

    String="The"
    Encode(L'$', L"he");
    Yields: T$68$65
*/
void HStrEncode(TSTRING& s, TCHAR chKey, PCTSTR wszChars)
{
    TSTRING r = _T("");// Resulting string
    TSTRING t = _T("");
    PCTSTR wszSrcThis = s.c_str();
    PCTSTR wszSrcChars = wszChars;
    TCHAR wszTempChar[5] = {0};

    TCHAR wc;

    while(1)
    {
        wc = wszSrcThis[0];
        if(wc == 0) break;

        wszSrcChars = wszChars;

        if(_tcschr(wszChars, wc) != 0)
        {
            // Make it %02
            TSTRING sByte;
            r.push_back(chKey);
            btos(sByte, (BYTE)wc);
            //t.Format("%c%02.2x", chKey, wc);
            r += sByte;
        }
        else
        {
            r.push_back(wc);
        }

        wszSrcThis ++;
    }

    s = r;
}


void HStrDecode(TSTRING& s, TCHAR cKey)
{
    TCHAR* wszToTemp = 0;

    PCTSTR wszFrom = s.c_str();
    PTSTR wszTo = 0;
    TCHAR wc;
    TCHAR wszTemp[3] = {0};
    size_t nSize = sizeof(TCHAR)*(s.length()+1);

    wszToTemp = (TCHAR*)malloc(nSize);
    memset(wszToTemp, 0, nSize);
    wszTo = wszToTemp;

    while(1)
    {
        if(wszFrom[0] == cKey)
        {
            wszTemp[0] = wszFrom[1];
            wszTemp[1] = wszFrom[2];
            wszFrom += 2;
            wc = (TCHAR)_tcstoul(wszTemp, NULL, 16);
            wszTo[0] = wc;
        }
        else
        {
            wszTo[0] = wszFrom[0];
        }
        if(wszFrom[0] == 0)
        {
            break;
        }
        wszFrom ++;
        wszTo ++;
    }

    s = wszToTemp;

    free(wszToTemp);
}


void HPathRemoveFileSpec(TSTRING& s)
{
    TSTRING::size_type nPos = 0;

    nPos = s.find_last_of(_T('\\'));// Find the last backslash.
    if(nPos != TSTRING::npos)
    {
        // it was found, so let's truncate it.
        s.resize(nPos);
    }
}

// returns index of the beginning of the next component of the path, or npos if there are no components left.
TSTRING::size_type HPathFindNextComponent(CTSTRING& s, TSTRING::size_type nStart)
{
    PCTSTR tszOrg = s.c_str() + nStart;
    PTSTR tszNext = PathFindNextComponent(tszOrg);
    if(!tszNext) return TSTRING::npos;
    return (tszNext - tszOrg) + nStart;
}


/*
    Start with the root, working our way through the paths, like:
    c:\
    c:\program files\
    c:\program files\hide\
    c:\program files\hide\demo\
    etc...
*/
bool HCreateDirectory(CTSTRING& s)
{
    bool r = false;
    TSTRING sTemp;
    TSTRING::size_type n = 0;
    DWORD dwAttributes = 0;

    while(1)
    {
        n = HPathFindNextComponent(s, n);
        sTemp = s.substr(0, n);// extract up to the next component.

        // See if this thing already exists
        dwAttributes = GetFileAttributes(sTemp.c_str());
        if(dwAttributes == (DWORD)-1)
        {
            // file doesn't exist at all.
            if(!CreateDirectory(sTemp.c_str(), 0))
            {
                // FAILED!
                g_pLog->msgerr(GetLastError(), _T("Failed to create directory '%s'"), sTemp.c_str());
                break;
            }
        }
        else if((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
        {
            // It already exists, but it's a file.
            g_pLog->msgerr(ERROR_FILE_EXISTS, _T("Failed to create directory '%s'"), sTemp.c_str());
            break;
        }

        if(n == TSTRING::npos)
        {
            // no more components; this means that we were successful creating all the previous components
            r = true;
            break;
        }
    }

    return r;
}


bool HCleanDirectory(CTSTRING& wszDir, CTSTRING& wszFilter)
{
    bool r = false;
    TSTRING sFindPath;
    WIN32_FIND_DATA fd = {0};
    HANDLE hFind = 0;

    sFindPath = wszDir;
    HPathJoin(sFindPath, _T("*"));

    hFind = FindFirstFile(sFindPath.c_str(), &fd);

    if((INVALID_HANDLE_VALUE != hFind) && (hFind != NULL))
    {
        do
        {
            if(HCleanDirectoryIsValidName(fd.cFileName))
            {
                // handle the file.
                if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    // It's a directory; clean it too...
                    TSTRING sNewDir = wszDir;
                    HPathJoin(sNewDir, fd.cFileName);
                    //if(!HCleanDirectory(sNewDir.c_str(), wszFilter))
                    if(!HCleanDirectory(sNewDir, wszFilter))
                    {
                        g_pLog->msg(_T("Warning: failed to clean folder '%s'"), sNewDir.c_str());
                    }
                }
                else
                {
                    // It's a file; see if we need to delete it.
                    if(HPathMatchSpec(fd.cFileName, wszFilter.c_str()))
                    {
                        TSTRING sFullPath = wszDir;
                        HPathJoin(sFullPath, fd.cFileName);
                        if(!DeleteFile(sFullPath.c_str()))
                        {
                            g_pLog->msgerr(GetLastError(), _T("Warning: failed to clean file '%s'"), sFullPath.c_str());
                        }
                    }
                }
            }
        }
        while(FindNextFile(hFind, &fd));

        FindClose(hFind);
        r = true;
    }

    return r;
}


bool HPathMatchSpec(CTSTRING& sFile, CTSTRING& sFilter)
{
    return (PathMatchSpec(sFile.c_str(), sFilter.c_str()) == TRUE);
}

bool TFileNameFromPath(CTSTRING &tsPath, TSTRING &tsFile) {

	bool r = false;
	TSTRING::size_type s;

	if(tsPath.length()) {
		s = tsPath.rfind(_T("\\"));
		if(TSTRING::npos != s) {
			tsFile.assign(tsPath, s + 1, TSTRING::npos);
			r = true;
		} else {
			s = tsPath.rfind(_T("/"));
			if(TSTRING::npos != s) {
				tsFile.assign(tsPath, s + 1, TSTRING::npos);
				r = true;
			} else {
				s = tsPath.rfind(_T(":"));
				if(TSTRING::npos != s) {
					tsFile.assign(tsPath, s + 1, TSTRING::npos);
					r = true;
				}
			}
		}
	}

	return r;
}

// Returns true if the file is not a "." or ".." filename
// returned by Find*File();
bool HCleanDirectoryIsValidName(CTSTRING& s)
{
    bool r = true;
    if(!s.length())
    {
        // empty string ?!
        r = false;
    }
    else
    {
        if(s[0] == _T('.'))
        {
            if(s[1] == _T('.'))
            {
                // two dots... let's see if that's all we have
                if(s[2] == 0)
                {
                    // blammo.
                    r = false;
                }
            }
            else
            {
                if(s[1] == 0)
                {
                    // L"."
                    r = false;
                }
            }
        }
    }
    return r;
}


bool HPathJoin(TSTRING& sDest, CTSTRING& sExtra)
{
    if(sDest.length())
    {
        if(sDest[sDest.length()-1] != _T('\\'))
        {
            sDest.push_back(_T('\\'));
        }
        sDest.append(sExtra);
    }
    else
    {
        sDest = sExtra;
    }

    return true;
}

// I MUST use a C-style string in here, or else there's no way to access arguments
bool sprintf(TSTRING& s, PCTSTR sFormat, ...)
{
    va_list argptr;
    TCHAR wsz[MAX_SPRINTF_SIZE] = {0};

    va_start(argptr, sFormat);
    wvnsprintf(wsz, MAX_SPRINTF_SIZE, sFormat, argptr);
    va_end(argptr);

    s = wsz;

    return true;
}

//
//bool sprintf(TSTRING& s, __CTSTRING_REF sFormat, ...)
//{
//    va_list argptr;
//    TCHAR wsz[MAX_SPRINTF_SIZE] = {0};
//
//    va_start(argptr, sFormat);
//    wvnsprintf(wsz, MAX_SPRINTF_SIZE, sFormat.m_r.c_str(), argptr);
//    va_end(argptr);
//
//    s = wsz;
//
//    return true;
//}


TSTRING::size_type HPathFindExtension(CTSTRING& s)
{
    TSTRING::size_type n;
    n = s.find_last_of('.');

    // If there was no slash found, OR the slash was the last char,
    // then return npos.
    if((n == TSTRING::npos) || ((n+1) == s.length()))
    {
        return TSTRING::npos;
    }

    // Return the index PAST the period.
    return n+1;
}


// Just like PathFindFileName()
TSTRING::size_type HPathFindFileName(CTSTRING& s)
{
    TSTRING::size_type n;
    n = s.find_last_of(_T("\\/:"));

    // If there was no slash found, OR the slash was the last char,
    // then return 0.
    if((n == TSTRING::npos) || ((n+1) == s.length()))
    {
        return 0;
    }

    // Return the index PAST the delimiting char.
    return n+1;
}


bool IsWhitespace(TCHAR c)
{
    // 9 10 11 12 13
    if((c>=9)&&(c<=13)) return true;
    // 28 29 30 31 32
    if((c>=28)&&(c<=32)) return true;
    if(c<8192) return false;// just to optimize a little bit - VERY rarely will we need to continue past here.
    // 8192 8193 8194 8195 8196 8197 8198 8199 8200 8201 8202 8203
    if((c>=8192)&&(c<=8203)) return true;
    // 8232 8233 12288
    switch(c)
    {
    case 8232:
    case 8233:
    case 12288:
        return true;
    }
    return false;
}


bool ASCIIToWideChar(const char* szIn, int cchA, WCHAR* wszOut, int cchW)
{
    bool r = false;
    int nChars = 0;
    for(nChars = min(cchW, cchA) ; nChars ; nChars--,szIn++,wszOut++)
    {
        *wszOut = *szIn;
    }
    return r;
}

bool WideCharToASCII(const WCHAR* wszIn, int cchW, char* szOut, int cchA)
{
    bool r = true;
    int nChars = 0;

    for(nChars = min(cchW, cchA);nChars;nChars--,wszIn++,szOut++)
    {
        if(*wszIn > 127)
        {
            *szOut = '?';
            r = false;
        }
        else
        {
            *szOut = (char)*wszIn;
        }
    }

    return r;
}


bool ConsoleGetLine(TSTRING& sOut)
{
    getline(_tcin, sOut);
    return true;
}



ByteOrder GetMachineByteOrder()
{
    WCHAR w = 0x0001;
    BYTE* p = (BYTE*)&w;
    if(p[0] == 0x01) return BO_LE;
    return BO_BE;
}


bool HGetWindowText(HWND hWnd, TSTRING& s)
{
    bool r = false;
    CBuffer<TCHAR> buf;
    TCHAR* tsz = 0;
    int nSize = 0;

    if(IsWindow(hWnd))
    {
        nSize = GetWindowTextLength(hWnd);
        if(!nSize)
        {
            s.clear();
            r = true;
        }
        else
        {
            // Allocate a buffer to hold the string.
            buf.Alloc(nSize+1);
            tsz = buf.Lock();
            if(tsz)
            {
                if(GetWindowText(hWnd, tsz, nSize+1))
                {
                    s = tsz;
                    r = true;
                }
                buf.Unlock();
                buf.Free();
            }
        }
    }

    return r;
}


bool HexToRGB(const TCHAR *pHex, COLORREF &clr)
{ 
      bool ret = false; 
      const TCHAR *p = pHex; 
  
      TCHAR r[3] = {0}; 
      TCHAR g[3] = {0}; 
      TCHAR b[3] = {0}; 
  
      BYTE bR = 0; 
      BYTE bG = 0; 
      BYTE bB = 0; 
  
      if(p) { 
  
           CopyMemory(r, p+1, sizeof(TCHAR) * 2); 
           CopyMemory(g, p+3, sizeof(TCHAR) * 2); 
           CopyMemory(b, p+5, sizeof(TCHAR) * 2); 
  
           bR = (BYTE)_tcstol(r, NULL, 16); 
           bG = (BYTE)_tcstol(g,  NULL, 16); 
           bB = (BYTE)_tcstol(b,  NULL, 16); 
  
           clr = RGB(bR, bG, bB); 
  
           ret = true; 
      } 
  
      return ret; 
 }


bool HCLSIDFromProgID(CTSTRING& sProgID, CLSID& x)
{
    HRESULT hr = 0;
    bool r = false;
    WSTRING wsProgID;

    HT2W(wsProgID, sProgID.c_str());

    if(SUCCEEDED(hr = CLSIDFromProgID(wsProgID.c_str(), &x)))
    {
        r = true;
    }

    return r;
}

/*
    43 0A 0D 3F 49 49 4C 41 0D 0A
*/
bool Bin2Str(TSTRING& s, PBYTE pData, int nSizeBytes)
{
    int i;
    TCHAR tszTemp[10] = {0};

    s.clear();

    for(i=0;i<nSizeBytes;i++)
    {
        _stprintf(tszTemp, _T("%02X "), pData[i]);
        s.append(tszTemp);
    }

    return true;
}


// returns number of bytes needed.  Everything but whitespace is considered valid, so you'll just
// get unexpected results with weird bad input.
int Str2Bin(CTSTRING& s, PBYTE pData, int nBufferSize)
{
    int nBytes = 0;
    TSTRING::size_type nSrc = 0;
    TCHAR ch = 0;
    int nTemp = 0;
    TCHAR tszTemp[10] = {0};

    for(nSrc=0;nSrc<s.length();nSrc++)
    {
        ch = s[nSrc];

        if(!IsWhitespace(ch))
        {
            // append it to temp.
            tszTemp[nTemp] = ch;
            nTemp++;
            if(nTemp == 2)
            {
                // tszTemp is filled up.  Flush it.
                if((nBytes < nBufferSize) && pData)
                {
                    pData[nBytes] = (BYTE)_tcstoul(tszTemp, 0, 16);
                }

                nTemp = 0;
                tszTemp[0] = 0;
                tszTemp[1] = 0;

                nBytes ++;
            }
        }
    }

    return nBytes;
}

// 0-1,023         = "990 bytes"
// 1,024-1,023,000 = "991.54 kb"
// 1,024,000+      = "1,234 kb"
TSTRING HFormatFileSize(unsigned long n)
{
  TSTRING r;
  if(n < 1024)
  {
    sprintf(r, _T("%u bytes"), n);
  }
  else if(n < 1023000)
  {
    sprintf(r, _T("%.2u kb"), n >> 10);
  }
  else
  {
    sprintf(r, _T("%u kb"), n >> 10);
  }
  return r;
}






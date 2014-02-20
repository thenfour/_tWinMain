<%


Function IsPathSeparator(s)
    Dim r
    r = False
    If Left(s, 1) = "/" Then r = True
    If Left(s, 1) = "\" Then r = True
    IsPathSeparator = r
End Function


' This function returns the position in the two strings where the path is different.
' this uses path-parsing, so the following will work correctly:
' "c:\mercurian" and "c:\mercury"  - this returns 4, stating that the 4th character in
' either string is the beginning of a "different" path component.
' this fucntion is NOT CASE SENSITIVE
Function FindDifferentPathComponent_Helper(s1_t, s2, fso)
    Dim s1
    s1 = s1_t

    ' default to 1
    FindDifferentPathComponent_Helper = 1

    ' keep removing components until the COMPLETE s1 can be found at position 1 in s2.
    While True
        If Instr(1, s2, s1) = 1 Then
            ' s1 was found completely at position 1 of s2.  We have our answer.
            FindDifferentPathComponent_Helper = Len(s1) + 1
            Exit Function
        Else
            ' It wasn't found; we need to trim more path components off.
            s1 = fso.GetParentFolderName(s1)
            If s1 = "" Then
                ' no parents left; no components are similar
                Exit Function
            End If
        End If
    Wend   
     
End Function


Function FindDifferentPathComponent(s1, s2, fso)
    Dim nReturn1, nReturn2
    nReturn1 = FindDifferentPathComponent_Helper(s1, s2, fso)
    nReturn2 = FindDifferentPathComponent_Helper(s2, s1, fso)
    If nReturn1 < nReturn2 Then
        FindDifferentPathComponent = nReturn1
    Else
        FindDifferentPathComponent = nReturn2
    End If
End Function


Function GetDrive(s)
    Dim nColon
    nColon = InStr(1, s, ":")
    If nColon < 1 Then
        GetDrive = ""
        Exit Function
    End If
    GetDrive = Left(s, nColon-1)
End Function

Function TrimLeadingSeparators(s)
    Dim r
    r = s
    While (IsPathSeparator(r) = True)
        r = Mid(r, 2)
    Wend
    TrimLeadingSeparators = r
End Function


' Convert a server-mapped path to a relative URL (I know I'm leet)
' "c:\inetpub\wwwroot\oneweb\high_quality_image.jpg" <-- path
' "c:\inetpub\wwwroot\oneweb\hide\"  <-- current directory
' result: "..\image.jpg"
Function PathAbsoluteToRelative(strFile, strDirectoryRelativeTo)
    Dim url, file, dir
    Dim fso
    Dim nTrim

    Set fso = Server.CreateObject("Scripting.FileSystemObject")
    
    PathAbsoluteToRelative = ""

    ' This is to eliminate fuss with case-sensitivity
    file = LCase(strFile)
    dir = LCase(strDirectoryRelativeTo)

    ' This will tell us if the paths are both absolute(they have to be) and what their drives are
    ' if the drives are different, then it's impossible.    
    Dim dFile, dDir
    dFile = GetDrive(file)
    dDir = GetDrive(dir)
    If Not (dFile = dDir) Then
        dbg "dirs are different (" & dFile & " vs. " & dDir & ")<br>"
        Exit Function
    End If
    If Len(dFile) = 0 Then
        dbg "no dir! (" & dFile & " and " & dDir & ")<br>"
        Exit Function
    End If

    ' trim off the similar parts of the path
    ' so we're left with "image.jpg" and "hide\"
    nTrim = FindDifferentPathComponent(file, dir, fso)
    file = Mid(file, nTrim)
    dir = Mid(dir, nTrim)
    
    ' Make sure neither path begins with path separators
    file = TrimLeadingSeparators(file)
    dir = TrimLeadingSeparators(dir)

    ' for each path component in dir, stick a "..\" in the result.
    url = ""
    While Not (dir = "")
        url = fso.BuildPath(url, "..")
        dir = fso.GetParentFolderName(dir)
    Wend
    
    url = fso.BuildPath(url, file)

    Set fso = Nothing
    
    PathAbsoluteToRelative = url
    
End Function


Function GetCurrentPath()
    Dim sPathInfo
    Dim sThisFile
    Dim fso
    Set fso = Server.CreateObject("Scripting.FileSystemObject")
    sPathInfo = Request.ServerVariables("PATH_INFO")
    sThisFile = Server.MapPath(sPathInfo)
    GetCurrentPath = fso.GetParentFolderName(sThisFile)

    Set fso = Nothing
End Function


' Takes a full path (server path) and creates the relative path from the current file's directory.
Function CurrentFileToRelative(sFile)
    Dim r
    r = PathAbsoluteToRelative(sFile, GetCurrentPath())
    CurrentFileToRelative = Replace(r, "\", "/")
End Function

' Generates the relative URL to this page from a different location - typically used to 
' generate "blah.asp?referer=../thispage.asp" query strings so that blah.asp knows where to 
' redirect.
'
' destinationpath is a path relative to the root.
Function GetRefererURLFromRoot(DestRelativeToRoot) 
    Dim Referer, ThisPath
    Dim DestPath' destination full path (server)

    DestPath = Session("fso").BuildPath(Application("Root"), DestRelativeToRoot)
    DestPath = Session("fso").GetParentFolderName(DestPath)' strip off the filename
    ThisPath = Request.ServerVariables("PATH_TRANSLATED")
    Referer = PathAbsoluteToRelative(ThisPath, DestPath)' this page URL, relative to the destination
    Referer = Replace(Referer, "\", "/")
    
    Dim s
    Dim bFirst
    bFirst = True
    For Each s In Request.QueryString
        If bFirst = True Then
            Referer = Referer & "?"
            bFirst = False
        Else
            Referer = Referer & "&"
        End If
        Referer = Referer & s & "=" & Request.QueryString(s)
    Next
    
    GetRefererURLFromRoot = GetURLFromRoot(DestRelativeToRoot) & "?referer=" & Server.URLEncode(Referer)
End Function


%>
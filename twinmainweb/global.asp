<%
Option Explicit

Dim g_sPassword
g_sPassword = "JoHnWaYnE"

Sub HandleAuthentication(Password)
    If (""&Password) <> g_sPassword Then Response.Redirect "manageentities.asp"
End Sub

'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
'
Function SqlGetMessageCreatorFromAliasID(aliasid)
    Dim r
    r = "SELECT * FROM WinprogUsers WHERE UserID IN ("
    r = r & "SELECT TransactionUserID FROM WinprogTransactions WHERE TransactionID IN ("
    r = r & "SELECT MessageCreationTransactionID FROM WinprogMessageCreations WHERE MessageCreationMessageID IN ("
    r = r & "SELECT AliasMessageID FROM WinprogAliases WHERE AliasID=" & (0+aliasid) & ") ) )"
    SqlGetMessageCreatorFromAliasID = r
End Function

Function SqlGetMessageCreatorFromAliasText(aliastext)
    Dim r
    r = "SELECT * FROM WinprogUsers WHERE UserID IN ("
    r = r & "SELECT TransactionUserID FROM WinprogTransactions WHERE TransactionID IN ("
    r = r & "SELECT MessageCreationTransactionID FROM WinprogMessageCreations WHERE MessageCreationMessageID IN ("
    r = r & "SELECT AliasMessageID FROM WinprogAliases WHERE AliasText=" & Str2Sql(aliastext) & ") ) )"
    SqlGetMessageCreatorFromAliasText = r
End Function

Function SqlGetMessageCreatorFromMessageID(messageid)
    Dim r
    r = "SELECT * FROM WinprogUsers WHERE UserID IN ("
    r = r & "SELECT TransactionUserID FROM WinprogTransactions WHERE TransactionID IN ("
    r = r & "SELECT MessageCreationTransactionID FROM WinprogMessageCreations WHERE MessageCreationMessageID=" & (0+messageid) & ") )"
    SqlGetMessageCreatorFromMessageID = r
End Function

Function SqlGetUserFromTransactionID(transid)
    Dim r
    r = "SELECT * FROM WinprogUsers WHERE UserID IN ("
    r = r & "SELECT TransactionUserID FROM WinprogTransactions WHERE TransactionID=" & (0+transid) & ")"
    SqlGetUserFromTransactionID = r
End Function

Function SqlGetCreationTransactionFromMessageID(messageid)
    Dim r
    r = "SELECT * FROM WinprogTransactions WHERE TransactionID IN ("
    r = r & "SELECT MessageCreationTransactionID FROM WinprogMessageCreations WHERE MessageCreationMessageID=" & (0+messageid) & ")"
    SqlGetCreationTransactionFromMessageID = r
End Function

Function SqlGetAliasesFromMessageID(messageid)
    Dim r
    r = "SELECT * FROM WinprogAliases WHERE AliasMessageID=" & (0+messageid)
    SqlGetAliasesFromMessageID = r
End Function

Function SqlGetTransactionHistoryFromMessageID(messageid)
    Dim r
    r = "SELECT * FROM WinprogTransactions INNER JOIN WinprogMessageUpdates ON MessageUpdateTransactionID=TransactionID WHERE MessageUpdateMessageID=" & (0+messageid)
    SqlGetTransactionHistoryFromMessageID = r
End Function
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
'
Function GetAliasText(id)
    GetAliasText = GetFirstValueOfQuery("SELECT AliasText FROM WinprogAliases WHERE AliasID=" & (0+id))
End Function

Function GetMessageText(id)
    GetMessageText = GetFirstValueOfQuery("SELECT MessageText FROM WinprogMessages WHERE MessageID=" & (0+id))
End Function

Function GetAliasID(name)
    GetAliasID = GetFirstValueOfQuery("SELECT AliasID FROM WinprogAliases WHERE AliasText=" & Str2Sql(name))
End Function

Function GetAliasMessageID(id)
    GetAliasMessageID = GetFirstValueOfQuery("SELECT AliasMessageID FROM WinprogAliases WHERE AliasID=" & (0+id))
End Function

Function UserRstToMask(rstUser)
    UserRstToMask = Trim(rstUser("UserNick")) & "!" & Trim(rstUser("UserIdent")) & "@" & Trim(rstUser("UserHost"))
End Function


'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
'
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

Sub AliasLink(rst)
    %><a href="details.asp?a=<%=dbu(rst("AliasID"))%>"><%
End Sub


Function CountCSWords (ByVal S)
    ' Counts the words in a string that are separated by commas.
    Dim WC, Pos 

    If VarType(S) <> 8 Or Len(S) = 0 Then
        CountCSWords = 0
        Exit Function
    End If

    WC = 1
    Pos = InStr(S, ",")
    Do While Pos > 0
        WC = WC + 1
        Pos = InStr(Pos + 1, S, ",")
    Loop
    CountCSWords = WC
End Function

Function GetCSWord (ByVal S, Indx)
    ' Returns the nth word in a specific field.
    Dim WC , Count , SPos , EPos 
    WC = CountCSWords(S)
    
    If Indx < 1 Or Indx > WC Then
        GetCSWord = Null
        Exit Function
    End If

    Count = 1
    SPos = 1
    For Count = 2 To Indx
        SPos = InStr(SPos, S, ",") + 1
    Next
    
    EPos = InStr(SPos, S, ",") - 1
    If EPos <= 0 Then EPos = Len(S)
    GetCSWord = Trim(Mid(S, SPos, EPos - SPos + 1))
End Function

Function HTMLEncode(s)
    HTMLEncode = Server.HTMLEncode(s & "")
End Function

Function QueryStringHandOff(s)
    QueryStringHandOff = s & "=" & Request.QueryString(s) & "&"
End Function


Function GetRequestField(s)
    Dim qs, f
    qs = Request.QueryString(s)
    f = Request.Form(s)
    If IsEmpty(qs) or IsNull(qs) Then
        GetRequestField = f
    Else
        GetRequestField = qs
    End If
End Function

%>


<!-- #include File = "form_api.asp"-->
<!-- #include File = "ui_api.asp"-->
<!-- #include File = "db_api.asp"-->
<!-- #include File = "path_api.asp"-->

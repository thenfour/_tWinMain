<%

Function GetFirstValueOfQuery(sql)
    Dim rst
    Set rst = Application("cnn").Execute(sql)
    GetFirstValueOfQuery = rst.Fields(0)
    Set rst = Nothing
End Function

Function Str2Sql(s)
    ' convert ot string if necessary
    Dim s2
    s2 = s & ""
    s2 = Replace(s2, "'", "''")
    Str2Sql = "'" & s2 & "'"
End Function


Function dbf(s)
    dbf = Server.HTMLEncode(Trim(s))
End Function

Function dbu(s)
    dbu = Server.URLEncode(Trim(s))
End Function


' returns True if there was an error
Function TestForSQLError(num, desc, sql)
    TestForSQLError = False
    If num <> 0 Then
        %>
        <p><b>There was an error executing the db command.</b></p>
        <p>SQL: <%=sql%></p>
        <p>Description: <%=desc%></p>
        <%
        TestForSQLError = True
    End If
    
End Function


Function GetURLFromRoot(path)
    GetURLFromRoot = CurrentFileToRelative(Session("fso").BuildPath(Application("Root"), path))
End Function


Function PerformSelect(sql)
    Dim rst
    Set rst = Application("cnn").Execute(sql)
    If Err.number <> 0 Then
        Set PerformSelect = Nothing
        TestForSQLError Err.number, Err.Description, sql
    Else
        Set PerformSelect = rst
    End If

    Set rst = Nothing
End Function

Function FormatSQLDate(d)
    Dim tempd
    
    If IsDate(d) = True Then
        ' convert d from whatever it is to a date.
        tempd = Int(CDate(d))
    Else
        tempd = Date()
    End If
    
    FormatSQLDate = tempd

End Function

%>
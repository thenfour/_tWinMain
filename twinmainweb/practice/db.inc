<%

Function Str2Sql(s)
    ' convert ot string if necessary
    Dim s2
    s2 = s & ""
    s2 = Replace(s2, "'", "''")
    Str2Sql = "'" & s2 & "'"
End Function

Function Str2SqlNoQuotes(s)
    ' convert ot string if necessary
    Dim s2
    s2 = s & ""
    Str2SqlNoQuotes = Replace(s2, "'", "''")
End Function

Function Date2Sql(d)
    If IsDate(d) = True Then
        Date2Sql = Str2Sql(CStr(CDate(d)))
    End If
End Function

' formats database text to be used on the webpage directly.
Function dbf(s)
    dbf = Server.HTMLEncode(Trim(s & ""))
End Function

' formats database NUMBER to TEXT to be used on the webpage directly.
Function dbfn(s)
    dbfn = s
End Function

' just trims it up
Function dbtrim(s)
    dbtrim = Trim(s & "")
End Function

' formats database text to be used in a URL directly
Function dbu(s)
    dbu = Server.URLEncode(Trim(s & ""))
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

' returns False if it was failure, True if it was OK
Function ExecuteSQL(sql)
    Application("cnn").Execute sql
    ExecuteSQL = Not TestForSQLError(Err.number, Err.Description, sql)
End Function

Function PerformSelect(sql)
    Dim rst
    Set rst = Application("cnn").Execute(sql)
    if TestForSQLError(Err.number, Err.Description, sql) Then
        Set PerformSelect = Nothing
    Else
        Set PerformSelect = rst
    End If
    Set rst = Nothing
End Function

Function GetFirstValueOfQuery(sql)
    Dim rst
    Set rst = PerformSelect(sql)
    GetFirstValueOfQuery = rst.Fields(0)
    rst.Close
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


Sub DumpTable(rst)
    %><table class=tmoframe><tr><%
    Dim f
    For Each f in rst.Fields
        %><th><%=dbf(f.Name)%></th><%
    Next
    %></tr><%
    
    Do while not rst.EOF
        %><tr><%
        For Each f in rst.Fields
            %><td><%=dbf(f)%></td><%
        Next
        %></tr><%
        rst.MoveNext
    Loop
    %></table><%
End Sub

%>
<!-- #include File = "global.asp"-->
<%
'   QUERYSTRINGS
'       q = the user search query
'       p = the passcode

'On Error Resume Next


WriteHeader "_tWinMain Web"

Dim Password
Password = Request.QueryString("p")
If (""&Password) <> g_sPassword Then
    RegisterFocusItem "f.p"
    %>
    _tWinMain Web administration authentication
    <form name=f action="manageentities.asp" method=get><input type=text name=p><input type=submit value="Proceed"></form>
    <%
Else
    Dim sOriginalQuery
    sOriginalQuery = Request.QueryString("q")

    RegisterFocusItem "f.q"

    %>
    <form action="manageentities.asp" method=get name=f ID="Form1">
        <input type=hidden name=p value=<%=HTMLEncode(Password)%>>
        <p>Search for users</p><input type=text name=q value="<%=HTMLEncode(sOriginalQuery)%>" ID="Text1"/><input type=submit value="Go" ID="Submit1" NAME="Submit1"/>
    </form>
    <%

    Dim sQuery
    sQuery = "%" & sOriginalQuery & "%"

    Sub BeginTable()
        %><form name=f2 action="entitylink.asp?q=<%=Server.URLEncode(sOriginalQuery)%>&p=<%=Server.URLEncode(g_sPassword)%>" method=post><input type=submit value="Link"><table><%
    End Sub

    Sub EndTable()
        %></table></form><%
    End Sub

    Sub NewEntityGroup(neID, seName)
        %><tr><td colspan=2><input type=checkbox name=<%=neID%>>&nbsp;
        <a href="entitysplit.asp?q=<%=Server.URLEncode(sOriginalQuery)%>&p=<%=Server.URLEncode(g_sPassword)%>&e=<%=Server.URLEncode(neID)%>">[split]
        </a>&nbsp;<%=HTMLEncode(seName)%></td></tr><%
    End Sub

    Sub WriteUser(sMask)
        %><tr><td width=50px>&nbsp;</td><td colspan=1><%=HTMLEncode(sMask)%></td></tr><%
    End Sub

    If Len(sOriginalQuery) > 0 Then
        Dim rst
        Set rst = PerformSelect("EXECUTE WinprogQueryUserEntities " & Str2Sql(sQuery))
        Dim sCurrentEntityName' start with a blank string
        Dim sThisEntityName
        %>Check two or more entities to link them<%
        BeginTable
        While Not rst.EOF
            sThisEntityName = rst("EntityName")
            If sThisEntityName <> sCurrentEntityName Then
                ' start a new entity group
                NewEntityGroup rst("EntityID"), sThisEntityName
                sCurrentEntityName = sThisEntityName
            End If

            WriteUser UserRstToMask(rst)
            
            rst.MoveNext
        Wend
        EndTable
    End If
End If
WriteFooter
%>




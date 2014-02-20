<!-- #include File = "global.asp"-->
<%
'   QUERY STRINGS
'       q = the query string


WriteHeader "_tWinMain Web"

Dim sQuery
sQuery = "" & Request.QueryString("q")
RegisterFocusItem "f.q"
%>
<a href="default.asp">[ search messages ]</a>
<form action="searchusers.asp" method=get name=f>
    <p>Search for users</p><input type=text name=q value="<%=sQuery%>"/><input type=submit value="Go"/>
</form>
<%

If Len(sQuery) > 0 Then
    Dim rst
    Dim sql
    Dim sSqlQuery
    sSqlQuery = Str2Sql("%" & sQuery & "%")
    sql = "EXECUTE WinprogQueryUserEntities " & sSqlQuery & ", 50"
    Set rst = PerformSelect(sql)
    Dim nCount

    nCount = 0
    %><table><%
    Dim sCurrentEntity
    Dim sEntity
    While Not rst.EOF
        sEntity = rst("EntityName")
        If sEntity <> sCurrentEntity Then
            ' new entity; put up the header for it
            %><tr><td colspan=2><a href="userdetails.asp?e=<%=Server.URLEncode(rst("EntityID"))%>"><%=HTMLEncode(sEntity)%></a></td></tr><%
            sCurrentEntity=sEntity
            nCount = nCount + 1
        End If
        %><tr><td width=50px>&nbsp;</td><td><%=HTMLEncode(UserRstToMask(rst))%></td></tr><%
        rst.MoveNext
    Wend
    %></table><%

    %><p><font class="ListCaption"><%
    If nCount = 0 Then
        %>No results were found.<%
    Else
        If nCount = 50 Then
            %>50 or more results were found.<%
        Else
            If nCount = 1 Then
                %>1 result was found.<%
            Else
                %>Approx. <%=nCount%> results were found.<%
            End If
        End If
    End If
    %></font></p><%
    
End If

'/////////////////////////////////////////////////////////////////////////////////////////////////////////
'///////////////////////////////////////////////////////////////////////////////////       BIGGEST HELPERS

'/////////////////////////////////////////////////////////////////////////////////////////////////////////

WriteFooter

%>

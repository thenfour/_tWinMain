<!-- #include File = "global.asp"-->
<%
'   QUERY STRINGS
'       q = the query string


WriteHeader "_tWinMain Web"

Dim sQuery
sQuery = "" & Request.QueryString("q")
RegisterFocusItem "f.q"
%>
<form action="default.asp" method=get name=f>
    <p>Search for help messages</p><input type=text name=q value="<%=sQuery%>"/><input type=submit value="Go"/>
</form>
<%

If Len(sQuery) > 0 Then
    Dim rst
    Dim sql
    Dim sSqlQuery
    sSqlQuery = Str2Sql("%" & sQuery & "%")
    sql = "EXECUTE WinprogWeb_SearchMessages 50, " & sSqlQuery
    Set rst = PerformSelect(sql)
    Dim nCount

    nCount = 0
    While Not rst.EOF
        %><p><font class="Alias"><a href="details.asp?a=<%=Server.URLEncode(rst("AliasID"))%>"><%=HTMLEncode(rst("AliasText"))%></a></font>
        <br/><font class="Message"><%=HTMLEncode(rst("MessageText"))%></font></p><%
        rst.MoveNext
        nCount = nCount + 1
    Wend
    
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
                %><%=nCount%> results were found.<%
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

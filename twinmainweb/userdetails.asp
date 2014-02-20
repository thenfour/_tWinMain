<!-- #include File = "global.asp"-->
<%
' Querystrings:
'       e = entityid
WriteHeader "_tWinMain Web"

' Total usages
' Latest !helps
' Latest !updates
' Latest !msgsets
' Most frequently used help messages

Dim eid, EntityName
eid = CInt(GetRequestField("e"))

Dim rst

EntityName = GetFirstValueOfQuery("select EntityName from WinprogEntities where EntityID = " & eid)

%>
<h1><%=dbf(EntityName)%></h1>
<%



' MOST ALIAS USES ------------------------------------------------------------------------
Dim mostuses

set rst = PerformSelect("execute dbo.WinprogWeb_GetMostUsedMessagesByEntity " & eid & ", 20")

if not rst.EOF Then
    mostuses = rst("Uses")
end if

%><table class=stats><tr><th>Alias</th><th>Uses</th><th>&nbsp;</th></tr><%
do while not rst.EOF
    %><tr><td><% AliasLink(rst) %><%=dbf(rst("AliasText"))%></a></td><td><%=dbf(rst("Uses"))%></td>
    <td><img src="style/red2.gif" height=4 width=<%=CInt(300.0 * (rst("Uses")/mostuses))%> ></td></tr><%
    rst.MoveNext
loop
%></table><%
rst.close
set rst = Nothing


WriteFooter
%>

<!-- #include File = "global.asp"-->
<%
' FORM ITEMS
'   q = query to pass around
'   p = password
'   e = The id of the entity we're dealing with
'   csv = comma separated values list of users
'   ne = New entity name

'On Error Resume Next

HandleAuthentication Request.Form("p")

WriteHeader "_tWinMain Web"

Dim sql
sql = "INSERT INTO WinprogEntities (EntityName) VALUES (" & Str2Sql(Request.Form("ne")) & ")"
'Response.Write sql
Application("cnn").Execute sql

%>
<h3>Success!</h3>
<form name=f action="entitysplit_chooseentity.asp" method=post ID="Form1">
<input type=hidden name=q value="<%=HTMLEncode(Request.Form("q"))%>" ID="Hidden1">
<input type=hidden name=eq value="<%=HTMLEncode(Request.Form("ne"))%>" ID="Hidden6">
<input type=hidden name=p value="<%=HTMLEncode(Request.Form("p"))%>" ID="Hidden2">
<input type=hidden name=csv value="<%=HTMLEncode(Request.Form("csv"))%>" ID="Hidden3">
<input type=hidden name=e value="<%=HTMLEncode(Request.Form("e"))%>" ID="Hidden4">
<input type=submit value="Back to choose entity" ID="Submit1" NAME="Submit1">
</form>
<%

WriteFooter

%>




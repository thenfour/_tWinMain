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

%>
<h3>Confirmation</h3>
<p>Please verify that the information is correct.  You are creating a new entity named:</p>
<h4><%=HTMLEncode(Request.Form("ne"))%></h4>
<form name=f action="entitysplit_new_2.asp" method=post>
<input type=hidden name=q value="<%=HTMLEncode(Request.Form("q"))%>" ID="Hidden1">
<input type=hidden name=p value="<%=HTMLEncode(Request.Form("p"))%>" ID="Hidden2">
<input type=hidden name=csv value="<%=HTMLEncode(Request.Form("csv"))%>" ID="Hidden3">
<input type=hidden name=e value="<%=HTMLEncode(Request.Form("e"))%>" ID="Hidden4">
<input type=hidden name=ne value="<%=HTMLEncode(Request.Form("ne"))%>" ID="Hidden5">
<input type=submit value="Next »">
</form>
<%

WriteFooter

%>




<!-- #include File="global.asp" -->
<%WriteHeader "SOF - Logoff"
%>
Goodbye.
<%
WriteFooter

Session.Abandon
%>
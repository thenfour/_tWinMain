<!-- #include File="global.asp" -->
<%
WriteHeader "Practice"

DumpTable PerformSelect("select top 100 * from tDates")
%>
<%WriteFooter%>

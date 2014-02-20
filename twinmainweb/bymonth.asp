<!-- #include File = "global.asp"-->
<%
WriteHeader "_tWinMain Web"

WhipUpATable "Transactions by month", "select count(TransactionID) TransactionCount, dbo.DateToYYYYMM(TransactionDate) YYYYMM from WinprogTransactions group by dbo.DateToYYYYMM(TransactionDate) order by YYYYMM", "TransactionCount"



WriteFooter
%>

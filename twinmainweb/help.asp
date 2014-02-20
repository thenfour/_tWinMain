<!-- #include File = "global.asp"-->
<%

WriteHeader "_tWinMain Web"
%>
<h1>Frequently Asked Questions</h1>

<%

Dim nCurrentQuestion
nCurrentQuestion = 1

Sub WriteQuestion(q)
    %><h4><%=nCurrentQuestion%>: <%=q%></h4>
<%
    nCurrentQuestion = nCurrentQuestion+1
End Sub

Sub WriteAnswer(a)
    %><p><%=a%></p>
<%
End Sub


WriteQuestion("What's with the 'assimil@ted' hostmasks (and all other seemingly messed up information)?")
WriteAnswer("The original database did not store detailed information - specifically ident and host information." & _
    " When converting to the new database format, I had to find a way to assimilate all the existing data so I wouldn't have " & _
    " to start the usage statistics from scratch, and to 'fill in' all the missing information.  'assimil@ted' is the ident/host " & _
    " combination I decided to use to solve this.")

WriteQuestion("Why does one person (someone!assimil@ted) have a huge number of usage statistics?")
WriteAnswer("Related to the previous answer... because the old database didn't keep detailed statistics, I had to find a way of " & _
    " filling in usage data.  If a message was used 1000 times, I had to find a way to fake 1000 uses so the " & _
    "statistic would carry over accurately.  I decided to use the creator of the message and simulate them " & _
    "using the message x number of times.")

WriteQuestion("How do I tell this thing that someone was using a different host or nickname, to 'link' multiple " & _
    "user nick/ident/host information to another?  It's screwing up the statistics otherwise.")
WriteAnswer("Go to #winprog and shout at tenfour.")
    
WriteQuestion("Someone used my nickname and screwed up the statistics.  How do I separate this guy?")
WriteAnswer("Go to #winprog and shout at tenfour.")


WriteFooter

%>

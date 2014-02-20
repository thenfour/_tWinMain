<!-- #include File="global.asp" -->
<%

WriteHeader "SOF - Info"

dim a

    %>
    <table class=tmoframe>
    <%
    On Error Resume next
    
    For each a in Request.ServerVariables
        Response.Write "<tr><td nowrap>Request.ServerVariables(""" & Server.HTMLEncode(a) & """)</td><td>" & Server.HTMLEncode(Request.ServerVariables(a)) & "</td></tr>"
    Next
    %></table>
    <table class=tmoframe><%
    For each a in Application.Contents
        %><tr><td nowrap>Application("<%=Server.HTMLEncode(a)%>")&nbsp;</td><td><%=Server.HTMLEncode(Application(a))%>&nbsp;</td></tr><%
    Next
    %>
    </table>
    <table class=tmoframe><%
    For each a in Session.Contents
        %><tr><td nowrap>Session("<%=Server.HTMLEncode(a)%>")&nbsp;</td><td><%=Server.HTMLEncode(Session(a))%>&nbsp;</td></tr><%
    Next
    %>
    </table>
    <%

WriteFooter%>
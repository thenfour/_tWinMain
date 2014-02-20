<%

' If you register an item to set focus to, this will be true.
Dim g_bRegisteredFocusItem
Dim g_sSetFocusScriptText

Sub WriteHomeLink()
    %><a href="<%=GetURLFromRoot(".")%>">[ search messages ]</a><%
End Sub
Sub WriteHeaderBare(strTitle)
    g_bRegisteredFocusItem = False
    %><HTML><HEAD><title><%=strTitle%></title><LINK href='<%=CurrentFileToRelative(Application("StyleSheet"))%>' type="text/css" rel="stylesheet">
</HEAD><body onload="sf();><%
End Sub

Sub WriteFooterBare()
    %><P><img class="line"></P><P><center><SMALL>All rights reserved.</SMALL></center><P></P></body></HTML><%
End Sub

Sub WriteHeader(strTitle)
    g_bRegisteredFocusItem = False
    %><HTML>
<HEAD>
<title><%=strTitle%></title>
<LINK href='<%=CurrentFileToRelative(Application("StyleSheet"))%>' type="text/css" rel="stylesheet">
</HEAD>
<body onload="sf();">
<P>
<TABLE id="Table2" width="100%">
<TR>
<TD><p align="left"><a href="<%=GetURLFromRoot(".")%>"><img border="0" src='<%=CurrentFileToRelative(Application("LogoURL"))%>'></a></p></TD>
<TD><p align="right"><%WriteHomeLink%><br>
<a href="searchusers.asp">[ search users ]</a><br>
<a href="globaldetails.asp">[ global statistics ]</a><br>
<a href="help.asp">[ FAQ ]</a><br>
<%
Dim p
p = Request.Form("p") & Request.QueryString("p")
If Len(p) > 0 Then
    %><a href="manageentities.asp?p=<%=Server.URLEncode(p)%>"><font color=#FFFFFF>[ admin ]</font></a><br><%
Else
    %><a href="manageentities.asp"><font color=#FFFFFF>[ admin ]</font></a><br><%
End If%>
</p>
</TD>
</TR>
</TABLE>
<img class="line">
</P>
<P align="center">
<TABLE id="Table1" width="800">
<TR>
<TD>
<%
End Sub

Sub RegisterFocusItem(sItem)' sItem is the name of a control under a document.
    ' sItem = "f.q" for this example
    ' document.f.q.focus();
    g_sSetFocusScriptText = "document." & sItem & ".focus();"
End Sub


Sub WriteFooter()
%>
</TD>
</TR>
</TABLE>
</P>
<P><img class="line"></P>
<P><center><SMALL>All rights reserved.</SMALL></center></P>
<%
If g_bRegisteredFocusItem = False Then
    ' write out the default setfocus script
    %>
<script>
<!--
function sf(){<%=g_sSetFocusScriptText%>}
//-->
</script>
<%
End If
%>
</body>
</HTML>
<%
End Sub



Sub WhipUpATable(Title, sql, count_col)
    Dim rst
    Dim f
    Dim maximum_val
    Set rst = PerformSelect(sql)
    Dim bCounts
    
    If (Len(count_col) > 0) and (not rst.EOF) then
        bCounts = True
        maximum_val = rst(count_col)
    end if
    
    %><table class=stats><caption><%=Title%></caption><tr><%
    For Each f In rst.Fields
        %><th><%=HTMLEncode(f.Name)%></th><%
    Next
    
    If bCounts Then
        %><th>&nbsp;</th><%
    End If
    
    %></tr><%
    While NOT rst.EOF
        %><tr><%
        For Each f In rst.Fields
            %><td><%=HTMLEncode(f.Value)%></td><%
        Next
        If bCounts Then
            %><td><img src="style/red2.gif" height=4 width=<%=CInt(300.0 * (rst(count_col)/maximum_val))%> ></td><%
        End If
        %></tr><%
        rst.MoveNext
    Wend
    %></table><%
End Sub



%>
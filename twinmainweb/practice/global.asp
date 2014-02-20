<%
Option Explicit
On Error Resume Next

Sub dbg(s)
End Sub

%>
<!-- #include file="siteinfo.inc" -->
<!-- #include file="db.inc" -->
<!-- #include file="path.inc" -->
<!-- #include file="practice.inc" -->
<%
' If you register an item to set focus to, this will be true.
Dim g_sSetFocusScriptText

' If you have one or more collapsable <span>, we need to add some javascript.  This flag will
' make sure it's only added once
Dim g_bCollapseScriptAdded

Sub WriteHeader(title)
    WriteHeaderBA title, "onload=sf();"
End Sub

Sub WriteHeaderBA(title, bodyattributes)
    %><html><head><title><%=Server.HTMLEncode(title)%></title><LINK href='style.css' type="text/css" rel="stylesheet"></head>
    <body <%=bodyattributes%> >
    <table class=g height=100% width=100% cellspacing=0>
    <tr><td colspan=2 style="height: 1px;" align="left"><img src='logo.gif'></td>
    <td style="height: 1px; text-align: right;">
    <%If (g_bTest = True) Then%>
        <font color=red><b>TEST SITE ONLY.</b></font><br>
    <%End If%>
    </td>
    <p>
    </tr><tr><td colspan=3 style="height: 1px; padding: 0; width: 100%; border-bottom: 2px solid black"></tr>
    <tr><td style="height: 100%; padding-right: 10px; width: 1%; border-right: 2px solid black">
    <a href="default.asp">[&nbsp;home&nbsp;]</a><br/>
    </p>
    <p>
    <a href="new_session.asp">[&nbsp;NewSession&nbsp;]</a><br/>
    </p>
    <p>
    <a href="instruments.asp">[&nbsp;Instruments&nbsp;]</a><br/>
    <a href="musicians.asp">[&nbsp;Musicians&nbsp;]</a><br/>
    <a href="books.asp">[&nbsp;Books&nbsp;]</a><br/>
    <a href="lessons.asp">[&nbsp;Lessons&nbsp;]</a><br/>
    <a href="skills.asp">[&nbsp;Skills&nbsp;]</a><br/>
    <a href="rankings.asp">[&nbsp;Rankings&nbsp;]</a><br/>
    </p>
    <a href="info.asp">[&nbsp;SiteInfo&nbsp;]</a><br/>
    <a href="logoff.asp">[&nbsp;logoff&nbsp;]</a><br/>
    </td><td style="width: 99%;" colspan=2><div class=mainbody>
<%End Sub

Sub WriteFooter()
    %></div></td></tr><tr><td colspan=3 style="height: 1px; border-top: 2px solid black">
    <%
    If g_bTest Then
        WriteFormData
    Else
        %>
        <p><center><small>©2003 Carl Corcoran, Inc.<br>
        The contents of this website are confidential.<br>All rights reserved.
        </small></center></p>
        <%
    End If
    %>
    </td></tr></table>
    <%
    ' write out the default setfocus script
    %><script>
    <!--
    function sf(){<%=g_sSetFocusScriptText%>}
    //-->
    </script>
    </body>
    </html><%
End Sub

Sub RegisterFocusItem(sItem)' sItem is the name of a control under a document.
    ' sItem = "f.q" for this example
    ' document.f.q.focus();
    g_sSetFocusScriptText = "document." & sItem & ".focus();"
End Sub

Function CreateGuid()
    Dim tl
    Set tl = Server.CreateObject("Scriptlet.TypeLib")
    CreateGuid = tl.Guid
    Set tl = Nothing
End Function

Sub WriteEditBox(caption, name, value, bEnabled)
    Dim EnabledText, ValueText
    EnabledText = " "
    If bEnabled = False Then EnabledText = " Disabled "
    ValueText = " "
    If value <> "" Then ValueText = "value=""" & Server.HTMLEncode(value) & """"
    %><P><%=caption%><INPUT maxlength=50 type="text" <%=EnabledText%> name="<%=name%>" <%=ValueText%> style="width:100%" ID="Text1"></P><%
End Sub

Sub WriteTextArea(caption, name, value, bEnabled)
    Dim EnabledText
    EnabledText = " "
    If bEnabled = False Then EnabledText = " Disabled "
    %><P><%=caption%>
    <textarea rows="12" <%=EnabledText%> wrap=virtual style="width:100%" name="<%=name%>" ID="Textarea1"><%=Server.HTMLEncode(value & "")%></textarea></p>
    <%
End Sub

Sub WriteYesNo(caption, name, bValue, bEnabled)
    Dim EnabledText
    EnabledText = " "
    If bEnabled = False Then EnabledText = " Disabled "
    %><P><%=caption%>
<!--    <SELECT style="width:100%" name="<%=name%>" <%=EnabledText%>>
        <%
        If bValue = True Then
            %>
            <OPTION selected>Yes</OPTION>
            <OPTION>No</OPTION>
            <%
        Else
            %>
            <OPTION>Yes</OPTION>
            <OPTION selected>No</OPTION>
            <%
        End If
    %></SELECT>-->
    <%
        If bValue = True Then
            %><INPUT type="checkbox" NAME="<%=name%>"  <%=EnabledText%> checked ID="Checkbox1"><%
        Else
            %><INPUT type="checkbox" NAME="<%=name%>"  <%=EnabledText%> ID="Checkbox2"><%
        End If
    %>
    <%
End Sub

Sub WriteFormData()
    %><pre><%
    Dim x
    Response.Write "<font color=blue>Request.Form.Count=" & Request.Form.Count & "</font>" & vbcrlf
    For Each x In Request.Form
        Response.Write "Request.Form(""" & x & """)=" & Request.Form(x) & vbcrlf
    Next
    Response.Write "<font color=blue>Request.QueryString.Count=" & Request.Form.Count & "</font>" & vbcrlf
    For Each x In Request.QueryString
        Response.Write "Request.QueryString(""" & x & """)=" & Request.QueryString(x) & vbcrlf
    Next
    %></pre><%
End Sub

Function StringToBool(s)
    Dim s2
    s2 = LCase(s)
    StringToBool = False
    If s2 = "yes" Or s2 = "true" Then StringToBool = True
End Function

' converts checkbox form results to a boolean True or False
Function CheckboxToBool(c)
    Dim lc
    lc = LCase(c)
    CheckboxToBool = False
    If lc = "on" Then CheckboxToBool = True
End Function

' you need to put the "</a>" by yourself.
Function GetViewEmployeeLinkURL(employeeid)
    GetViewEmployeeLinkURL = "viewemployee.asp?id=" & Server.URLEncode(employeeid)
End Function

' if i = 1, returns True.  if it's 0 or anything else, returns False
Function Int2Bool(i)
    Int2Bool = False
    If i = True Then Int2Bool = True
End Function

' retrieves a value from either QueryString() or Form(), whichever EXISTS
Function GetRequestField(s)
    If IsEmpty(Request.QueryString(s)) Then
        GetRequestField = Request.Form(s)
    Else
        GetRequestField = Request.QueryString(s)
    End If
End Function

Function WriteCheckAllCheckbox(formname)
    %>
    <input type=checkbox name="<%=formname%>_SelectAll" onClick="<%=formname%>_mojo(document.<%=formname%>);">
    <SCRIPT language=JavaScript>
    <!--
    function <%=formname%>_mojo(f)
    {
        var dval = false;
        if(f.elements["<%=formname%>_SelectAll"].checked) dval = true;
        for(var i=0;i<f.length;i++)
        {
            if((f.elements[i].type=="checkbox") && (f.elements[i].name != "<%=formname%>_SelectAll"))
            {
                f.elements[i].checked = dval;
            }
        }
    }
    //-->
    </SCRIPT>
    <%
End Function

Function Str2JScript(s)
    Str2JScript = Replace(s, "'", "''")
End Function

Sub WriteCollapseButton(id, bVisible)
    If g_bCollapseScriptAdded <> True Then
        %>
        <SCRIPT language=javascript>
        <!--
        function Collapse_ToggleView(id)
        {
            var thespan;
            var theimg;
            thespan = document.getElementsByName('Collapse_Span_' + id)[0];
            theimg = document.getElementsByName('Collapse_Button_' + id)[0];
            if(thespan.style.display == "none")
            {
                thespan.style.display = "";
                theimg.src = "minus.gif";
            }
            else 
            {
                thespan.style.display = "none";
                theimg.src = "plus.gif";
            }
        }
        //-->
        </SCRIPT>
        <%
        g_bCollapseScriptAdded = True
    End If
    
    If bVisible = True Then
        %>
        <img id='Collapse_Button_<%=id%>' onClick='Collapse_ToggleView("<%=id%>");' src='minus.gif'>
        <%
    Else
        %>
        <img id='Collapse_Button_<%=id%>' onClick='Collapse_ToggleView("<%=id%>");' src='plus.gif'>
        <%
    End If
End Sub

Sub BeginCollapsableSection(id, bVisible)
    %><span id="Collapse_Span_<%=id%>" <% If bVisible = False Then %> style="display: none;" <%End If %> ><%
End Sub

Sub EndCollapsableSection(id)
    %></span><%
End Sub

%>

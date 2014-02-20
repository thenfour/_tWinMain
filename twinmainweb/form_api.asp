<%

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
    Dim x
    For Each x In Request.Form
        Response.Write "Request.Form(" & x & ")=" & Request.Form(x) & "<br>"
    Next
    For Each x In Request.QueryString
        Response.Write "Request.QueryString(" & x & ")=" & Request.QueryString(x) & "<br>"
    Next
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

%>
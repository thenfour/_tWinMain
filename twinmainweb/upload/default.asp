<!-- #include File = "..\global.asp"-->
<!--#INCLUDE FILE="clsUpload.asp"-->
<%

function FormatFileSize(n)
  dim r
  if n < 1024 then
    r = CStr(n) & " bytes"
  elseif n < 1023000 then
    r = FormatNumber((n / 1024), 2, , , True) & " kb"
  else
    r = FormatNumber((n / 1024), 0, , , True) & " kb"
  end if
  FormatFileSize = r
end function

function FormatSeconds(s)
    dim min, sec
    sec = s mod 60
    min = (s - sec) / 60
    FormatSeconds = min & " min " & sec & " secs"
end function

function GetLocalFilename(cookie)
    GetLocalFilename = Server.MapPath(".\bin\" & cookie)
end function

sub WriteIcon(filename)
    Dim ext, icon
    ext = session("fso").GetExtensionName(filename)
    Select Case LCase(ext)
        Case "jpg" icon = "bmp.gif"
        Case "gif" icon = "bmp.gif"
        Case "bmp" icon = "bmp.gif"
        Case "png" icon = "bmp.gif"
        Case "jpeg" icon = "bmp.gif"

        Case "mid" icon = "wav.gif"
        Case "mp3" icon = "wav.gif"
        Case "wav" icon = "wav.gif"

        Case "rar" icon = "rar.gif"
        Case "zip" icon = "zip.gif"

        Case "mdb" icon = "mdb.gif"
        Case "asp" icon = "asp.gif"
        Case "cpp" icon = "cpp.gif"
        Case "c" icon = "cpp.gif"
        Case "cxx" icon = "cpp.gif"
        Case "h" icon = "h.gif"
        Case "hxx" icon = "h.gif"
        Case "hpp" icon = "h.gif"
        
        Case "txt" icon = "txt.gif"
        Case "doc" icon = "doc.gif"
        Case "ppt" icon = "ppt.gif"
        Case "pdf" icon = "pdf.gif"
        Case "vsd" icon = "vsd.gif"
        Case "xls" icon = "xls.gif"
        Case Else
            icon = "unknown.gif"
    End Select
    %><img src="ext\<%=icon%>">&nbsp;<%
end sub

' do house cleaning
Dim rst
Dim sql
Set rst = PerformSelect("exec WhatNeedsCleaning")

while not rst.eof
    ' delete this file
    'response.write GetLocalFilename(rst("Cookie"))
    if Session("fso").FileExists(GetLocalFilename(rst("Cookie"))) then
        Session("fso").DeleteFile GetLocalFilename(rst("Cookie"))
    end if
    Application("cnn").Execute "exec dbo.SetDeleted @c=" & str2sql(rst("Cookie"))
    rst.movenext
wend

set rst = Nothing

Response.Expires = 0

'- house cleaning
'- get info on ID, verify timeout
'- get filename:
'    - display form.
'- upload file:
'    - update table and say its been uploaded

Dim maxsize
Dim guid
Dim uploaded
Dim bValid
Dim Expires
Dim Filename
Dim TimeNow

' the ID is invalid until we determine that its good.
guid = request.querystring("id")
bValid = False

sql = "select Bytes, Cookie, Uploaded, Filename, Expires, GetDate() as [now] from Uploads where Cookie = " & Str2Sql(guid)
Set rst = PerformSelect(sql)
if not rst.EOF then
    maxsize = rst("Bytes")
    guid = rst("Cookie")
    uploaded = rst("Uploaded")
    Expires = rst("Expires")
    TimeNow = rst("now")
    Filename = rst("Filename")
    bValid = True
end if

If bValid = False Then
    if guid = "" then
        ' no id was supplied at all... just show recent uploads
        WriteHeader "_tWinMain Web"
        %><h2>Recently uploaded files</h2><%
        set rst = PerformSelect("exec dbo.RecentUploads")
        Dim c
        c = 0
        while not rst.eof
            WriteIcon rst("filename")
            %>[<%=server.htmlencode(rst("nick"))%>] <a href="default.asp?id=<%=server.urlencode(rst("Cookie"))%>&v=y"><%=server.htmlencode(rst("Filename"))%></a> (<%=FormatFileSize(rst("realsize"))%>, expires in <%=FormatSeconds(rst("secondsleft"))%>)<br><%
            rst.movenext
            c = c + 1
        wend
        
        if c = 0 then
            %>(none)<%
        end if

        %><h2>Expired files</h2><%
        set rst = PerformSelect("exec dbo.ExpiredUploads")
        c = 0
        while not rst.eof
            WriteIcon rst("filename")
            %>[<%=server.htmlencode(rst("nick"))%>] <%=server.htmlencode(rst("Filename"))%> (<%=FormatFileSize(rst("realsize"))%>)<br><%
            rst.movenext
            c = c + 1
        wend
        
        if c = 0 then
            %>(none)<%
        end if

        WriteFooter
    else
        WriteHeader "_tWinMain Web"
        %>Invalid ID.<%
        WriteFooter
    end if
ElseIf TimeNow >= Expires Then
    WriteHeader "_tWinMain Web"
    %>Sorry, this upload has expired.<%
    WriteFooter
ElseIf Uploaded = True Then
    ' show the download page for it
    if request.querystring("v") <> "y" then
        WriteHeader "_tWinMain Web"
        WriteIcon Filename
        %><a href="default.asp?id=<%=server.urlencode(guid)%>&v=y">Click here to download "<%=server.htmlencode(Filename)%>"</a><%
        WriteFooter
    else
        Dim objRead
        Dim filePath
        Dim fileSize
        Dim ext
        'Const adTypeBinary = 1
    
        filePath = GetLocalFilename(guid)
        fileSize = Session("fso").GetFile(filePath).Size

        Set objRead = Server.CreateObject("ADODB.Stream")
        objRead.Open
        objRead.Type = adTypeBinary
        objRead.LoadFromFile filePath
    
        Response.Buffer = True
        Response.Clear
        'Response.ContentType = "application/download"
        Response.AddHeader "Content-disposition", "inline"
        Response.AddHeader "Content-Length", fileSize
        Response.Charset = "UTF-8"

        ext = session("fso").GetExtensionName(filename)
        Select Case LCase(ext)
            Case "jpg" Response.ContentType = "image/jpeg"
            Case "jpeg" Response.ContentType = "image/jpeg"
            Case "gif" Response.ContentType = "image/gif"
            Case "png" Response.ContentType = "image/png"
            Case Else Response.ContentType = "application/octet-stream" : Response.AddHeader "Content-disposition", "attachment;filename=" & FileName
        end select
        
        Response.BinaryWrite objRead.Read
        Response.Flush
        Response.End
        
        Set objRead = Nothing        
        Set rstFile = Nothing
    end if
Else
    ' the user may be trying to upload the file, or they may need to be prompted while it uploads.
    'writeformdata
    if Request.Querystring("p") = "y" then
        ' process an uploaded file
        Dim sOrgFilename
        Dim objUpload
        Dim strFileName
        Set objUpload = New clsUpload
        'response.write objUpload.Fields("File").Length
        sOrgFilename = objUpload.Fields("File").FileName
        If Len(sOrgFilename) = 0 Then
            WriteHeader "_tWinMain Web"
            %>
            There was a problem processing the file.  Possibly a browser incompatibility?
            <%
            WriteFormData
            WriteFooter
        Else
            If objUpload.Fields("File").Length > maxsize Then
                WriteHeader "_tWinMain Web"
                %><p>The file is <%=FormatFileSize(objUpload.Fields("File").Length)%>, which exceeds the limit of <%=FormatFileSize(maxsize)%>.  Try again.</p><%
                ShowUploadForm
                WriteFooter
            ElseIf objUpload.Fields("File").Length = 0 Then
                WriteHeader "_tWinMain Web"
                %><p>No File Specified. Try again.</p><%
                ShowUploadForm
                WriteFooter
            Else
                WriteHeader "_tWinMain Web"
                strFileName = GetLocalFilename(guid)
                objUpload("File").SaveAs strFileName
                sql = "execute dbo.SetUploaded @c=" & str2sql(guid) & ", @filename=" & str2sql(sOrgFilename) & ", @size=" & objUpload.Fields("File").Length
                'response.write "<pre>" & sql & "</pre>"
                Application("cnn").Execute(sql)
                %><p>Successfully uploaded.  Use the link below to view the file.</p>
                <a href="http://tenfour.ath.cx:85/upload/?id=<%=guid%>">http://tenfour.ath.cx:85/upload/?id=<%=guid%></a>
                <%
                WriteFooter
            End if
        End If
        Set objUpload = Nothing
    else
        WriteHeader "_tWinMain Web"
        %>Upload a new file.<%
        ShowUploadForm
        WriteFooter
    end if
End If

Sub ShowUploadForm()
    ' let the user upload a new file
    %>
    You may upload a maximum of <%=FormatFileSize(maxsize)%>.
    <FORM method="post" encType="multipart/form-data" action="default.asp?id=<%=server.urlencode(guid)%>&p=y">
    Select the file to upload:
    <INPUT type="File" name="File">
    <INPUT type="Submit" value="Upload" NAME="Submit1">
    </FORM>
    <%
End Sub

%>

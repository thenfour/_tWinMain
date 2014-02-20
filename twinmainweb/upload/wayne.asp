<!-- #include File = "..\global.asp"-->
<!--#INCLUDE FILE="clsUpload.asp"-->
<%
WriteHeader "_tWinMain Web"

Response.Expires = 0

%>
    <FORM method="post" encType="multipart/form-data" action="test.asp?hi=yes">
    Select the file to upload:
    <INPUT type="File" name="File">
    <input type=hidden name="State" value="hi">
    <INPUT type="Submit" value="Upload" NAME="Submit1">
    </FORM>
<%


' process an uploaded file
Dim objUpload
Dim strFileName


if request.querystring("hi") = "yes" then
    Set objUpload = New clsUpload
    Response.Write "FILE INFO:<br>"
    Response.Write "Filename = " & objUpload.Fields("File").FileName & "<br>"
    Response.Write "Length = " & objUpload.Fields("File").Length & "<br>"
end if

WriteFooter
%>
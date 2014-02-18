sep 9 2004 carlc (carl@ript.net)
i have tried pretty hard to include as much as possible, without giving away passwords and other administrative stuff.

IMPORTANT: i am provide the sources as a reference - not as a package you can use to make your own _tWinMain.  I am not granting anyone permission to use these sources at all.  I am liberal about the use of my code, but I am just too lazy to think about licensing, so if you really want to use this for your own IRC chan or other purposes, just email me (carl@ript.net) and ask if you can use it.

There are 2 projects here... "cmdline" and "helper".  Helper is all the good stuff - including the code for !conv, !help (and related), and !upload.  It's a mIRC plugin DLL.  Cmdline is a mirc simulator that let's you use helper without being on IRC.  It's basically a small test app.

If you are REALLY bored and want to set this sucker up for personal use, you will need to do some things:
1) set up a mssql (or msde) database.  I included the create scripts for everything in the \assets\ dir.  Not sure, but you may have to add some content manually to the db to make it work.  I am not even sure the create scripts work correctly.
2) add a connection string to the .ini file.  The .ini file is also in \assets\, and needs to be copied to the same location as the EXE that hosts helper.dll
3) integrate it into mirc or whatever

Questions i can imagine being asked:

Q: Where is the code to !eval?
A: thats a separate project of mine i might include in the future.  It's not part of helper.dll

Q: ur code is teh sux
A: yea, a lot of this was written when i thought a database was just a couple tables and the only way to get at it was using select stataments on single tables.  So there's a lot of inefficient sql stuff happening, and general bad design, but get over it - it works.
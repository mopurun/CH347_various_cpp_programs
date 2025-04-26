@cd C:\Program Files\Microsoft Visual Studio\2022\Community
@set destdir=%~dp0
cl /Os /c /Fo%destdir%hello.obj %destdir%hello.cpp
link %destdir%hello.obj /OUT:%destdir%hello.exe ^
/MACHINE:X64 /SUBSYSTEM:CONSOLE /LIBPATH:%destdir%lib
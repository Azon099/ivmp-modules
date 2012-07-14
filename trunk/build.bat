call "c:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat"
devenv.exe Modules.sln /build Release

cd iv-mysql
call release.bat
cd ..\iv-pawn
call release.bat
cd ..\iv-irc
call release.bat
cd ..

cd Binary\modules\
erase *.pdb
erase *.lib
erase *.exp
cd ..\..\
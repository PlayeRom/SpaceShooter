md SpaceShooter_release_v052
cd SpaceShooter_release_v052
copy ..\..\OpenGLFramework\readme.txt readme.txt
md SpaceShooter_release_v052
cd SpaceShooter_release_v052

md fonts
md languages
md objects
md sounds
md music
md textures

copy ..\..\..\OpenGLFramework\OpenGLFramework\fonts\*.ttf fonts\*.ttf
copy ..\..\..\OpenGLFramework\OpenGLFramework\languages\languages_def.xml languages\languages_def.xml
copy ..\..\..\OpenGLFramework\OpenGLFramework\languages\*.lng languages\*.lng
copy ..\..\..\OpenGLFramework\OpenGLFramework\objects\*.3DObj objects\*.3DObj
copy ..\..\..\OpenGLFramework\OpenGLFramework\sounds\*.wav sounds\*.wav
copy ..\..\..\OpenGLFramework\OpenGLFramework\music\*.ogg music\*.ogg
copy ..\..\..\OpenGLFramework\OpenGLFramework\textures\*.jeh textures\*.jeh

copy ..\..\..\OpenGLFramework\OpenGLFramework\SpaceShooter.exe SpaceShooter.exe
copy ..\..\..\OpenGLFramework\OpenGLFramework\levels.dat levels.dat
copy ..\..\..\OpenGLFramework\OpenGLFramework\ships.dat ships.dat
copy ..\..\..\OpenGLFramework\OpenGLFramework\alut.dll alut.dll
copy ..\..\..\OpenGLFramework\OpenGLFramework\libfreetype-6.dll libfreetype-6.dll
copy ..\..\..\OpenGLFramework\OpenGLFramework\ogg.dll ogg.dll
copy ..\..\..\OpenGLFramework\OpenGLFramework\OpenAL32.dll OpenAL32.dll
copy ..\..\..\OpenGLFramework\OpenGLFramework\SDL.dll SDL.dll
copy ..\..\..\OpenGLFramework\OpenGLFramework\SDL_ttf.dll SDL_ttf.dll
copy ..\..\..\OpenGLFramework\OpenGLFramework\vorbis.dll vorbis.dll
copy ..\..\..\OpenGLFramework\OpenGLFramework\vorbisfile.dll vorbisfile.dll
copy ..\..\..\OpenGLFramework\OpenGLFramework\zlib1.dll zlib1.dll
copy ..\..\..\Instrukcja_uzytkownika\*.pdf *.pdf
cd..
cd..
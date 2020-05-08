md SpaceShooter_release_v051
cd SpaceShooter_release_v051
copy ..\readme.txt readme.txt
md SpaceShooter_release_v051
cd SpaceShooter_release_v051

md fonts
md languages
md objects
md sounds
md textures

copy ..\..\OpenGLFramework\fonts\*.ttf fonts\*.ttf
copy ..\..\OpenGLFramework\languages\languages_def.xml languages\languages_def.xml
copy ..\..\OpenGLFramework\languages\*.lng languages\*.lng
copy ..\..\OpenGLFramework\objects\*.3DObj objects\*.3DObj
copy ..\..\OpenGLFramework\sounds\*.wav sounds\*.wav
copy ..\..\OpenGLFramework\textures\*.jeh textures\*.jeh

copy ..\..\OpenGLFramework\SpaceShooter.exe SpaceShooter.exe
copy ..\..\OpenGLFramework\levels.dat levels.dat
copy ..\..\OpenGLFramework\ships.dat ships.dat
copy ..\..\OpenGLFramework\alut.dll alut.dll
copy ..\..\OpenGLFramework\libfreetype-6.dll libfreetype-6.dll
#copy ..\..\OpenGLFramework\ogg.dll ogg.dll
copy ..\..\OpenGLFramework\OpenAL32.dll OpenAL32.dll
copy ..\..\OpenGLFramework\SDL.dll SDL.dll
copy ..\..\OpenGLFramework\SDL_ttf.dll SDL_ttf.dll
#copy ..\..\OpenGLFramework\vorbis.dll vorbis.dll
#copy ..\..\OpenGLFramework\vorbisfile.dll vorbisfile.dll
copy ..\..\OpenGLFramework\zlib1.dll zlib1.dll
copy ..\..\..\Instrukcja_uzytkownika\*.pdf *.pdf
cd..
cd..
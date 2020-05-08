#pragma once

#include "resource.h"

/** \mainpage
 *
 * \section intro_sec Wst�p
 *
 * OpenGLFramework jest to prosty szablon, na podstawie kt�rego mo�emy budowa� aplikacje (w tym gry) oparte
 * na bibliotece OpenGL. I takie te� by�o jego g��wne za�o�enie. OpenGLFramework zawiera wi�c podstawowe
 * funkcje zwi�zane chyba z ka�d� gr�. Ca�� reszt�, tj. funkcjonalno��, mechanizm gry nale�y ju� wykona� we
 * w�asnym zakresie, dok�adaj�c do opisywanego tu szablonu w�asne klasy/modu�y. \n
 * Na stan obecny OpenGLFramework zawiera:
 * - wczytywanie tekstur (nearest, linear oraz mipmap) z plik�w BMP oraz TGA,
 * - multiteksturing,
 * - obs�uga d�wi�ku za pomoc� biblioteki FMOD oraz OpenAL,
 * - wy�wietlanie tekstu � a� pi�� mo�liwo��,
 * - obliczanie i wy�wietlanie FPS (ilo�� generowanych klatek na sekund�),
 * - kontrola pr�dko�ci animacji, dzi�ki czemu animacje wy�wietlane b�d� z pr�dko�ci� niezale�n� od FPS,
 * - prosty profiler do sprawdzania czasu wykonania si� r�nych funkcji,
 * - konsola do wy�wietlania i wprowadzania komunikat�w,
 * - wspieranie UNICODE,
 * - wspieranie wieloj�zyczno�ci,
 * - wspieranie o�wietlania i materia��w,
 * - pe�noekranowy antyaliasing,
 * - logger,
 * - tryb 2D oraz 3D z mo�liwo�ci� prze��czania si�,
 * - wykorzystanie VertexArrays,
 * - wykorzystanie Vertex Buffer Object,
 * - w�asny format pliku 3DObj dla modeli 3D,
 * - w��czanie / wy��czanie synchronizacji pionowej,
 * - emboss bump mapping (mapowanie wyboj�w),
 * - prosty system cz�steczkowy,
 * - rysowanie billboard'�w (sprite'�w zawsze zwr�conych przodem do widza),
 * - mo�liwo�� zmiany, np. rozdzielczo�ci, g��bi kolor�w, pr�bkowania dla antyaliasingu, itp. bez utraty 
aktualnego stanu gry,
 * - mo�liwo�� tworzenia cieni w oparciu o stencil buffer.
 *
 * \section library_sec Wykorzystywane biblioteki.
 * 
 * OpenGLFramework wykorzystuje takie biblioteki jak SDL_ttf, FMOD, OpenAL oraz STL no i oczywi�cie OpenGL.
 * Oznacza to, �e aby skompilowa� projekt nale�y posiada� biblioteki SDL, SDL_ttf, FMOD oraz OpenAL � mo�emy
 * pobra� je z internetu (pozosta�e powinni�my ju� mie�). Oczywi�cie musimy w Visual C++ wskaza� �cie�ki do
 * folder�w include oraz lib � dla ka�dej z biblioteki z osobna. Do g��wnego katalogu projektu nale�y tak�e
 * skopiowa� pliki DLL i s� to: fmod.dll (o ile zamierzamy korzysta� z biblioteki FMOD a nie z OpenAL), alut.dll,
 * OpenAL32.dll, ogg.dll, vorbis.dll i vorbisfile.dll (o ile zamierzamy korzysta� z biblioteki OpenAL a nie FMOD),
 * SDL.dll oraz SDL_ttf.dll.
 */
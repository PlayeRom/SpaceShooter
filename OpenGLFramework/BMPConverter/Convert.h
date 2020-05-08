#pragma once

#define JEH_HEADER "JEH"

struct SHeaderJEH
{
	char cHeader[4]; //JEH_HEADER
	unsigned int uiWidth; //szerokosc obrazu
	unsigned int uiHeight; //wysokosc obrazu
	unsigned int uiSizeImage; //wielkosc danych obrazu = BITMAPINFOHEADER.biSizeImage;
	unsigned int uiBitCount;
};

struct SFileName
{
	TCHAR *pFileName;
};

class CConvert
{
public:
	CConvert(wxTextCtrl *pLog);
	~CConvert();

	bool ConvertBMPtoJEH(LPCTSTR lpFileName);
	bool ConvertJEHtoBMP(LPCTSTR lpFileName);

	bool ConvertAllBMPtoJEH(LPCTSTR lpCurDir);
	bool ConvertAllJEHtoBMP(LPCTSTR lpCurDir);

public:
	bool SaveJEHFile(LPCTSTR lpFileName, const SHeaderJEH &sHeaderJEH, const unsigned char *pImgData);
	bool SaveBMPFile(LPCTSTR lpFileName, const SHeaderJEH &sHeaderJEH, const unsigned char *pImgData);
	void SearchFile(LPCTSTR lpCurDir, LPCTSTR lpExtension);
	std::vector<SFileName> m_aFileName;
	void ClearFileNameArray();
	inline int GetFileNameArraySize() { return static_cast<int>(m_aFileName.size()); }
	wxTextCtrl		*m_pLog;
};

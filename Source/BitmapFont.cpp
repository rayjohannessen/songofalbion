////////////////////////////////////////////////////////
//   File Name	:	"BitmapFont.cpp"
//
//   Author		:	Ray Johannessen
//
//   Purpose	:	To provide a way for the user to draw text
//					using bitmap fonts
////////////////////////////////////////////////////////
#include "stdafx.h"

#include "BitmapFont.h"


CBitmapFont* CBitmapFont::GetInstance()
{
	static CBitmapFont instance;
	return &instance;
}

#define SPACING_MOD 2
void CBitmapFont::DrawString(const char* szString, int posX, int posY, float posZ, float scale, DWORD dwcolor)
{
	char ch;
	int len = (int)strlen(szString);

	for(int i = 0; i < len; i++)
	{
		ch = szString[i];
		if (!m_pCurrBMProf->HasLowerCase)
			ch = toupper(szString[i]);
		if(ch == ' ')
		{
			posX += int(m_pCurrBMProf->Size * scale);
			continue;
		}
		//convert ascii char to id off sheet
		int id = (int)(ch - m_pCurrBMProf->StartChar);
		rect rcell = CellAlgorithm(id);

		CSGD_TextureManager::GetInstance()->DrawWithZSort(m_pCurrBMProf->ID, posX, posY, posZ, scale, scale, &rcell, 0.f, 0.f, 0.f, dwcolor);
		posX += int((m_pCurrBMProf->Size + SPACING_MOD) * scale);
	}
}

point CBitmapFont::DrawStringAutoCenter (const char* szString, const rect& r, float zPos, float fScale, DWORD dwColor)
{
	char ch;
	size_t len = strlen(szString);
	int posX = r.left;
	if (r.width())
		posX -= (r.width() >> 1) - ((int)(len * (m_pCurrBMProf->Size) * fScale) >> 1);
	int posY = r.top; 
	if (r.height())
		posY -= (r.height() >> 1) - ((int)(len * (m_pCurrBMProf->Size) * fScale) >> 1);
	point start(posX, posY);

	// 	loop through string	
	for (size_t i = 0; i < len; i++)
	{
		//	do cell algorithm for each character
		ch = szString[i];
		if (!m_pCurrBMProf->HasLowerCase)
			ch = toupper(ch);

		if (ch == ' ')
		{
			posX += (int)(m_pCurrBMProf->Size * fScale);
			continue;
		}
		//	convert ascii value into an id off the sheet
		int nID = (int)(ch  - m_pCurrBMProf->StartChar);
		rect rCell = CellAlgorithm(nID);

		CSGD_TextureManager::GetInstance()->DrawWithZSort(m_pCurrBMProf->ID, posX, posY, zPos, fScale, fScale, &rCell, 0.f, 0.f, 0.f, dwColor);

		posX += (int)((m_pCurrBMProf->Size + SPACING_MOD) * fScale);
	}
	return start;
}

//////////////////////////////////////////////////////////////////////////
// PRIVATE
//////////////////////////////////////////////////////////////////////////

rect CBitmapFont::CellAlgorithm(int ID)
{
	rect rcell;
	rcell.left = (ID % m_pCurrBMProf->NumCols) * m_pCurrBMProf->Size;
	rcell.top = (ID / m_pCurrBMProf->NumCols) * m_pCurrBMProf->Size;
	rcell.right = rcell.left + m_pCurrBMProf->Size;
	rcell.bottom = rcell.top + m_pCurrBMProf->Size;

	return rcell;
}

//////////////////////////////////////////////////////////////////////////
//	Image file name format:		
//	<startChar(int)>_<hasLowerCase(bool)>_<numCols(int)>_<Name>.png
//
//	EXAMPLE: "32_0_16_Medieval.png"
//////////////////////////////////////////////////////////////////////////
void CBitmapFont::LoadProfiles()
{
	// get all the bitmap font files
	vector<string> files;
	vector<string> toIgnore; toIgnore.push_back(string(".svn"));
	GetFoldersInDirectory(string("Resources/GUI/BitmapFonts"), files, &toIgnore);

	// use the name of the file to get all the info, create the bitmap font profiles
	char startChar = 0;
	bool lowerCase = false;
	int	 id = -1;
	int  numCols = 0;
	int	 s;

	vector<string>::iterator iter, end; unsigned i = 0;
	for (iter = files.begin(), end = files.end(); iter != end && i < NUM_BFPROFILES; ++iter, ++i)
	{
		string fileName = (*iter);
		string curr = "";
		char* c = &fileName[0];
		int index = 0;
		while (true)
		{
			switch (index)
			{
			case 0:	// startChar
				{
					curr += *c;
					startChar = atoi(curr.c_str());
				}break;
			case 1:	// lowerCase
				{
					lowerCase = (bool)atoi(&(*c));
				}break;
			case 2:	// numCols
				{
					curr += *c;
					numCols = atoi(curr.c_str());
				}break;
			}
			++c;
			if (*c == '_')
			{
				++index;
				++c;
				curr = "";
			} 
			else if (*c == '.')
				break;	// at the name of the bitmapfont image, done
		}

		id = CSGD_TextureManager::GetInstance()->LoadTexture(("Resources/GUI/BitmapFonts/" + fileName).c_str(), D3DCOLOR_XRGB(0,0,0));
		s = CSGD_TextureManager::GetInstance()->GetTextureWidth(id) / numCols;

		m_arrProfiles[i] = new BFProfile(startChar, lowerCase, id, numCols, s);		
	}
	m_pCurrBMProf = m_arrProfiles[BFP_DEFAULT];
}

CBitmapFont::CBitmapFont()
{
	for (unsigned i = 0; i < NUM_BFPROFILES; ++i)
		m_arrProfiles[i] = NULL;
	LoadProfiles();
}
CBitmapFont::~CBitmapFont(void)
{
	for (unsigned i = 0; i < NUM_BFPROFILES; ++i)
		SAFE_DELETE(m_arrProfiles[i]);
}
//////////////////////////////////////////////////////////////////////////
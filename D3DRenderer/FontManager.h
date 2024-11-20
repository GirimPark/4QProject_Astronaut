#pragma once
class FontManager
{
public:
	FontManager();
	~FontManager();

	void Initailize(IDWriteFactory* pDWriteFactory);

	ComPtr<IDWriteTextFormat> GetTextFormat(
		std::wstring_view fontFamily,
		FLOAT fontSize,
		DWRITE_FONT_WEIGHT fontWeight = DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE fontStyle = DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH fontStretch = DWRITE_FONT_STRETCH_NORMAL);

private:
	IDWriteFactory* m_pDWriteFactory;
	std::unordered_map<std::wstring, ComPtr<IDWriteTextFormat>> m_textFormats;

	std::wstring GetTextFormatKey(
		std::wstring_view fontFamily,
		FLOAT fontSize,
		DWRITE_FONT_WEIGHT fontWeight,
		DWRITE_FONT_STYLE fontStyle,
		DWRITE_FONT_STRETCH fontStretch);
};
#include "pch.h"
#include "FontManager.h"

FontManager::FontManager()
	: m_pDWriteFactory(nullptr)
{
}

FontManager::~FontManager()
{
	m_textFormats.clear();
}

void FontManager::Initailize(IDWriteFactory* pDWriteFactory)
{
	m_pDWriteFactory = pDWriteFactory;
}

ComPtr<IDWriteTextFormat> FontManager::GetTextFormat(std::wstring_view fontFamily, FLOAT fontSize, DWRITE_FONT_WEIGHT fontWeight, DWRITE_FONT_STYLE fontStyle, DWRITE_FONT_STRETCH fontStretch)
{
	assert(m_pDWriteFactory);

	std::wstring textFormatKey = GetTextFormatKey(fontFamily, fontSize, fontWeight, fontStyle, fontStretch).data();

	auto it = m_textFormats.find(textFormatKey);
	if (it != m_textFormats.end()) {
		return it->second;
	}

	ComPtr<IDWriteTextFormat> textFormat;
	HR_T(m_pDWriteFactory->CreateTextFormat(
		fontFamily.data(),
		nullptr,
		fontWeight,
		fontStyle,
		fontStretch,
		fontSize,
		L"",
		textFormat.GetAddressOf()));

	m_textFormats[textFormatKey] = textFormat;
	return textFormat;
}

std::wstring FontManager::GetTextFormatKey(std::wstring_view fontFamily, FLOAT fontSize, DWRITE_FONT_WEIGHT fontWeight, DWRITE_FONT_STYLE fontStyle, DWRITE_FONT_STRETCH fontStretch)
{
	return fontFamily.data() +
		std::to_wstring(fontSize) +
		std::to_wstring(static_cast<int>(fontWeight)) +
		std::to_wstring(static_cast<int>(fontStyle)) +
		std::to_wstring(static_cast<int>(fontStretch));
}

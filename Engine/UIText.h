#pragma once
#include "UIObject.h"
class UIText :
	public UIObject
{
public:
	UIText(const std::wstring& name)
		: UIObject(name)
		, m_color(D2D1::ColorF::Black)
		, m_fontSize(10.f)
	{
	};

	void SetText(std::wstring_view text) { m_text = text; }
	void SetFont(std::wstring_view fontName) { m_fontFamily = fontName; }
	void SetFontSize(float size) { m_fontSize = size; }
	void SetColor(D2D1::ColorF color) { m_color = color; }
	void SetFontType(DWRITE_FONT_WEIGHT weight, DWRITE_FONT_STYLE style, DWRITE_FONT_STRETCH stretch) {
		m_fontWeight = weight;
		m_fontStyle = style;
		m_fontStretch = stretch;
	}

	void Update(float deltaTime);

private:
	std::wstring m_text;
	std::wstring m_fontFamily;
	FLOAT m_fontSize;
	D2D1::ColorF m_color;
	DWRITE_FONT_WEIGHT m_fontWeight = DWRITE_FONT_WEIGHT_NORMAL;
	DWRITE_FONT_STYLE m_fontStyle = DWRITE_FONT_STYLE_NORMAL;
	DWRITE_FONT_STRETCH m_fontStretch = DWRITE_FONT_STRETCH_NORMAL;
};
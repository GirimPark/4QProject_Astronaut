#include "pch.h"
#include "TextRenderer.h"
#include "FontManager.h"

TextRenderer::TextRenderer()
	: m_pd2dRenderTarget(nullptr), m_pdWriteFactory(nullptr)
{
}

TextRenderer::~TextRenderer()
{
}

void TextRenderer::DrawTextRequest(std::wstring_view text, FLOAT x, FLOAT y, D2D1::ColorF color, std::wstring_view fontFamily, FLOAT fontSize, float clientWidth, float clientHeight, DWRITE_FONT_WEIGHT fontWeight, DWRITE_FONT_STYLE fontStyle, DWRITE_FONT_STRETCH fontStretch)
{
	float halfCientWidth = clientWidth * 0.5f;
	float halfCientHeight = clientHeight * 0.5f;

	TextRenderRequest request{
		TextLayoutKey{
		text.data(),
		fontFamily.data(),
		fontSize,
		fontWeight,
		fontStyle,
		fontStretch },
		color,
		Math::Vector2(x + halfCientWidth, y + halfCientHeight) };

	m_renderQueue.push(request);
}

void TextRenderer::DrawTextInQueue()
{
	while (!m_renderQueue.empty()) {
		TextRenderRequest& request = m_renderQueue.front();
		RenderString(
			request.textLayoutKey.text,
			request.pos.x, request.pos.y,
			request.color, request.textLayoutKey.fontFamily,
			request.textLayoutKey.fontSize,
			request.textLayoutKey.fontWeight,
			request.textLayoutKey.fontStyle,
			request.textLayoutKey.fontStretch);

		m_renderQueue.pop();
	}
}

void TextRenderer::RenderString(std::wstring_view text, FLOAT x, FLOAT y, D2D1::ColorF color, std::wstring_view fontFamily, FLOAT fontSize, DWRITE_FONT_WEIGHT fontWeight, DWRITE_FONT_STYLE fontStyle, DWRITE_FONT_STRETCH fontStretch)
{
	TextLayoutKey textLayoutKey = { text.data(), fontFamily.data(), fontSize, fontWeight, fontStyle, fontStretch };

	std::wstring keyString = textLayoutKey.ToString();

	auto it = m_textLayoutMap.find(keyString);

	if (it == m_textLayoutMap.end())
	{
		TextLayoutInfo textLayoutInfo;

		textLayoutInfo.textFormat = m_fontManager->GetTextFormat(
			textLayoutKey.fontFamily,
			textLayoutKey.fontSize,
			textLayoutKey.fontWeight,
			textLayoutKey.fontStyle,
			textLayoutKey.fontStretch
		);


		HR_T(m_pdWriteFactory->CreateTextLayout(
			text.data(),
			wcslen(text.data()),
			textLayoutInfo.textFormat.Get(),
			FLT_MAX,
			FLT_MAX,
			&textLayoutInfo.textLayout
		));

		m_textLayoutMap[keyString] = std::move(textLayoutInfo);

		auto& currentTextLayoutInfo = m_textLayoutMap[keyString];
		ComPtr<ID2D1SolidColorBrush> pBrush;
		m_pd2dRenderTarget->CreateSolidColorBrush(color, &pBrush);

		DWRITE_TEXT_METRICS textMetrics;
		currentTextLayoutInfo.textLayout->GetMetrics(&textMetrics);
		float dx = textMetrics.width * 0.5f;
		float dy = textMetrics.height * 0.5f;

		m_pd2dRenderTarget->DrawTextLayout({ x - dx, y - dy }, currentTextLayoutInfo.textLayout.Get(), pBrush.Get());
	}
	else
	{
		auto& currentTextLayoutInfo = it->second;

		ComPtr<ID2D1SolidColorBrush> pBrush;
		m_pd2dRenderTarget->CreateSolidColorBrush(color, &pBrush);

		DWRITE_TEXT_METRICS textMetrics;
		currentTextLayoutInfo.textLayout->GetMetrics(&textMetrics);
		float dx = textMetrics.width * 0.5f;
		float dy = textMetrics.height * 0.5f;

		m_pd2dRenderTarget->DrawTextLayout({ x - dx, y - dy }, currentTextLayoutInfo.textLayout.Get(), pBrush.Get());
	}
}

void TextRenderer::Initailize(ID2D1RenderTarget* pd2dRenderTarget, IDWriteFactory* pDWriteFactory)
{
	m_pd2dRenderTarget = pd2dRenderTarget;
	m_pdWriteFactory = pDWriteFactory;
	m_fontManager = std::make_unique<FontManager>();
	m_fontManager->Initailize(pDWriteFactory);
}

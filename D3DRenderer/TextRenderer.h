#pragma once
class FontManager;
/// <summary>
/// TextRenderer
///	CommonApp::m_pInstance->GetRenderer()->DrawTextRequest(L"쓸거", x좌표(dx좌표계기준), y좌표(dx좌표계기준), 색(D2D1::ColorF), L"폰트이름", 폰트사이즈, 폰트두께(생략 가능, 디폴트 400), 폰트기울임(생략 가능, 디폴트 노말), 폰트늘이기(생략 가능, 디폴트 5));
/// 이렇게 쓰면 됨.
/// </summary>

class TextRenderer
{
public:
	TextRenderer();
	~TextRenderer();

	void DrawTextRequest(std::wstring_view text,
		FLOAT x,
		FLOAT y,
		D2D1::ColorF color,
		std::wstring_view fontFamily,
		FLOAT fontSize,
		float clientWidth,
		float clientHeight,
		DWRITE_FONT_WEIGHT fontWeight = DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE fontStyle = DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH fontStretch = DWRITE_FONT_STRETCH_NORMAL);

	void DrawTextInQueue();

	void Initailize(ID2D1RenderTarget* pd2dRenderTarget, IDWriteFactory* pdWriteFactory);

private:
	struct TextLayoutKey
	{
		std::wstring text;
		std::wstring fontFamily;
		FLOAT fontSize;
		DWRITE_FONT_WEIGHT fontWeight;
		DWRITE_FONT_STYLE fontStyle;
		DWRITE_FONT_STRETCH fontStretch;

		std::wstring ToString() const
		{
			return text + fontFamily + std::to_wstring(fontSize)
				+ std::to_wstring(static_cast<int>(fontWeight))
				+ std::to_wstring(static_cast<int>(fontStyle))
				+ std::to_wstring(static_cast<int>(fontStretch));
		}
	};

	struct TextLayoutInfo
	{
		Microsoft::WRL::ComPtr<IDWriteTextFormat> textFormat;
		Microsoft::WRL::ComPtr<IDWriteTextLayout> textLayout;
	};

	struct TextRenderRequest
	{
		TextLayoutKey textLayoutKey;
		D2D1::ColorF color;
		Math::Vector2 pos;

		TextRenderRequest(const TextLayoutKey& key, const D2D1::ColorF& c, const Math::Vector2& pos)
			: textLayoutKey(key), color(c), pos(pos) {}
	};

	void RenderString(std::wstring_view text,
		FLOAT x,
		FLOAT y,
		D2D1::ColorF color,
		std::wstring_view fontFamily,
		FLOAT fontSize,
		DWRITE_FONT_WEIGHT fontWeight = DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE fontStyle = DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH fontStretch = DWRITE_FONT_STRETCH_NORMAL);

	ID2D1RenderTarget* m_pd2dRenderTarget;
	IDWriteFactory* m_pdWriteFactory;
	std::unordered_map<std::wstring, TextLayoutInfo> m_textLayoutMap;
	std::queue<TextRenderRequest> m_renderQueue;
	std::unique_ptr<FontManager> m_fontManager;
};
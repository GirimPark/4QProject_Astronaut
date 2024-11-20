#pragma once

enum class eParticleType
{
	FIRE,
	SHOCK_STAR,
	SMOKE,
	SPARKLE,

	END
};

class ParticleShader
{
public:
	ParticleShader();
	~ParticleShader();

public:
	void Create(const std::wstring& shaderFileName, eParticleType type);
	void StreamOutShaderSetting(ID3D11DeviceContext* deviceContext);
	void DrawShaderSetting(ID3D11DeviceContext* deviceContext);
	void NullShaderSetting(ID3D11DeviceContext* deviceContext);

private:
	std::wstring m_Name;
	eParticleType m_Type;

	ComPtr<ID3D11InputLayout> m_InputLayout;
	ComPtr<ID3D11VertexShader> m_StreamOutVertexShader;
	ComPtr<ID3D11VertexShader> m_DrawVertexShader;
	ComPtr<ID3D11GeometryShader> m_StreamOutGeometryShader;
	ComPtr<ID3D11GeometryShader> m_DrawGeometryShader;
	ComPtr<ID3D11PixelShader> m_DrawPixelShader;
};

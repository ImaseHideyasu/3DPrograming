//--------------------------------------------------------------------------------------
// File: Sprite3D.cpp
//
// プリミティブを使用した３Ｄスプライトを描画するクラス
//
// Date: 2018.4.15
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "Sprite3D.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Sprite3D::Sprite3D(ID3D11Device* device, ID3D11DeviceContext* context, DirectX::CommonStates* states, Vector2 size, Vector2 origin)
	: m_device(device), m_context(context), m_states(states)
	, m_size(size), m_origin(origin)
{
	// エフェクトの作成
	m_batchEffect = std::make_unique<AlphaTestEffect>(device);
	m_batchEffect->SetAlphaFunction(D3D11_COMPARISON_EQUAL);
	m_batchEffect->SetReferenceAlpha(255);

	// 入力レイアウト生成
	void const* shaderByteCode;
	size_t byteCodeLength;
	m_batchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	device->CreateInputLayout(VertexPositionTexture::InputElements,
		VertexPositionTexture::InputElementCount,
		shaderByteCode, byteCodeLength,
		m_inputLayout.GetAddressOf());

	// プリミティブバッチの作成
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);
}

void Sprite3D::Draw(ID3D11ShaderResourceView * texture, Matrix world, Matrix view, Matrix projection)
{
	// 頂点情報
	VertexPositionTexture vertex[4] = 
	{
		VertexPositionTexture(Vector3(m_size.x - m_origin.x,             m_origin.y, 0.0f), Vector2(0.0f, 0.0f)),
		VertexPositionTexture(Vector3(         - m_origin.x,             m_origin.y, 0.0f), Vector2(1.0f, 0.0f)),
		VertexPositionTexture(Vector3(         - m_origin.x, -m_size.y + m_origin.y, 0.0f), Vector2(1.0f, 1.0f)),
		VertexPositionTexture(Vector3(m_size.x - m_origin.x, -m_size.y + m_origin.y, 0.0f), Vector2(0.0f, 1.0f)),
	};

	// テクスチャサンプラーの設定（クランプテクスチャアドレッシングモード）
	ID3D11SamplerState* samplers[1] = { m_states->LinearClamp() };
	m_context->PSSetSamplers(0, 1, samplers);
	// 不透明に設定
	m_context->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	// 深度バッファに書き込み参照する
	m_context->OMSetDepthStencilState(m_states->DepthDefault(), 0);
	// カリングは左周り
	m_context->RSSetState(m_states->CullCounterClockwise());

	// 不透明のみ描画する設定
	m_batchEffect->SetAlphaFunction(D3D11_COMPARISON_EQUAL);
	m_batchEffect->SetReferenceAlpha(255);

	m_batchEffect->SetWorld(world);
	m_batchEffect->SetView(view);
	m_batchEffect->SetProjection(projection);
	m_batchEffect->SetTexture(texture);
	m_batchEffect->Apply(m_context);
	m_context->IASetInputLayout(m_inputLayout.Get());

	// 不透明部分を描画
	m_batch->Begin();
	m_batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	m_batch->End();

	// 不透明以外の半透明部分を描画する設定
	m_batchEffect->SetAlphaFunction(D3D11_COMPARISON_NOT_EQUAL);
	m_batchEffect->Apply(m_context);

	// 半透明で描画
	m_context->OMSetBlendState(m_states->NonPremultiplied(), nullptr, 0xFFFFFFFF);
	// 深度バッファに書き込まないが参照だけする
	m_context->OMSetDepthStencilState(m_states->DepthRead(), 0);

	// 半透明部分を描画
	m_batch->Begin();
	m_batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	m_batch->End();
}

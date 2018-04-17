//--------------------------------------------------------------------------------------
// File: Sprite3D.h
//
// プリミティブを使用した３Ｄスプライトを描画するクラス
//
// Date: 2018.4.15
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#pragma once

class Sprite3D
{
	// DirectXのデバイス
	ID3D11Device* m_device;

	// DirectXのコンテキスト
	ID3D11DeviceContext* m_context;

	// コモンステートへのポインタ
	DirectX::CommonStates* m_states;

	// サイズ
	DirectX::SimpleMath::Vector2 m_size;

	// 原点の位置
	DirectX::SimpleMath::Vector2 m_origin;

	// エフェクト
	std::unique_ptr<DirectX::AlphaTestEffect> m_batchEffect;

	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;

	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="device">DirectXのデバイス</param>
	/// <param name="context">DirectXのコンテキスト</param>
	/// <param name="size">サイズ</param>
	/// <param name="origin">原点</param>
	Sprite3D::Sprite3D(ID3D11Device* device, ID3D11DeviceContext* context, DirectX::CommonStates* states, DirectX::SimpleMath::Vector2 size, DirectX::SimpleMath::Vector2 origin = DirectX::SimpleMath::Vector2(0.0f, 0.0f));
	
	/// <summary>
	/// 描画関数
	/// </summary>
	/// <param name="texture">テクスチャハンドル</param>
	/// <param name="world">ワールド行列</param>
	/// <param name="view">ビュー行列</param>
	/// <param name="projection">射影行列</param>
	void Draw(ID3D11ShaderResourceView* texture, DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);
};

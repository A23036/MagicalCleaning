#include "Particle.h"
#include <algorithm>

namespace {
	struct Info {
		VECTOR3 pos;
		VECTOR2 uv;
	};
};

Particle::Particle()
{
	useAddBlend = false;
	texture = nullptr;
	//vertexBuffer = nullptr;
}

Particle::~Particle()
{
//	SAFE_DELETE(vertexBuffer);
//	SAFE_DELETE(texture);
}

void Particle::LoadTexture(std::string filename, const VECTOR2& _uv)
{
	SAFE_DELETE(texture);
	SAFE_DELETE(vertexBuffer);
	GameDevice()->m_pD3D->CreateShaderResourceViewFromFile(filename.c_str(), &texture);
	uvSize = _uv;
	static const Info verts[] = {
		{ VECTOR3(-0.5f, 0.5f, 0), VECTOR2(uvSize.x, 0) },
		{ VECTOR3(-0.5f, -0.5f, 0), VECTOR2(uvSize.x, uvSize.y) },
		{ VECTOR3(0.5f, 0.5f, 0), VECTOR2(0, 0) },
		{ VECTOR3(0.5f, -0.5f, 0), VECTOR2(0, uvSize.y) },
	};

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(verts);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA init;
	init.pSysMem = verts;
	GameDevice()->m_pD3D->m_pDevice->CreateBuffer(&bd, &init, &vertexBuffer);
}

void Particle::Draw()
{
	if (texture == nullptr)
		return;

	CDirect3D* d3d = GameDevice()->m_pD3D;
	CShader* shd = GameDevice()->m_pShader;

	d3d->m_pDeviceContext->VSSetShader(shd->m_pEffect3D_VS_BILL, NULL, 0);
	d3d->m_pDeviceContext->PSSetShader(shd->m_pEffect3D_PS, NULL, 0);

	//バーテックスバッファをセット
	UINT stride = sizeof(Info);
	UINT offset = 0;
	ID3D11Buffer* pVertexBuffer = vertexBuffer;   // バーテックスバッファ
	d3d->m_pDeviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

	MATRIX4X4 world = XMMatrixScaling(transform.scale.x, transform.scale.y, transform.scale.z);
	world = world * GetLookatMatrix(transform.position, GameDevice()->m_vEyePt);

	if (useAddBlend)
	{
		UINT mask = 0xffffffff;
		d3d->m_pDeviceContext->OMSetBlendState(d3d->m_pBlendStateAdd, NULL, mask);
	}
	
	std::list<Chip> drawChips;
	// Chipをカメラとの距離の降順に並び替える
	for (auto& c : chips) {
		// 描画中心位置の移動をする
		VECTOR3 off = VECTOR3(-c.offset.x, c.offset.y, -c.offset.z) + transform.position;

		//カメラからの距離?
		off = off * XMMatrixTranspose(world * GameDevice()->m_mView * GameDevice()->m_mProj);

		c.order = off.z;
		drawChips.push_back(c);
	}

	// 距離に応じたソート
	drawChips.sort([](const Chip& a, const Chip& b) {
		return a.order > b.order; // 降順
		});

	for (auto c : drawChips) {
		//ビルボードの、視点を向くワールドトランスフォームを求める
		// 描画中心位置の移動をする
		MATRIX4X4 off = XMMatrixTranslation(-c.offset.x, c.offset.y, -c.offset.z);

		//シェーダーのコンスタントバッファーに各種データを渡す
		D3D11_MAPPED_SUBRESOURCE pData;
		CONSTANT_BUFFER_EFFECT cb;
		ZeroMemory(&cb, sizeof(cb));

		if (SUCCEEDED(d3d->m_pDeviceContext->Map(shd->m_pConstantBufferEffect, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			MATRIX4X4 scale = XMMatrixScaling(c.scale, c.scale, c.scale);
			//ワールド、カメラ、射影行列、テクスチャーオフセットを渡す
			cb.mWVP = XMMatrixTranspose(scale * off * world * GameDevice()->m_mView * GameDevice()->m_mProj);

			cb.vUVOffset.x = c.uv.x;
			cb.vUVOffset.y = c.uv.y;
			cb.vUVScale.x = 1;
			cb.vUVScale.y = 1;
			cb.fAlpha = c.alpha;

			memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
			d3d->m_pDeviceContext->Unmap(shd->m_pConstantBufferEffect, 0);
		}
		//このコンスタントバッファをどのシェーダーで使うか
		d3d->m_pDeviceContext->VSSetConstantBuffers(0, 1, &shd->m_pConstantBufferEffect);
		d3d->m_pDeviceContext->PSSetConstantBuffers(0, 1, &shd->m_pConstantBufferEffect);
		//頂点インプットレイアウトをセット
		d3d->m_pDeviceContext->IASetInputLayout(shd->m_pEffect3D_VertexLayout_BILL);
		//プリミティブ・トポロジーをセット
		d3d->m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		//テクスチャーをシェーダーに渡す
		d3d->m_pDeviceContext->PSSetSamplers(0, 1, &d3d->m_pSampleLinear);
		d3d->m_pDeviceContext->PSSetShaderResources(0, 1, &texture);
		//プリミティブをレンダリング
		d3d->m_pDeviceContext->Draw(4, 0);
	}

	if (useAddBlend)   // 加算合成色指定
	{
		UINT mask = 0xffffffff;
		d3d->m_pDeviceContext->OMSetBlendState(d3d->m_pBlendStateTrapen, NULL, mask);
	}
}

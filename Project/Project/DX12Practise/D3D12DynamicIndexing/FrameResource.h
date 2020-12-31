#pragma once

#include "DXSampleHelper.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;
/*
Bundle are small list of command that are recorded once, resue multiple time.
But the bundles are not tied to the PSO state. Meaning the PSO can update the descriptor table then 
when the bundle is run again with the different bindings the game gets a different result.
*/
class FrameResource
{
private:
	void SetCityPositions(FLOAT intervalX, FLOAT intervalZ);

public:
	struct SceneConstantBuffer
	{
		XMFLOAT4X4 mvp;        // Model-view-projection (MVP) matrix.
		FLOAT padding[48];
	};

	ComPtr<ID3D12Resource> m_cbvUploadHeap;
	ComPtr<ID3D12CommandAllocator> m_commandAllocator;
	ComPtr<ID3D12CommandAllocator> m_bundleAllocator;
	ComPtr<ID3D12GraphicsCommandList> m_bundle;
	SceneConstantBuffer* m_pConstantBuffers;
	UINT64 m_fenceValue;

	std::vector<XMFLOAT4X4> m_modelMatrices;
	UINT m_cityRowCount;
	UINT m_cityColumnCount;
	UINT m_cityMaterialCount;

	FrameResource(ID3D12Device* pDevice, UINT cityRowCount, UINT cityColumnCount);
	~FrameResource();

	void InitBundle(ID3D12Device* pDevice, ID3D12PipelineState* pPso1, ID3D12PipelineState* pPso2,
		UINT frameResourceIndex, UINT numIndices, D3D12_INDEX_BUFFER_VIEW* pIndexBufferViewDesc, D3D12_VERTEX_BUFFER_VIEW* pVertexBufferViewDesc,
		ID3D12DescriptorHeap* pCbvSrvDescriptorHeap, UINT cbvSrvDescriptorSize, ID3D12DescriptorHeap* pSamplerDescriptorHeap, ID3D12RootSignature* pRootSignature);

	void PopulateCommandList(ID3D12GraphicsCommandList* pCommandList, ID3D12PipelineState* pPso1, ID3D12PipelineState* pPso2,
		UINT frameResourceIndex, UINT numIndices, D3D12_INDEX_BUFFER_VIEW* pIndexBufferViewDesc, D3D12_VERTEX_BUFFER_VIEW* pVertexBufferViewDesc,
		ID3D12DescriptorHeap* pCbvSrvDescriptorHeap, UINT cbvSrvDescriptorSize, ID3D12DescriptorHeap* pSamplerDescriptorHeap, ID3D12RootSignature* pRootSignature);

	void XM_CALLCONV UpdateConstantBuffers(FXMMATRIX view, CXMMATRIX projection);
};
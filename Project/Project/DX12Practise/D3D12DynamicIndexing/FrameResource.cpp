#include "../stdafx.h"
#include "FrameResource.h"

FrameResource::FrameResource(ID3D12Device* pDevice, UINT cityRowCount, UINT cityColumnCount) :
    m_fenceValue(0),
    m_cityRowCount(cityRowCount),
    m_cityColumnCount(cityColumnCount)
{
    m_modelMatrices.resize(m_cityRowCount * m_cityColumnCount);

    /*
    The command allocator is use by the main class when 
    resetting the command list in the main loop. Each frame
    resources need the command allocator because command allocator 
    cannot be reuse until the GPU excute all the command.
    */
    ThrowIfFailed(pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator)));
    ThrowIfFailed(pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_BUNDLE, IID_PPV_ARGS(&m_bundleAllocator)));

    // Create an upload heap for the constant buffers.
    ThrowIfFailed(pDevice->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(sizeof(SceneConstantBuffer) * m_cityRowCount * m_cityColumnCount),
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&m_cbvUploadHeap)));

    /*
    Map the constant buffer.
    */
    CD3DX12_RANGE readRange(0, 0);        // We do not intend to read from this resource on the CPU.
    ThrowIfFailed(m_cbvUploadHeap->Map(0, &readRange, reinterpret_cast<void**>(&m_pConstantBuffers)));

    // Update all of the model matrices once; our cities don't move so 
    // we don't need to do this ever again.
    SetCityPositions(8.0f, -8.0f);
}

FrameResource::~FrameResource()
{
    m_cbvUploadHeap->Unmap(0, nullptr);
    m_pConstantBuffers = nullptr;
}

void FrameResource::InitBundle(ID3D12Device* pDevice, ID3D12PipelineState* pPso1, ID3D12PipelineState* pPso2,
    UINT frameResourceIndex, UINT numIndices, D3D12_INDEX_BUFFER_VIEW* pIndexBufferViewDesc, D3D12_VERTEX_BUFFER_VIEW* pVertexBufferViewDesc,
    ID3D12DescriptorHeap* pCbvSrvDescriptorHeap, UINT cbvSrvDescriptorSize, ID3D12DescriptorHeap* pSamplerDescriptorHeap, ID3D12RootSignature* pRootSignature)
{
    ThrowIfFailed(pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_BUNDLE, m_bundleAllocator.Get(), pPso1, IID_PPV_ARGS(&m_bundle)));
    NAME_D3D12_OBJECT(m_bundle);

    PopulateCommandList(m_bundle.Get(), pPso1, pPso2, frameResourceIndex, numIndices, pIndexBufferViewDesc,
        pVertexBufferViewDesc, pCbvSrvDescriptorHeap, cbvSrvDescriptorSize, pSamplerDescriptorHeap, pRootSignature);
    //Close the command list
    ThrowIfFailed(m_bundle->Close());
}

void FrameResource::SetCityPositions(FLOAT intervalX, FLOAT intervalZ)
{
    for (UINT i = 0; i < m_cityRowCount; i++)
    {
        FLOAT cityOffsetZ = i * intervalZ;
        for (UINT j = 0; j < m_cityColumnCount; j++)
        {
            FLOAT cityOffsetX = j * intervalX;

            // The y position is based off of the city's row and column 
            // position to prevent z-fighting.
            XMStoreFloat4x4(&m_modelMatrices[i * m_cityColumnCount + j], XMMatrixTranslation(cityOffsetX, 0.02f * (i * m_cityColumnCount + j), cityOffsetZ));
        }
    }
}

void FrameResource::PopulateCommandList(ID3D12GraphicsCommandList* pCommandList, ID3D12PipelineState* pPso1, ID3D12PipelineState* pPso2,
    UINT frameResourceIndex, UINT numIndices, D3D12_INDEX_BUFFER_VIEW* pIndexBufferViewDesc, D3D12_VERTEX_BUFFER_VIEW* pVertexBufferViewDesc,
    ID3D12DescriptorHeap* pCbvSrvDescriptorHeap, UINT cbvSrvDescriptorSize, ID3D12DescriptorHeap* pSamplerDescriptorHeap, ID3D12RootSignature* pRootSignature)
{
    /*
    If the root signature matches the root signature of caller, then
    binding are inherited, otherwise the bind space reset.
    */
    pCommandList->SetGraphicsRootSignature(pRootSignature);

    ID3D12DescriptorHeap* ppHeaps[] = { pCbvSrvDescriptorHeap, pSamplerDescriptorHeap };
    pCommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
    pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    pCommandList->IASetIndexBuffer(pIndexBufferViewDesc);
    pCommandList->IASetVertexBuffers(0, 1, pVertexBufferViewDesc);
    pCommandList->SetGraphicsRootDescriptorTable(0, pCbvSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
    pCommandList->SetGraphicsRootDescriptorTable(1, pSamplerDescriptorHeap->GetGPUDescriptorHandleForHeapStart());

    //Calculate the dexcriptor offset due to multiple frame resources.
    // 1 SRV + how many CBVs we have currently
    UINT frameResourceDescriptorOffset = 1 + (frameResourceIndex * m_cityRowCount * m_cityColumnCount);
    CD3DX12_GPU_DESCRIPTOR_HANDLE cbvSrvHandle(pCbvSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart(), frameResourceDescriptorOffset, cbvSrvDescriptorSize);

    PIXBeginEvent(pCommandList, 0, L"Draw cities");
    BOOL usePso1 = TRUE;
    for (UINT i = 0; i < m_cityRowCount; i++)
    {
        for (UINT j = 0; j < m_cityColumnCount; j++)
        {
            // Alternate which PSO to use; the pixel shader is different on 
            // each just as a PSO setting demonstration.
            pCommandList->SetPipelineState(usePso1 ? pPso1 : pPso2);
            usePso1 = !usePso1;

            // Set this city's CBV table and move to the next descriptor.
            pCommandList->SetGraphicsRootDescriptorTable(2, cbvSrvHandle);
            cbvSrvHandle.Offset(cbvSrvDescriptorSize);

            pCommandList->DrawIndexedInstanced(numIndices, 1, 0, 0, 0);
        }
    }
    PIXEndEvent(pCommandList);

}

void XM_CALLCONV FrameResource::UpdateConstantBuffers(FXMMATRIX view, CXMMATRIX projection)
{
    XMMATRIX model;
    XMFLOAT4X4 mvp;

    for (UINT i = 0; i < m_cityRowCount; i++)
    {
        for (UINT j = 0; j < m_cityColumnCount; j++)
        {
            model = XMLoadFloat4x4(&m_modelMatrices[i * m_cityColumnCount + j]);

            // Compute the model-view-projection matrix.
            XMStoreFloat4x4(&mvp, XMMatrixTranspose(model * view * projection));

            // Copy this matrix into the appropriate location in the upload heap subresource.
            memcpy(&m_pConstantBuffers[i * m_cityColumnCount + j], &mvp, sizeof(mvp));
        }
    }
}
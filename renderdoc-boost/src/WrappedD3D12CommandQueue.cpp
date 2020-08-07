#include "WrappedD3D12CommandQueue.h"
#include "WrappedD3D12DXGISwapChain.h"
#include "WrappedD3D12Resource.h"
#include "WrappedD3D12Heap.h"
#include "WrappedD3D12CommandList.h"
#include "WrappedD3D12Fence.h"
RDCBOOST_NAMESPACE_BEGIN

WrappedD3D12CommandQueue::WrappedD3D12CommandQueue(ID3D12CommandQueue* pRealD3D12CommandQueue, WrappedD3D12Device* pDevice) :
	WrappedD3D12DeviceChild(pRealD3D12CommandQueue, pDevice),
	m_pWrappedSwapChain(NULL)
{

}

WrappedD3D12CommandQueue::~WrappedD3D12CommandQueue()
{
}

HRESULT  WrappedD3D12CommandQueue::createSwapChain(
	IDXGIFactory2* pDXGIFactory, HWND hWnd, 
	const DXGI_SWAP_CHAIN_DESC1 *pDesc, const DXGI_SWAP_CHAIN_FULLSCREEN_DESC *pFullscreenDesc, 
	IDXGIOutput *pRestrictToOutput, IDXGISwapChain1 **ppSwapChain)
{
	COMPtr<IDXGISwapChain1> swapChain1;
	HRESULT ret = pDXGIFactory->CreateSwapChainForHwnd(
		GetReal().Get(), hWnd,
		pDesc, pFullscreenDesc,
		pRestrictToOutput, &swapChain1);
	if (FAILED(ret)) {
		LogError("Initialzation: create swapchain1 failed");
	}
	Assert(swapChain1.Get());
	auto pSwapChain = new WrappedD3D12DXGISwapChain(swapChain1.Get(), this);//ref=1
	m_pWrappedSwapChain = pSwapChain;//ref=2
	if (pSwapChain&& ppSwapChain) {
		*ppSwapChain = static_cast<IDXGISwapChain1*>(m_pWrappedSwapChain.Get());
		pSwapChain->AddRef();//here ref should be 3
	}
	else
		m_pWrappedSwapChain.Reset();//here ref should be 1
	pSwapChain->Release();// 1. ref=2, else ref=0
	pSwapChain = 0;//safe release
	return ret;
}

COMPtr<ID3D12DeviceChild> WrappedD3D12CommandQueue::CopyToDevice(ID3D12Device* pNewDevice) {
	COMPtr<ID3D12CommandQueue> pNewCommandQueue = NULL;
	D3D12_COMMAND_QUEUE_DESC newDesc = GetDesc();
	HRESULT ret = pNewDevice->CreateCommandQueue(&newDesc, IID_PPV_ARGS(&pNewCommandQueue));
	if (FAILED(ret)) {
		LogError("create new commandqueue failed");
		return NULL;
	}
	// switch swapchain
	Assert(pNewCommandQueue.Get());
	if(m_pWrappedSwapChain.Get())
		m_pWrappedSwapChain->SwitchToCommandQueue(pNewCommandQueue.Get());
	return pNewCommandQueue;
}

bool WrappedD3D12CommandQueue::isResourceExist(WrappedD3D12Resource* pWrappedResource, ID3D12Resource* pRealResource) {
	if (m_pWrappedSwapChain.Get()) {
		return m_pWrappedSwapChain->isResourceExist(pWrappedResource, pRealResource);
	}
	return false;
}

void WrappedD3D12CommandQueue::cacheResourceReflectionToOldReal() {
	if (m_pWrappedSwapChain.Get())
		m_pWrappedSwapChain->cacheResourceReflectionToOldReal();
};
void WrappedD3D12CommandQueue::clearResourceReflectionToOldReal() {
	if (m_pWrappedSwapChain.Get())
		m_pWrappedSwapChain->clearResourceReflectionToOldReal();
}

/************************************************************************/
/*                         override                                                                     */
/************************************************************************/
void STDMETHODCALLTYPE WrappedD3D12CommandQueue::UpdateTileMappings(
	_In_  ID3D12Resource *pResource,
	UINT NumResourceRegions,
	_In_reads_opt_(NumResourceRegions)  const D3D12_TILED_RESOURCE_COORDINATE *pResourceRegionStartCoordinates,
	_In_reads_opt_(NumResourceRegions)  const D3D12_TILE_REGION_SIZE *pResourceRegionSizes,
	_In_opt_  ID3D12Heap *pHeap,
	UINT NumRanges,
	_In_reads_opt_(NumRanges)  const D3D12_TILE_RANGE_FLAGS *pRangeFlags,
	_In_reads_opt_(NumRanges)  const UINT *pHeapRangeStartOffsets,
	_In_reads_opt_(NumRanges)  const UINT *pRangeTileCounts,
	D3D12_TILE_MAPPING_FLAGS Flags) {
	GetReal()->UpdateTileMappings(
		static_cast<WrappedD3D12Resource*>(pResource)->GetReal().Get() 
		, NumResourceRegions, pResourceRegionStartCoordinates,pResourceRegionSizes, 
		static_cast<WrappedD3D12Heap*>(pHeap)->GetReal().Get(),
		NumRanges, pRangeFlags, pHeapRangeStartOffsets, pRangeTileCounts, Flags);
}

void STDMETHODCALLTYPE WrappedD3D12CommandQueue::CopyTileMappings(
	_In_  ID3D12Resource *pDstResource,
	_In_  const D3D12_TILED_RESOURCE_COORDINATE *pDstRegionStartCoordinate,
	_In_  ID3D12Resource *pSrcResource,
	_In_  const D3D12_TILED_RESOURCE_COORDINATE *pSrcRegionStartCoordinate,
	_In_  const D3D12_TILE_REGION_SIZE *pRegionSize,
	D3D12_TILE_MAPPING_FLAGS Flags) {
	GetReal()->CopyTileMappings(
		static_cast<WrappedD3D12Resource*>(pDstResource)->GetReal().Get(), pDstRegionStartCoordinate,
		static_cast<WrappedD3D12Resource *>(pSrcResource)->GetReal().Get(), pSrcRegionStartCoordinate,
		pRegionSize, Flags);
}

void STDMETHODCALLTYPE WrappedD3D12CommandQueue::ExecuteCommandLists(
	_In_  UINT NumCommandLists,
	_In_reads_(NumCommandLists)  ID3D12CommandList *const *ppCommandLists) {
	std::vector<ID3D12CommandList*> listArray(NumCommandLists);
	for (UINT i=0; i<NumCommandLists; i++)
	{
		listArray[i] = static_cast<WrappedD3D12CommandList*>(ppCommandLists[i])->GetReal().Get();
		static_cast<WrappedD3D12CommandList*>(ppCommandLists[i])->FlushPendingResourceStates();
	}
	GetReal()->ExecuteCommandLists(NumCommandLists, listArray.data());
}

void STDMETHODCALLTYPE WrappedD3D12CommandQueue::SetMarker(
	UINT Metadata,
	_In_reads_bytes_opt_(Size)  const void *pData,
	UINT Size) {
	GetReal()->SetMarker(Metadata, pData, Size);
}

void STDMETHODCALLTYPE WrappedD3D12CommandQueue::BeginEvent(
	UINT Metadata,
	_In_reads_bytes_opt_(Size)  const void *pData,
	UINT Size) {
	GetReal()->BeginEvent(Metadata, pData, Size);
}

void STDMETHODCALLTYPE WrappedD3D12CommandQueue::EndEvent(void) {
	GetReal()->EndEvent();
}

HRESULT STDMETHODCALLTYPE WrappedD3D12CommandQueue::Signal(
	ID3D12Fence *pFence,
	UINT64 Value) {
	static_cast<WrappedD3D12Fence *>(pFence)->UpdateApplicationFenceValue(Value);
	return GetReal()->Signal(
		static_cast<WrappedD3D12Fence *>(pFence)->GetReal().Get(), 
		Value);
}

HRESULT STDMETHODCALLTYPE WrappedD3D12CommandQueue::Wait(
	ID3D12Fence *pFence,
	UINT64 Value) {
	return GetReal()->Wait(
		static_cast<WrappedD3D12Fence *>(pFence)->GetReal().Get(), 
		Value);
}

HRESULT STDMETHODCALLTYPE WrappedD3D12CommandQueue::GetTimestampFrequency(
	_Out_  UINT64 *pFrequency) {
	return GetReal()->GetTimestampFrequency(
		pFrequency);
}

HRESULT STDMETHODCALLTYPE WrappedD3D12CommandQueue::GetClockCalibration(
	_Out_  UINT64 *pGpuTimestamp,
	_Out_  UINT64 *pCpuTimestamp) {
	return GetReal()->GetClockCalibration(
		pGpuTimestamp, 
		pCpuTimestamp);
}

D3D12_COMMAND_QUEUE_DESC STDMETHODCALLTYPE WrappedD3D12CommandQueue::GetDesc(void) {
	return GetReal()->GetDesc();
}

RDCBOOST_NAMESPACE_END
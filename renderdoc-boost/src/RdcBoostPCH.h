#pragma once
#include <d3d11.h>
#include <d3d12.h>
#include <d3dx12.h>

#define RDCBOOST_NAMESPACE_BEGIN namespace rdcboost\
{

#define RDCBOOST_NAMESPACE_END }

#include <wrl.h>
#define COMPtr Microsoft::WRL::ComPtr

template <typename T>
T RDCMIN(const T &a, const T &b)
{
	return a < b ? a : b;
}
template <typename T>
T RDCMAX(const T &a, const T &b)
{
	return a > b ? a : b;
}
template <typename T>
inline T AlignUpWithMask(T value, size_t mask)
{
	return (T)(((size_t)value + mask) & ~mask);
}

template <typename T>
inline T AlignDownWithMask(T value, size_t mask)
{
	return (T)((size_t)value & ~mask);
}

template <typename T>
inline T AlignUp(T value, size_t alignment)
{
	return AlignUpWithMask(value, alignment - 1);
}

template <typename T>
inline T AlignDown(T value, size_t alignment)
{
	return AlignDownWithMask(value, alignment - 1);
}

template <typename T>
inline bool IsAligned(T value, size_t alignment)
{
	return 0 == ((size_t)value & (alignment - 1));
}

#define ANALYZE_WRAPPED_CPU_HANDLE(wrapped) \
static_cast<WrappedD3D12DescriptorHeap::DescriptorHeapSlot*>((void*)((wrapped).ptr))->getRealCPUHandle()

#define ANALYZE_WRAPPED_GPU_HANDLE(wrapped) \
static_cast<WrappedD3D12DescriptorHeap::DescriptorHeapSlot*>((void*)((wrapped).ptr))->getRealGPUHandle()

#define ANALYZE_WRAPPED_SLOT(pName, wrapped) auto pName = \
static_cast<WrappedD3D12DescriptorHeap::DescriptorHeapSlot*>((void*)((wrapped).ptr))

#define DEFINE_AND_ASSERT_WRAPPED_GPU_VADDR(VADDR) \
auto pWrappedResource_Ano = WrappedD3D12GPUVAddrMgr::Get().GetWrappedResourceByAddr(VADDR);\
Assert(pWrappedResource_Ano);\
auto realVAddr =  pWrappedResource_Ano->GetReal()->GetGPUVirtualAddress() + (VADDR - pWrappedResource_Ano->GetOffset())

// update fist��Ľṹ
// cons des
// override
// func
// framework

//update 7/25 switchToDevice D3D12������

//update 7/27 ��� device��swapchain1��override

//update 7/29 ���descriptor heap�Ļ���Ϳ����� �����С��������wrapper																										*****Perf Impact xxx fixed *****

//update 7/30 ʵ�ֵ�֡�߼���һ֡���presentȻ��waitForFirstBackBuffer value�����D3D12CallAtEndOfFrame��Ȼ��wrappedDevice��ȥ��switch����������Ҫ�����ǵȴ�����fence��value����

//update 7/30 ����ʵ��d3d12 object�Ŀ������

// update 7/31 ��comptr�����
// comptr��ָ��init�Ļ���Ѷ���ref++
// comptr��operator=�����--ԭ���ģ����µ�++
// ������d3d��create�������������ptr����comptr�����ڴ��ptr����������һ��ref=1�Ķ������ptr��, ��Ϊ�������ppv����ֵ�����ref�࣫1
// ���������ȥnewһ��Wrapper�Ļ�Ҫ�ǵ�new��������ref=1������

//update7/31 ���������ڵ����
//�Լ��ı���m_pReal��Ҫref��Wrapper����ӵ��һ��real����������                                         ------>   ComPtr
//��¼��other_pReal���Բ���ref�������Ǵ�һ��ָ��������switch��ʱ��Ƚ�����                         ------>   RawPtr
//��¼��other_pWrapper��Ҫref, �ڱ����ͷŵ�ʱ�����release                                                    ------>   ComPtr
//��device��new��Wrapper�Ķ��󣬴������Զ���������Ϊ1�� ��ֵ��*ppV                                  ------>   RawPtr       
//��backRefs�м�¼ָ�룬�������������ڣ���Wrapper����delete��ʱ���Զ���map��erase��     ------>   RawPtr  
//copyToDevice�д����Ķ��󣬸ĳ�ComPtrֱ����operator=                                                       ------>   ComPtr
//�ͷ�wrapped��Դ��ʱ����Ҫ��device���ҵ�����ɾ��																																			 *****Perf Impact *****

//update7/31 ֧��api�������Ϊnull���������Ҫ֧����CreateView�ǲ���

//update8/01 ��һ������ͬʱ֧��SwapChain1-4�� ʹ��queryInterface�ķ���������һ������ͨ�õ����

//update8/02 ʹ��Dummy Info Queue��Hook��Debug Layer�����أ����Ա�֤renderdoc�д�����renderdoc wrapped device���������ѭ��

//update8/02 ����pipeline state��ʱ��Ҫ����byte code���ͷ�Ҫ��ȷ��delete������CS��VS��PS��SO��																*****Perf Impact *****

//update8/03 ������Դ�������߼���1. ����������ԴD3D12_HEAP_TYPE_UPLOAD/D3D12_HEAP_TYPE_READBACK/..											*****Perf Impact ***** //TODO ����upload��Դ
//          2.��Ϊrenderdoc wrapped new res��raw res������ͬһ��queue/list/allocator�ܣ�Ҫд��cpu��cpu��������upload-copy
//          3. readback/upload heapֻ����buffer������ʹ��d3dx12�е�UpdateSubresource��һ�׻��ƽ��
//          4. desc heap���½�view�����е����⣬����catch exception������Ϊ�ͷŵ���Դ�½�view

//update8/04 descriptor heap�е����������cpu visible�Ļ�������ȷ��cpu handle�ģ������gpu visible�Ļ���û�е�
// ���ֻ��Ҫ����cpu descriptor heap�е�params���ؽ�ҲֻҪ�ָ�cpu descriptor�е����ݾͺ���
// ��Ϊ����ȥ����handle�������handle��Ҫ��wrappedDescriptorHeap��hookһ���µ��ڴ��ַ������Ȼ���û�����ʱ���ٽ�������ʵ��d3d api����descriptorheap���ڴ��ַ
// Ȼ����ô���ǲ��еģ���Ϊcpu handle���ڿ����ǻ����ص��ģ�����cpu��gpu�����ܻ��gpu�Ľ�����cpu��������cache���»ָ���ʱ�򱨴�
// �ο�renderdoc�������ǰ�descriptor wrap��������ȥ��handle.ptr��hook��descriptor�ĵ�ַ

//update8/05
//Descriptor Heap&Handle��wrap�����µ�bugfix���Ż���cache��isResourceExist�Ŀ��
//��Ҫwrapped��Դ�ͷź�slot�д����Ȼ����ָ�룬û���������Ƿ��ͷš�
//�����������ڴ��½�һ��wrapped��Դ�ͻᵼ����Դָ�����Ҳ���ǻ���view desc����������Դ��view��desc�Բ��Ͼͻ���d3d device remove��
//������Դ���ؽ���ʱ��ᵼ��wrapper�ڵ�real��������������ÿ��slot�ж�����real��Ȼ������Դ�ؽ�ǰ��device��swap chain�м��¾����������ڵ�wrapped->old raw res��ָ�롣��descHeap�е�view�ؽ���ʱ�����Ƿ���ڡ��������޸ĵ���isResourceExist�Ļ���

// update  TODO
// 0. descriptor heap�е�resource�����Ѿ����ͷ��� -�� catch(...)
// 1. ֧��CreateView�еĲ������
// 2. ֧������API�Ĳ������
// 3. ID3D12Device1-6
// 4. ID3D12GraphicsCommandList1-5
// 5. ֧��ID3D12CommandSignature / ID3D12QueryHeap / ID3D12StateObject 

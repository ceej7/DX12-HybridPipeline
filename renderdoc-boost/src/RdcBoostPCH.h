#pragma once
#include <d3d11.h>
#include <d3d12.h>
#include <d3dx12.h>

#define RDCBOOST_NAMESPACE_BEGIN namespace rdcboost\
{

#define RDCBOOST_NAMESPACE_END }

#include <wrl.h>
#define COMPtr Microsoft::WRL::ComPtr

// update fist��Ľṹ
// cons des
// override
// func
// framework

//update 7/25 switchToDevice D3D12������

//update 7/27 ��� device��swapchain1��override

//update 7/29 ���descriptor heap�Ļ���Ϳ����� �����С��������wrapper

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

//update7/31 ֧��api�������Ϊnull���������Ҫ֧����CreateView�ǲ���

// update  TODO
// 0. descriptor heap�е�resource�����Ѿ����ͷ��� -�� catch(...)
// 1. ֧��CreateView�еĲ������
// 2. ֧������API�Ĳ������
// 3. ID3D12Device1-6
// 4. ID3D12GraphicsCommandList1-5
// 5. ֧��ID3D12CommandSignature / ID3D12QueryHeap / ID3D12StateObject 

#pragma once
#include <d3d11.h>
#include <d3d12.h>
#define RDCBOOST_NAMESPACE_BEGIN namespace rdcboost\
{

#define RDCBOOST_NAMESPACE_END }

#include <wrl.h>
#define COMPtr Microsoft::WRL::ComPtr

//cons des
// override
// func
// framework

//
////�Լ��ı���Ҫref
////���õ�ID3D12Object��Ҫref
////���õ�WrappedD3D12ObjectҪref
////�Լ������Ķ���ֻ����Wrapped������Ϊʹ�õ���new�������Զ���ref(1)

//��comptr�����
// comptr��ָ��init�Ļ���Ѷ���ref++
// comptr��operator=�����--ԭ���ģ����µ�++
// ������d3d��create�������������ptr����comptr�����ڴ��ptr����������һ��ref=1�Ķ������ptr��
// ���������ȥnewһ��Wrapper�Ļ�Ҫ�ǵð�ԭ����ptr ref--

//update ���������ڵ����
//�Լ��ı���m_pReal��Ҫref������ֻҪref���һ��, ֻ����new������Wrapper�����ߣ�Wrapper���󼴴���һ��real�����Ǹĳ�ComPtr
//��¼��other_pReal���Բ���ref�������Ǵ�һ��ָ��������switch��ʱ��Ƚ����ݣ����Ҫ����ʹ���������һ���������
//��¼��other_pWrapper��Ҫref, �����������ComPtr��ref����ΪComPtr����ָ���ʼ����ʱ����Զ�ref++�����Կ���WrappedD3D12Object�г�ʼ��Ϊref0
//��device�д�����Wrapper�Ķ����¼ָ�룬����¼�������ڣ�������delete��ʱ���Զ���map��erase��
//copyToDevice�д����Ķ�����Ҫ�����ԭ����m_pReal, ���û�иĳ�ComPtr����Ҫ�Զ��ͷ�ԭ���ģ�ref�µģ��ĳ�ComPtr�Ļ�ֱ����operator=
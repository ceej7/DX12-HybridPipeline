#pragma once
#include <d3d11.h>
#include <d3d12.h>
#define RDCBOOST_NAMESPACE_BEGIN namespace rdcboost\
{

#define RDCBOOST_NAMESPACE_END }

#include <wrl.h>
#define COMPtr Microsoft::WRL::ComPtr

//��Ľṹ
//cons des
// override
// func
// framework

//��comptr�����
// comptr��ָ��init�Ļ���Ѷ���ref++
// comptr��operator=�����--ԭ���ģ����µ�++
// ������d3d��create�������������ptr����comptr�����ڴ��ptr����������һ��ref=1�Ķ������ptr��, ��Ϊ�������ppv����ֵ�����ref�࣫1
// ���������ȥnewһ��Wrapper�Ļ�Ҫ�ǵð�ԭ����ptr ref--

//update ���������ڵ����
//�Լ��ı���m_pReal��Ҫref������ֻҪref���һ��, ֻ����new������Wrapper�����ߣ�Wrapper���󼴴���һ��real��ʹ��ComPtr
//��¼��other_pReal���Բ���ref�������Ǵ�һ��ָ��������switch��ʱ��Ƚ����ݣ����Ҫ����ʹ���������һ���������
//��¼��other_pWrapper��Ҫref, ��ComPtr��ref���ڱ����ͷŵ�ʱ�����release
//��device�д�����Wrapper�Ķ�����backRefs�м�¼ָ�룬����¼�������ڣ�������delete��ʱ���Զ���map��erase��
//copyToDevice�д����Ķ��󣬸ĳ�ComPtrֱ����operator=
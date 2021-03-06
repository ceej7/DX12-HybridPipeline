#include "DX12LibPCH.h"

#include <Resource.h>

#include <Application.h>
#include <ResourceStateTracker.h>

Resource::Resource(const std::wstring& name)
    : m_ResourceName(name)
{}

Resource::Resource(const D3D12_RESOURCE_DESC& resourceDesc, const D3D12_CLEAR_VALUE* clearValue, const std::wstring& name)
{
    auto device = Application::Get().GetDevice();

    if ( clearValue )
    {
        m_d3d12ClearValue = std::make_unique<D3D12_CLEAR_VALUE>(*clearValue);
    }
    
    ThrowIfFailed( device->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_COMMON,
        m_d3d12ClearValue.get(),
        IID_PPV_ARGS(&m_d3d12Resource)
    ) );

    ResourceStateTracker::AddGlobalResourceState(m_d3d12Resource.Get(), D3D12_RESOURCE_STATE_COMMON );

    SetName(name);
}

Resource::Resource(Microsoft::WRL::ComPtr<ID3D12Resource> resource, const std::wstring& name)
    : m_d3d12Resource(resource)
{
    SetName(name);
}

Resource::Resource(const Resource& copy)
    : m_d3d12Resource(copy.m_d3d12Resource)
    , m_ResourceName(copy.m_ResourceName)
{
	if (copy.m_d3d12ClearValue.get()) {
		m_d3d12ClearValue = std::make_unique<D3D12_CLEAR_VALUE>(*copy.m_d3d12ClearValue);
	}
}

Resource::Resource(Resource&& copy)
    : m_d3d12Resource(std::move(copy.m_d3d12Resource))
    , m_ResourceName(std::move(copy.m_ResourceName))
    , m_d3d12ClearValue(std::move(copy.m_d3d12ClearValue))
{
}

Resource& Resource::operator=(const Resource& other)
{
    if ( this != &other )
    {
        m_d3d12Resource = other.m_d3d12Resource;
        m_ResourceName = other.m_ResourceName;
        if ( other.m_d3d12ClearValue )
        {
            m_d3d12ClearValue = std::make_unique<D3D12_CLEAR_VALUE>( *other.m_d3d12ClearValue );
        }
    }

    return *this;
}

Resource& Resource::operator=(Resource&& other)
{
    if (this != &other)
    {
        m_d3d12Resource = other.m_d3d12Resource;
        m_ResourceName = other.m_ResourceName;
        m_d3d12ClearValue = std::move( other.m_d3d12ClearValue );

        other.m_d3d12Resource.Reset();
        other.m_ResourceName.clear();
    }

    return *this;
}


Resource::~Resource()
{
}

void Resource::SetD3D12Resource(Microsoft::WRL::ComPtr<ID3D12Resource> d3d12Resource, const D3D12_CLEAR_VALUE* clearValue )
{
    m_d3d12Resource = d3d12Resource;
    if ( m_d3d12ClearValue )
    {
        m_d3d12ClearValue = std::make_unique<D3D12_CLEAR_VALUE>( *clearValue );
    }
    else
    {
        m_d3d12ClearValue.reset();
    }

	//Remind: 这里不必添加Global State Tracker
	//// 我们认为全局所有的D3DResoure必须通过构造函数生成Default Heap
	//// 否则由用户自行添加

    SetName(m_ResourceName);
}

void Resource::SetName(const std::wstring& name)
{
    m_ResourceName = name;
    if (m_d3d12Resource && !m_ResourceName.empty())
    {
        m_d3d12Resource->SetName(m_ResourceName.c_str());
    }
}

void Resource::Reset()
{
    m_d3d12Resource.Reset();
    m_d3d12ClearValue.reset();
}

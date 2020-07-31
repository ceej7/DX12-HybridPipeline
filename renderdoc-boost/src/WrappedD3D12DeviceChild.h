#pragma once
#include <string>
#include "WrappedD3D12Device.h"

RDCBOOST_NAMESPACE_BEGIN

template<typename NestedType>
class WrappedD3D12DeviceChild : public WrappedD3D12Object<NestedType> {
public:
	WrappedD3D12DeviceChild(NestedType* pReal, WrappedD3D12Device* pDevice)
		:WrappedD3D12Object(pReal), m_pWrappedDevice(pDevice)
	{
		m_pRealDevice = m_pWrappedDevice->GetReal().Get();
	};
	virtual ~WrappedD3D12DeviceChild()
	{
		m_pWrappedDevice->OnDeviceChildReleased(static_cast<ID3D12DeviceChild*>(m_pReal.Get()));
	}

public: //override for ID3D12DeviceChild
	virtual HRESULT STDMETHODCALLTYPE GetDevice(REFIID riid, void **ppvDevice) {
		if (riid == __uuidof(ID3D12Device)) {
			*ppvDevice = static_cast<ID3D12Device*>(m_pWrappedDevice.Get());
			m_pWrappedDevice->AddRef();
			return S_OK;
		}
		return S_FALSE;
	}

public: //function
	COMPtr<WrappedD3D12Device> GetWrappedDevice() { return m_pWrappedDevice; }

public: //framework
	virtual void SwitchToDevice(ID3D12Device* pNewDevice)
	{
		if (m_pRealDevice == pNewDevice)
			return;

		COMPtr<ID3D12DeviceChild> pCopied = CopyToDevice(pNewDevice);
		Assert(pCopied.Get() != NULL);
		m_PrivateData.CopyPrivateData(pCopied);
		m_pReal = pCopied;
		pCopied->SetName(m_ObjectName);
		m_pRealDevice = pNewDevice;//���ݿ��, ����WrappedDeviceû�б䣬ֻ���ڲ���pReal����

	}

	virtual COMPtr<ID3D12DeviceChild> CopyToDevice(ID3D12Device* pNewDevice) = 0;

protected:
	COMPtr<WrappedD3D12Device> const m_pWrappedDevice;
	ID3D12Device* m_pRealDevice;
};

RDCBOOST_NAMESPACE_END
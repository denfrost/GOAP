#	pragma once

#	include "IntrusivePtrBase.h"

namespace GOAP
{
	class SwitchProvider
		: public IntrusivePtrBase<SwitchProvider>
	{
	public:
		virtual uint32_t onSwitch() = 0;
	};

	typedef IntrusivePtr<SwitchProvider> SwitchProviderPtr;

	template<class F>
	class SwitchProviderT
		: public SwitchProvider
	{
	public:
		SwitchProviderT( F _f )
			: m_f( _f )
		{
		}

	public:
		uint32_t onSwitch() override
		{
			uint32_t result = m_f();

			return result;
		}

	protected:
		F m_f;
	};

	template<class F>
	SwitchProviderPtr makeSwitchProvider( F _f )
	{
		SwitchProviderPtr provider = new SwitchProviderT<F>( _f );

		return provider;
	}
}
#pragma once

#include "../IMul.h"
#include <UTemplate/SI.h>

namespace Ubpa {
	template<typename Base, typename Impl, typename ArgList>
	struct IArrayHadamardProduct : Base {
		using IList = TemplateList<IMul, IArray>;
		using Base::Base;

		using F = Arg_F<ArgList>;
		using T = Arg_T<ArgList>;
		static constexpr size_t N = Arg_N<ArgList>;

		using Base::operator*=;
		using Base::operator/;
		using Base::operator/=;

		Impl& operator*=(const Impl& y) noexcept {
			auto& x = static_cast<Impl&>(*this);
#ifdef UBPA_USE_XSIMD
			if constexpr (std::is_same_v<T, float> && N == 4)
				x.get() *= y.get();
			else
#endif // UBPA_USE_XSIMD
			for (size_t i = 0; i < N; i++)
				x[i] *= y[i];
			return x;
		}

		const Impl operator/(const Impl& y) const noexcept {
			auto& x = static_cast<const Impl&>(*this);
#ifdef UBPA_USE_XSIMD
			if constexpr (std::is_same_v<T, float> && N == 4)
				return x.get() / y.get();
			else
#endif // UBPA_USE_XSIMD
			{
				Impl rst{};
				for (size_t i = 0; i < N; i++)
					rst[i] = x[i] / y[i];
				return rst;
			}
		}

		Impl& operator/=(const Impl& y) noexcept {
			auto& x = static_cast<Impl&>(*this);
#ifdef UBPA_USE_XSIMD
			if constexpr (std::is_same_v<T, float> && N == 4)
				x.get() /= y.get();
			else
#endif // UBPA_USE_XSIMD
			for (size_t i = 0; i < N; i++)
				x[i] /= y[i];
			return x;
		}

		inline const Impl inverse() const noexcept {
			auto& x = static_cast<const Impl&>(*this);

#ifdef UBPA_USE_XSIMD
			if constexpr (std::is_same_v<T, float> && N == 4)
				return 1.f / x;
			else
#endif // UBPA_USE_XSIMD
			{
				Impl rst{};
				for (size_t i = 0; i < N; i++)
					rst[i] = static_cast<F>(1) / x[i];
				return rst;
			}
		}

	private:
		template<typename Base, typename Impl, typename ArgList>
		friend struct IMul;

		inline const Impl impl_mul(const Impl& y) const noexcept {
			auto& x = static_cast<const Impl&>(*this);

#ifdef UBPA_USE_XSIMD
			if constexpr (std::is_same_v<T, float> && N == 4)
				return x.get() * y.get();
			else
#endif // UBPA_USE_XSIMD
			{
				Impl rst{};
				for (size_t i = 0; i < N; i++)
					rst[i] = x[i] * y[i];
				return rst;
			}
		}
	};
}
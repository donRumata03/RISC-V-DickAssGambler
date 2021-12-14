#pragma once

/// Why isn't this in std?

#include "pch.h"

template <typename T>
class Future
{
	class Promise
	{
	public:
		using value_type = std::optional<T>;

		Promise() = default;
		std::suspend_always initial_suspend() { return {}; }
		std::suspend_always final_suspend() noexcept { return {}; }
		void unhandled_exception() {
			std::rethrow_exception(std::move(std::current_exception()));
		}

		std::suspend_always yield_value(T value) {
			this->value = std::move(value);
			return {};
		}

		// void return_value(T value) {
		//     this->value = std::move(value);
		// }

		void return_void() {
			this->value = std::nullopt;
		}

		inline Future get_return_object();

		value_type get_value() {
			return std::move(value);
		}

		bool finished() {
			return !value.has_value();
		}

	private:
		value_type value{};
	};

public:
	using value_type = T;
	using promise_type = Promise;

	explicit Future(std::coroutine_handle<Promise> handle)
			: handle (handle)
	{}

	~Future() {
		if (handle) { handle.destroy(); }
	}

	Promise::value_type next() {
		if (handle) {
			handle.resume();
			return handle.promise().get_value();
		}
		else {
			return {};
		}
	}

private:
	std::coroutine_handle<Promise> handle;
};


template <typename T>
inline Future<T> Future<T>::Promise::get_return_object()
{
	return Future{ std::coroutine_handle<Promise>::from_promise(*this) };
}
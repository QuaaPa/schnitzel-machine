#ifndef SM_CORE_RESULT_H_
#define SM_CORE_RESULT_H_

#include <variant>
#include <string>

// --------------------------------------
//
// Implement of the bool/VkResult wrapper
//
// --------------------------------------

// namespace SM {    
//     template<typename T, typename E = std::string>
//     class Result {
//         std::variant<T, E> data;
//         bool ok;
//     public:
//         static Result Ok(T value) { return Result{std::move(value), true}; }
//         static Result Err(E error) { return Result{std::move(error), false}; }

//         bool isOk() const { return ok; }
//         T& value() { return std::get<T>(data); }
//         const E& error() const { return std::get<E>(data); }
//     };
// }

#endif // SM_CORE_RESULT_H_

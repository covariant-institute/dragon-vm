//
// Created by kiva on 2017/9/25.
//

#include <core/object/method.hpp>

namespace dvm {
    namespace core {
        namespace runtime {
            class Invocation {
            public:
                /**
                 * Look up a method in VM.
                 *
                 * @param thread Executing thread
                 * @param name_id Constant id of method name
                 * @param signature_id Constant id of method signature
                 * @return Resolved method
                 */
                static inline object::Method *resolve_by_id(Thread *thread, UInt16 name_id, UInt16 signature_id) {
                    VMContext *context = thread->get_context();
                    const std::string &name = context->find_constant(name_id);
                    const std::string &signature = context->find_constant(signature_id);

                    return thread->get_context()->resolve_method(name, signature);
                }

                /**
                 * Invoke a method
                 *
                 * @param thread Executing thread
                 * @param method Method to be executed
                 */
                static inline void invoke_method_raw(Thread *thread, object::Method *method) {
                    Invocation::before_invoke(thread, method);

                    // Method::invoke() only sets thread->pc to its body start
                    // or just invoked a native function
                    method->invoke(thread);
                }

                /**
                 * Safe and quick Method::invoke() wrapper function.
                 *
                 * @param thread Executing thread
                 * @param method Method to be executed
                 * @param return_size Method return value's size
                 */
                static inline void invoke_simple(Thread *thread, object::Method *method,
                                                 SizeT return_size) {
                    // ensure that we can exit successfully.
                    auto exit_code = static_cast<Byte>(VMOpcodes::halt);

                    // set current pc to exit_code, so when the method returns,
                    // the program ends.
                    thread->pc = &exit_code;

                    // create initial frame, just as a data holder.
                    // In order to let our's caller be accessible to the return value,
                    // do not call remove_top_frame(), let the caller do that!
                    thread->stack.new_frame(return_size);

                    invoke_method_raw(thread, method);

                    // start interpreting
                    thread->run();

                    // clear pc
                    thread->pc = nullptr;
                }

                /**
                 * Invoke a method and get the return value.
                 *
                 * @tparam T Type of return value.
                 * @param thread Executing thread.
                 * @param method Method to be executed.
                 * @return
                 */
                template <typename T>
                static inline T invoke_get_result(Thread *thread, object::Method *method) {
                    SizeT args_size = sizeof(T);
                    Invocation::invoke_simple(thread, method, args_size);
                    T ret = thread->get_stack().peek_pop<T>();
                    thread->get_stack().remove_top_frame();
                    return ret;
                }

                /**
                 * Invoke a method which returns void.
                 *
                 * @param thread Executing thread.
                 * @param method Method to be executed.
                 */
                static inline void invoke_void(Thread *thread, object::Method *method) {
                    Invocation::invoke_simple(thread, method, 0);
                    thread->get_stack().remove_top_frame();
                }

                /**
                 * Return a value from a method.
                 *
                 * @tparam T Type of return value
                 * @param thread Executing thread
                 * @param ret Return value
                 */
                template <typename T>
                static inline void return_from_method(Thread *thread, T ret) {
                    Invocation::return_dispose(thread);
                    thread->stack.push<T>(std::forward<T>(ret));
                }

                /**
                 * Return void from a method.
                 *
                 * @param thread Executing thread
                 */
                static inline void return_void(Thread *thread) {
                    Invocation::return_dispose(thread);
                }


            private:
                /**
                 * Prepare to invoke a method.
                 * Create frame and pass arguments to it.
                 *
                 * @param thread Executing thread
                 * @param method Method to be executed
                 */
                static inline void before_invoke(Thread *thread, object::Method *method) {
                    if (method == nullptr || thread == nullptr) {
                        return;
                    }

                    UInt16 locals_size = method->get_locals_size();
                    UInt16 args_size = method->get_args_size();

                    // share args_size bytes with current frame for argument passing
                    Frame *frame = thread->get_stack().new_frame(locals_size, args_size);

                    // return address
                    frame->set_last_pc(thread->pc);
                    frame->set_method(method);
                }

                /**
                 * Clean frame for the returning method,
                 * restore caller's pc
                 * and pass the return value to its caller.
                 *
                 * @param thread Executing thread
                 */
                static inline void return_dispose(Thread *thread) {
                    Frame *frame = thread->get_stack().current_frame();
                    if (frame == nullptr) {
                        return;
                    }

                    // restore return address
                    thread->pc = frame->get_last_pc();

                    auto method = frame->get_method();
                    thread->get_stack().remove_top_frame();

                    // clear args
                    if (method != nullptr) {
                        UInt16 args_size = method->get_args_size();
                        if (args_size > 0) {
                            thread->get_stack().pop(args_size);
                        }
                    }
                }
            };
        }
    }
}

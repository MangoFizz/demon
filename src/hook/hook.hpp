#ifndef DEMON__HOOK_HPP
#define DEMON__HOOK_HPP

#include <cstdint>
#include <vector>

namespace Demon {
    enum HookType {
        /**
         * This is intended to be called from the game to the library.
         *
         * This will patch the function at the given address.
         */
        GameToLib,

        /**
         * This is intended to be called from the library to the game.
         *
         * No patches will be done to the game.
         */
        LibToGame
    };

    enum ParameterStorageType {
        Register,
        Stack
    };

    enum ParameterRegister {
        ST0, ST1, ST2, ST3, ST4, ST5, ST6, ST7,
        EAX, EBX, ECX, EDX, ESI, EDI, EBP, ESP
    };

    class Hook {
        public:
            Hook(std::uintptr_t function_address) :
                function_address(function_address),
                hook_type(LibToGame) {
            }
            Hook(std::uintptr_t function_address, std::uintptr_t destination_address) :
                function_address(function_address),
                destination_address(destination_address),
                hook_type(GameToLib) {
            }

            /**
             * Push a parameter.
             *
             * If Register, index should be a ParameterRegister. Otherwise, it should be the stack offset.
             *
             * For GameToLib functions, it will convert these into using the stack.
             *
             * For LibToGame functions, it will convert from stack into these.
             */
            Hook &push_parameter(ParameterStorageType type, std::size_t index) {
                parameters.emplace_back(type, index);
                return *this;
            }

            /**
             * Specify that a return value should be handled.
             */
            Hook &has_return_value() {
                this->uses_return_value = true;
                return *this;
            }

            /**
             * Write the hook.
             *
             * @return hook address
             */
            void *write_hook();

        private:
            std::uintptr_t function_address;
            std::uintptr_t destination_address;
            HookType hook_type;
            bool uses_return_value = false;
            std::vector<std::pair<ParameterStorageType, std::size_t>> parameters;
    };

    void set_up_all_hooks();
}

#endif

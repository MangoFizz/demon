#include <windows.h>
#include <cstring>
#include <cstddef>
#include <cstdint>
#include <vector>
#include "hook/hook.hpp"

using namespace Demon;

static std::byte *hook_heap;
static const std::size_t hook_heap_size = 512 * 1024;
static std::size_t hook_heap_usage;

extern "C" BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved) {
    static WSADATA fun = {};

    switch(reason) {
        case DLL_PROCESS_ATTACH: {
            // Enable DEP (if doable) because executing code not marked as executable is bad
            SetProcessDEPPolicy(PROCESS_DEP_ENABLE);

            // Required because Halo Trial is hardcoded to write garbage to 0x00000000 to try to crash the game if the DLL is different.
            auto *crash_if_dll_is_different = reinterpret_cast<void *>(0x53C746);
            DWORD old_protection;
            VirtualProtect(crash_if_dll_is_different, 6, PAGE_EXECUTE_READWRITE, &old_protection);
            std::memset(crash_if_dll_is_different, 0x90, 6);
            VirtualProtect(crash_if_dll_is_different, 6, old_protection, &old_protection);

            // Allocate the heap
            hook_heap = reinterpret_cast<std::byte *>(VirtualAlloc(NULL, hook_heap_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE));
            hook_heap_usage = 0;

            set_up_all_hooks();

            // Once done, set the protection to execute/read only so we don't get pwned.
            VirtualProtect(hook_heap, hook_heap_size, PAGE_EXECUTE_READ, &old_protection);
            break;
        }
        case DLL_PROCESS_DETACH: {
            // Done
            VirtualFree(hook_heap, hook_heap_size, MEM_DECOMMIT | MEM_RELEASE);

            break;
        }
    }

    return TRUE;
}

void Hook::write_hook() {
    std::byte *hook_offset = hook_heap + hook_heap_usage;
    std::byte *starting_hook_offset = hook_offset;

    #define HOOK_PUSH_BYTE(value) *(hook_offset++) = static_cast<std::byte>(value)
    #define HOOK_PUSH_DWORD(value) *reinterpret_cast<std::uint32_t *>(hook_offset) = static_cast<std::uint32_t>(value); hook_offset += sizeof(std::uint32_t) \

    // Push flags and registers
    //
    // pushad, pushfd
    HOOK_PUSH_BYTE(0x9C);
    HOOK_PUSH_BYTE(0x60);

    // Current stack offset (0x20 bytes from pushad, 0x4 bytes from pushfd, 0x4 bytes from call)
    std::size_t starting_stack_offset = 0x28;
    std::size_t stack_offset = starting_stack_offset;

    // Push parameters
    if(!this->parameters.empty()) {
        for(auto start = this->parameters.rbegin(); start != this->parameters.rend(); start++) {
            auto &p = *start;
            switch(p.first) {
                case Stack: {
                    std::size_t offset = p.second + stack_offset;
                    // push dword ptr [esp + <offset>]
                    if(offset <= 0x7F) {
                        HOOK_PUSH_BYTE(0xFF);
                        HOOK_PUSH_BYTE(0x74);
                        HOOK_PUSH_BYTE(0x24);
                        HOOK_PUSH_BYTE(offset);
                    }
                    else {
                        HOOK_PUSH_BYTE(0xFF);
                        HOOK_PUSH_BYTE(0xB4);
                        HOOK_PUSH_BYTE(0x24);
                        HOOK_PUSH_DWORD(offset);
                    }
                    stack_offset += sizeof(std::uint32_t);
                    break;
                }
                case Register: {
                    switch (this->hook_type) {
                        case GameToLib: {
                            switch(p.second) {
                                case EAX:
                                    HOOK_PUSH_BYTE(0x50);
                                    break;
                                case EBX:
                                    HOOK_PUSH_BYTE(0x53);
                                    break;
                                case ECX:
                                    HOOK_PUSH_BYTE(0x51);
                                    break;
                                case EDX:
                                    HOOK_PUSH_BYTE(0x52);
                                    break;
                                case ESI:
                                    HOOK_PUSH_BYTE(0x56);
                                    break;
                                case EDI:
                                    HOOK_PUSH_BYTE(0x57);
                                    break;
                                case EBP:
                                    HOOK_PUSH_BYTE(0x55);
                                    break;
                                default:
                                    throw std::exception();
                            }
                            stack_offset += sizeof(std::uint32_t);
                            break;
                        }
                        case LibToGame: {
                            std::size_t offset = starting_stack_offset + stack_offset;
                            HOOK_PUSH_BYTE(0x8B);

                            std::uint8_t operand;

                            // mov <register>, <offset>
                            switch(p.second) {
                                case EAX:
                                    operand = 0x44;
                                    break;
                                case EBX:
                                    operand = 0x5C;
                                    break;
                                case ECX:
                                    operand = 0x4C;
                                    break;
                                case EDX:
                                    operand = 0x54;
                                    break;
                                case ESI:
                                    operand = 0x74;
                                    break;
                                case EDI:
                                    operand = 0x7C;
                                    break;
                                case EBP:
                                    operand = 0x6C;
                                    break;
                                default:
                                    throw std::exception();
                            }

                            if(offset <= 0x7F) {
                                HOOK_PUSH_BYTE(operand);
                                HOOK_PUSH_BYTE(0x24);
                                HOOK_PUSH_BYTE(offset);
                            }
                            else {
                                HOOK_PUSH_BYTE(operand + 0x40);
                                HOOK_PUSH_BYTE(0x24);
                                HOOK_PUSH_DWORD(offset);
                            }

                            break;
                        }
                    }
                    break;
                }
            }
        }
    }

    // Put a call function here
    //
    // call <function_address>
    auto *call_instruction_end = (hook_offset + 1 + sizeof(std::uintptr_t));
    std::uintptr_t call_offset;
    switch(this->hook_type) {
        case GameToLib:
            call_offset = reinterpret_cast<std::byte *>(this->destination_address) - call_instruction_end;
            break;
        case LibToGame:
            call_offset = reinterpret_cast<std::byte *>(this->function_address) - call_instruction_end;
            break;
    }
    HOOK_PUSH_BYTE(0xE8);
    HOOK_PUSH_DWORD(call_offset);

    // Cleanup stack
    //
    // add esp, <stack_to_cleanup>
    std::size_t stack_to_cleanup = stack_offset - starting_stack_offset;
    if(stack_to_cleanup != 0) {
        if(stack_to_cleanup < 0x80) {
            HOOK_PUSH_BYTE(0x83);
            HOOK_PUSH_BYTE(0xC4);
            HOOK_PUSH_BYTE(stack_to_cleanup & 0xFF);
        }
        else {
            HOOK_PUSH_BYTE(0x81);
            HOOK_PUSH_BYTE(0xC4);
            HOOK_PUSH_DWORD(stack_to_cleanup);
        }
    }

    // Return value? If so, set EAX in our stack to our current EAX value
    //
    // mov dword ptr [esp+0x1C], eax
    if(this->uses_return_value) {
        HOOK_PUSH_BYTE(0x89);
        HOOK_PUSH_BYTE(0x44);
        HOOK_PUSH_BYTE(0x24);
        HOOK_PUSH_BYTE(0x1C);
    }

    // Pop flags and registers
    //
    // popad, popfd
    HOOK_PUSH_BYTE(0x61);
    HOOK_PUSH_BYTE(0x9D);

    // Return
    //
    // ret
    HOOK_PUSH_BYTE(0xC3);

    #undef HOOK_PUSH_BYTE

    hook_heap_usage += (hook_offset - starting_hook_offset);

    // Jump if needed
    if(this->hook_type == GameToLib) {
        DWORD old_protection;
        auto *addr = reinterpret_cast<std::byte *>(this->function_address);
        VirtualProtect(reinterpret_cast<void *>(addr), 5, PAGE_READWRITE, &old_protection);
        *addr = static_cast<std::byte>(0xE9);
        *reinterpret_cast<std::uintptr_t *>(addr + 1) = static_cast<std::uintptr_t>((starting_hook_offset) - (addr + 5));
        VirtualProtect(reinterpret_cast<void *>(addr), 5, old_protection, &old_protection);
    }
}

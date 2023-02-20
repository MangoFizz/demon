.intel_syntax

.text

.globl _attack_mode
_attack_mode:
    pushfd
    pushad
    call _game_main_entry
    popad
    popfd

    pop edi
    pop esi
    pop ebx

    add esp, 0x2268
    add esp, 0x8
    pop eax
    add esp, 0xC
    pop ebp

    ret

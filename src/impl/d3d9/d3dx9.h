#ifndef DEMON__IMPL_D3D9_D3DX9_H
#define DEMON__IMPL_D3D9_D3DX9_H

#include <d3dx9.h>

#define CAST_VIRTUAL_METHOD(p, i, r) ((r (WINAPI *)())(*(uint32_t *)(((char *)(p)->lpVtbl) + i)))
#define CALL_VIRTUAL_METHOD(p, i, r, ...) CAST_VIRTUAL_METHOD(p, i, r)(p, __VA_ARGS__)
#define CALL_VIRTUAL_METHOD_NO_ARGS(p, i, r) CAST_VIRTUAL_METHOD(p, i, r)(p)

/**
 * ID3DXEffect interface helper macros
 */
#define ID3DXEffect_Release(p) CALL_VIRTUAL_METHOD_NO_ARGS(p, 0x08, HRESULT)
#define ID3DXEffect_GetParameterByName(p, s, n) CALL_VIRTUAL_METHOD(p, 0x24, D3DXHANDLE, s, n)
#define ID3DXEffect_GetTechniqueByName(p, n) CALL_VIRTUAL_METHOD(p, 0x34, D3DXHANDLE, n) 
#define ID3DXEffect_ValidateTechnique(p, t) CALL_VIRTUAL_METHOD(p, 0xF4, HRESULT, t) 
#define ID3DXEffect_FindNextValidTechnique(p, t, n) CALL_VIRTUAL_METHOD(p, 0xF8, HRESULT, t, n) 
#define ID3DXEffect_SetTechnique(p, t) CALL_VIRTUAL_METHOD(p, 0xEC, HRESULT, t)

/**
 * ID3DXEffectPool interface helper macros
 */
#define ID3DXEffectPool_Release(p) CALL_VIRTUAL_METHOD_NO_ARGS(p, 0x08, HRESULT)

#endif

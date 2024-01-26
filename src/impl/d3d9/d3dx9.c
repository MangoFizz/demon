#include <windows.h>
#include <d3dx9.h>

extern HRESULT (*d3dx9_create_effect)(struct IDirect3DDevice9 *device, const void *srcdata, UINT srcdatalen, const D3DXMACRO *defines, struct ID3DXInclude *include, DWORD flags, struct ID3DXEffectPool *pool, struct ID3DXEffect **effect, struct ID3DXBuffer **compilation_errors); 
extern HRESULT (*d3dx9_create_effect_pool)(ID3DXEffectPool **pool); 

HRESULT WINAPI D3DXCreateEffect(struct IDirect3DDevice9 *device, const void *srcdata, UINT srcdatalen, const D3DXMACRO *defines, struct ID3DXInclude *include, DWORD flags, struct ID3DXEffectPool *pool, struct ID3DXEffect **effect, struct ID3DXBuffer **compilation_errors) {
    return d3dx9_create_effect(device, srcdata, srcdatalen, defines, include, flags, pool, effect, compilation_errors);
}

HRESULT WINAPI D3DXCreateEffectPool(ID3DXEffectPool **pool) {
    return d3dx9_create_effect_pool(pool);
}

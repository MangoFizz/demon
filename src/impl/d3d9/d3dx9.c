#include <windows.h>
#include <d3dx9.h>
#include <d3dx9mesh.h>

extern HRESULT (*d3dx_create_effect)(); 
extern HRESULT (*d3dx_create_effect_pool)(); 
extern HRESULT (*d3dx_fvf_from_declarator)();

HRESULT WINAPI D3DXCreateEffect(struct IDirect3DDevice9 *device, const void *srcdata, UINT srcdatalen, const D3DXMACRO *defines, struct ID3DXInclude *include, DWORD flags, struct ID3DXEffectPool *pool, struct ID3DXEffect **effect, struct ID3DXBuffer **compilation_errors) {
    return d3dx_create_effect(device, srcdata, srcdatalen, defines, include, flags, pool, effect, compilation_errors);
}

HRESULT WINAPI D3DXCreateEffectPool(ID3DXEffectPool **pool) {
    return d3dx_create_effect_pool(pool);
}

HRESULT WINAPI D3DXFVFFromDeclarator(const D3DVERTEXELEMENT9 *decl, DWORD *fvf) {
    return d3dx_fvf_from_declarator(decl, fvf);
}

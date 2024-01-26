#ifndef DEMON__IMPL_D3D9_D3DX9_H
#define DEMON__IMPL_D3D9_D3DX9_H

#include <d3dx9.h>

/**
 * ID3DXEffect interface helper macros
 */
#define ID3DXEffect_Release(p) (p)->lpVtbl->Release(p)
#define ID3DXEffect_GetTechniqueByName(p, n) (p)->lpVtbl->GetTechniqueByName(p, n)
#define ID3DXEffect_ValidateTechnique(p, t) (p)->lpVtbl->ValidateTechnique(p, t)
#define ID3DXEffect_SetTechnique(p, t) (p)->lpVtbl->SetTechnique(p, t)
#define ID3DXEffect_FindNextValidTechnique(p, t, n) (p)->lpVtbl->FindNextValidTechnique(p, t, n)
#define ID3DXEffect_GetParameterByName(p, s, n) (p)->lpVtbl->GetParameterByName(p, s, n)

/**
 * ID3DXBuffer interface helper macros
 */
#define ID3DXBuffer_Release(p) (p)->lpVtbl->Release(p)

#endif

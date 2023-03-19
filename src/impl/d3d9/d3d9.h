#ifndef DEMON__IMPL_D3D9_D3D9_H
#define DEMON__IMPL_D3D9_D3D9_H

#include <d3d9.h>

/**
 * Load D3D9. Crashes if failed.
 */
void load_d3d9(void);

/**
 * Get the D3D9 device.
 */
IDirect3DDevice9 **get_d3d9_device(void);

#endif

#ifndef STDAFX_H
#define STDAFX_H

#include <windows.h>
#include <mmsystem.h>
#include <pdh.h>

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx11async.h>
#include <d3dx10math.h>
#include <d3dx11tex.h>
#include <dinput.h>

#include <fstream>
#include <memory>
#include <algorithm>
#include <functional>
#include <vector>

#include "Timer.h"
#include "FPSManager.h"
#include "CPUManager.h"
#include "InputManager.h"

#include "Direct3D.h"
#include "Texture.h"
#include "Font.h"
#include "Bitmap.h"

#include "FontShader.h"
#include "TextureShader.h"
#include "LightShader.h"
#include "ShaderManager.h"

#include "Component.h"
#include "Transform.h"
#include "Collider.h"
#include "BoxCollider.h"
#include "GameObject.h"
#include "Scene.h"

#include "Camera.h"
#include "modelclass.h"
#include "lightclass.h"
#include "textclass.h"
#include "SkyBox.h"

#include "system.h"

#endif
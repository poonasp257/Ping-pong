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
#include <dsound.h>

#include <cstdlib>
#include <cstdarg>
#include <string>
#include <memory>
#include <algorithm>
#include <functional>
#include <fstream>

#include <array>
#include <vector>
#include <list>

#include "Util/Type.h"

#include "Components/Component.h"
#include "Components/Transform.h"
#include "Components/Collider.h"
#include "Components/BoxCollider.h"

#include "Core/Direct3D.h"
#include "Core/Camera.h"
#include "Core/Timer.h"
#include "Core/FPSManager.h"
#include "Core/CPUManager.h"
#include "Core/InputManager.h"
#include "Core/Light.h"
#include "Core/Texture.h"
#include "Core/Model.h"
#include "Core/Font.h"
#include "Core/Text.h"
#include "Core/Bitmap.h"
#include "Core/SkyBox.h"
#include "Core/GameObject.h"
#include "Core/Scene.h"
#include "Core/Sound.h"
#include "Core/System.h"

#include "Shaders/FontShader.h"
#include "Shaders/TextureShader.h"
#include "Shaders/LightShader.h"
#include "Shaders/ShaderManager.h"

#endif
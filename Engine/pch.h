// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H

// 여기에 미리 컴파일하려는 헤더 추가
#include "framework.h"
#include "../D3DRenderer/Helper.h"
#include "define.h"

#include <Windows.h>
#include <wrl/client.h>

#include <d3d11.h>
#include <DirectXMath.h>
#include <directxtk/SimpleMath.h>
#include <d2d1.h>
#include <d2d1_1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <DirectXColors.h>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>

// --------------------------------------
// 건재 : 그래픽스 엔진 외부 헤더 파일 추가
#include <memory>
#include <string>
#include <string_view>
#include <vector>
#include <list>
#include <iostream>
#include <functional>
#include <concepts>
#include <map>

#include <DirectXtk/BufferHelpers.h>
#include <directxtk/SimpleMath.h>
#include <dxgi1_4.h>

using namespace Microsoft::WRL;
using namespace DirectX::SimpleMath;
using namespace DirectX;
namespace Math = DirectX::SimpleMath;

// --------------------------------------

#include "../D3DRenderer/pch.h"

// 네트워크
constexpr int SND_BUF_SIZE = 1024;
constexpr int RCV_BUF_SIZE = 9604;

#endif //PCH_H

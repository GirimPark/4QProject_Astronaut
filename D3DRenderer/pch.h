// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H

// 여기에 미리 컴파일하려는 헤더 추가
#include "framework.h"
#include "Helper.h"

// 건재 : STL
#include <wchar.h>
#include <d3d11.h>
#include <exception>
#include <vector>
#include <stdio.h>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <fstream>
#include <stdexcept>
#include <system_error>
#include <stdio.h>
#include <locale>
#include <codecvt>
#include <string>
#include <memory>
#include <list>
#include <map>
#include <queue>
#include <filesystem>
#include <wrl.h>
#include <String>
#include <Psapi.h>
using namespace std;

// 건재 : DirectX
#include <d3d11.h>
#include <d2d1.h>
#include <d2d1_1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <DirectXColors.h>

#include <dxgi1_4.h>
#include <DirectXtk/BufferHelpers.h>
#include <directxtk/SimpleMath.h>
#include <directxtk\DDSTextureLoader.h>
#include <directxtk\WICTextureLoader.h>
using namespace Microsoft::WRL;
using namespace DirectX::SimpleMath;
using namespace DirectX;
namespace Math = DirectX::SimpleMath;

// 건재 : Assimp
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <assimp/scene.h>
#include <assimp/anim.h>
#include <assimp/quaternion.h>


#endif //PCH_H

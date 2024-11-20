#pragma once

struct BlockPOD;
struct PartPOD;
enum class eBlockType;
enum class ePartType;

// 밖에서는 파일 읽어오고
// 블록 제너레이터 동작시킴
// 블록 정보는 그리드매니저도 접근할 수 있어야 한다.
// 그러므로 블록 정보 자체는 월드에 저장하기
// 

// 데이터 파싱하는 클래스
// 블록 제너레이터 하나 더 만들기
class YAMLParser
{
public:
	YAMLParser() = default;
	~YAMLParser() = default;

	void ReadBlockPOD(std::string_view filePath, std::vector<std::shared_ptr<BlockPOD>>& blockPODs);
	void ReadPartPOD(std::string_view filePath, std::vector<std::shared_ptr<PartPOD>>& partPODs);

private:
	eBlockType GetBlockType(std::string_view blockName);
	ePartType GetPartType(std::string_view partName);
};


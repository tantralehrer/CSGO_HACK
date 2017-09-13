#include "Chams.hpp"

//#include "../../CSGO_SDK/IVModelInfo.hpp"

//#include "../../Globals.hpp"
//
//#include <fstream>
//#include <sstream>
//
//void Chams::CreateMaterials()
//{
//	std::stringstream chams;
//	std::stringstream chamsIgnorez;
//	std::stringstream chamsFlat;
//	std::stringstream chamsFlatIgnorez;
//	std::wstring chamsPath = Globals::CheatPath + L"\\omdis_chams.vmt";
//	std::wstring chamsIgnorezPath = Globals::CheatPath + L"\\omdis_chamsIgnorez.vmt";
//	std::wstring chamsFlatPath = Globals::CheatPath + L"\\omdis_chamsFlat.vmt";
//	std::wstring chamsFlatIgnorezPath = Globals::CheatPath + L"\\omdis_chamsFlatIgnorez.vmt";
//
//	chams << "\"VertexLitGeneric\"" << std::endl;
//	chams << "{" << std::endl;
//	chams << "\t\"$basetexture\" \"VGUI/white_additive\"" << std::endl;
//	chams << "\t\"$ignorez\" \"0\"" << std::endl;
//	chams << "\t\"$nofog\" \"1\"" << std::endl;
//	chams << "\t\"$model\" \"1\"" << std::endl;
//	chams << "\t\"$nocull\" \"1\"" << std::endl;
//	chams << "\t\"$halflambert\" \"1\"" << std::endl;
//	chams << "}" << std::endl;
//
//	chamsIgnorez << "\"VertexLitGeneric\"" << std::endl;
//	chamsIgnorez << "{" << std::endl;
//	chamsIgnorez << "\t\"$basetexture\" \"VGUI/white_additive\"" << std::endl;
//	chamsIgnorez << "\t\"$ignorez\" \"1\"" << std::endl;
//	chamsIgnorez << "\t\"$nofog\" \"1\"" << std::endl;
//	chamsIgnorez << "\t\"$model\" \"1\"" << std::endl;
//	chamsIgnorez << "\t\"$nocull\" \"1\"" << std::endl;
//	chamsIgnorez << "\t\"$halflambert\" \"1\"" << std::endl;
//	chamsIgnorez << "}" << std::endl;
//
//	chamsFlat << "\"UnlitGeneric\"" << std::endl;
//	chamsFlat << "{" << std::endl;
//	chamsFlat << "\t\"$basetexture\" \"VGUI/white_additive\"" << std::endl;
//	chamsFlat << "\t\"$ignorez\" \"0\"" << std::endl;
//	chamsFlat << "\t\"$nofog\" \"1\"" << std::endl;
//	chamsFlat << "\t\"$model\" \"1\"" << std::endl;
//	chamsFlat << "\t\"$nocull\" \"1\"" << std::endl;
//	chamsFlat << "\t\"$halflambert\" \"1\"" << std::endl;
//	chamsFlat << "}" << std::endl;
//
//	chamsFlatIgnorez << "\"UnlitGeneric\"" << std::endl;
//	chamsFlatIgnorez << "{" << std::endl;
//	chamsFlatIgnorez << "\t\"$basetexture\" \"VGUI/white_additive\"" << std::endl;
//	chamsFlatIgnorez << "\t\"$ignorez\" \"1\"" << std::endl;
//	chamsFlatIgnorez << "\t\"$nofog\" \"1\"" << std::endl;
//	chamsFlatIgnorez << "\t\"$model\" \"1\"" << std::endl;
//	chamsFlatIgnorez << "\t\"$nocull\" \"1\"" << std::endl;
//	chamsFlatIgnorez << "\t\"$halflambert\" \"1\"" << std::endl;
//	chamsFlatIgnorez << "}" << std::endl;
//
//	std::ofstream(chamsPath) << chams.str();
//	std::ofstream(chamsIgnorezPath) << chamsIgnorez.str();
//	std::ofstream(chamsFlatPath) << chamsFlat.str();
//	std::ofstream(chamsFlatIgnorezPath) << chamsFlatIgnorez.str();
//}
//
//void ChamsPlayer(void)
//{
//
//}
//
//void Chams::DrawModelExecute(void *ThisPtr, void *Context, void *State, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld)
//{
//	if (!pInfo.pModel)
//		return;
//
//	std::string ModelName = Globals::ModelInfo->GetModelName(pInfo.pModel);
//
//	if (ModelName.find("models/player") != std::string::npos)
//	{
//
//	}
//}
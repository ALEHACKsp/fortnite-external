#pragma once

#include "world.h"

bool IsVisible(uintptr_t TargetPawn)
{
	auto targetMesh = ReadPointer<uint64_t>(TargetPawn + 0x318);
	auto localMesh = ReadPointer<DWORD_PTR>(offname::localactorpawn + 0x318);

	if (targetMesh && localMesh)
	{
		float targetLastRenderTimeOnScreen = ReadPointer<float>(targetMesh + 0x318);
		float localLastRenderTimeOnScreen = ReadPointer<float>(localMesh + 0x318);

		if (localLastRenderTimeOnScreen == targetLastRenderTimeOnScreen)
			return true;
	}
	return false;
}

void draw_esp()
{
	float closestDistance = FLT_MAX;
	DWORD_PTR closestPawn = NULL;
	float closestactor = FLT_MAX;
	for (unsigned long i = 0; i < allplayers.size(); ++i)
	{
		actors entity = allplayers[i];
		uintptr_t entitymesh = ReadPointer<uintptr_t>(entity.ACTOR + Offsets::External::MESH); // need
		uint64_t rootcomp = ReadPointer<uint64_t>(entity.ACTOR + Offsets::External::ROOTCOMPONENT);
		Vector3 actorpos = ReadPointer<Vector3>(rootcomp + Offsets::External::RELATIVELOC);
		Vector3 actorposW2s = w2s(actorpos);

		DWORD64 otherPlayerState = ReadPointer<uint64_t>(entity.ACTOR + Offsets::External::ACTORSTATE);
		Vector3 Headpos = bonerota(entitymesh, 98); // need
		uintptr_t localroot = ReadPointer<uintptr_t>(offname::localactorpawn + Offsets::External::ROOTCOMPONENT);
		localactorpos = ReadPointer<Vector3>(localroot + Offsets::External::RELATIVELOC);

		float distance = localactorpos.Distance(actorpos) / 100.f;

		Vector3 bone0 = bonerota(entitymesh, 0);
		Vector3 bonepp = bonerota(entitymesh, 2);
		Vector3 bottom = w2s(bone0);
		Vector3 pplol = w2s(bonepp);
		Vector3 Headbox = w2s(Vector3(Headpos.x, Headpos.y, Headpos.z + 15));
		Vector3 w2shead = w2s(Headpos); // need

		float BoxHeight = (float)(Headbox.y - bottom.y);
		float BoxWidth = BoxHeight * 0.250;

		float LeftX = (float)Headbox.x - (BoxWidth / 1);
		float LeftY = (float)bottom.y;

		float CornerHeight = abs(Headbox.y - bottom.y);
		float CornerWidth = CornerHeight * features::boxwidthlol;

		int MyTeamId = ReadPointer<int>(offname::actorstate + Offsets::External::TEAMID);
		int ActorTeamId = ReadPointer<int>(otherPlayerState + Offsets::External::TEAMID);

		auto visiblecheckbyte = ReadPointer<int>(entitymesh + 0x707);

		bool bSpotted = false;
		if (visiblecheckbyte == 2703442)
		{
			bSpotted = true;
		}


		if (MyTeamId != ActorTeamId && offname::localactorpawn && distance < 250)
		{
			if (features::boxes)
			{
				if (entity.name == XorStr("NPC").c_str())
				{
					ImGui::GetBackgroundDrawList()->AddRect(ImVec2(LeftX, LeftY), ImVec2(Headbox.x + BoxWidth, Headbox.y), IM_COL32(50, 200, 0, 255), 0, 15, 0.2f);
				}
				else if (entity.name == XorStr("player").c_str() || entity.name == XorStr("bot").c_str() || entity.name == XorStr("guard").c_str())
				{
					if (bSpotted)
					{
						ImGui::GetBackgroundDrawList()->AddRect(ImVec2(LeftX, LeftY), ImVec2(Headbox.x + BoxWidth, Headbox.y), IM_COL32(255, 255, 255, 255), 0, 15, 0.2f);
					}
					else
					{
						ImGui::GetBackgroundDrawList()->AddRect(ImVec2(LeftX, LeftY), ImVec2(Headbox.x + BoxWidth, Headbox.y), IM_COL32(210, 190, 0, 255), 0, 15, 0.2f);
					}
				}
			}

			char dist[64];
			std::string nichon;
			ImU32 color;
			if (entity.name == XorStr("player").c_str())
			{
				color = IM_COL32(255, 0, 0, 255);
				nichon = XorStr("Joueur [%.fm]");
			}
			else if (entity.name == XorStr("bot").c_str())
			{
				color = IM_COL32(255, 0, 0, 255);
				nichon = XorStr("IA [%.fm]");
			}
			else if (entity.name == XorStr("NPC").c_str())
			{
				color = IM_COL32(153, 226, 180, 255);
				nichon = XorStr("NPC [%.fm]");
			}
			else if (entity.name == XorStr("guard").c_str())
			{
				color = IM_COL32(255, 189, 0, 255);
				nichon = XorStr("IA [%.fm]");
			}

			if (features::distance)
			{
				sprintf_s(dist, nichon.c_str(), distance);
				outlinedtext(l_pFont, dist, ImVec2(bottom.x, bottom.y), 13.0f, color, true);
			}

			if (features::lines)
			{
				if (features::lineesppos == 1)
				{
					if (features::allan)
					{
						lineesp(Width / 2, Height / 2, pplol.x, pplol.y, IM_COL32(255, 0, 228, 255), 0.2f);
					}
					else
					{
						lineesp(Width / 2, Height, pplol.x, pplol.y, IM_COL32(255, 255, 255, 255), 0.2f);
					}
				}
				else if (features::lineesppos == 2)
				{
					if (features::allan)
					{
						lineesp(Width / 2, Height / 2, bottom.x, bottom.y, IM_COL32(255, 0, 228, 255), 0.2f);
					}
					else
					{
						lineesp(Width / 2, Height, bottom.x, bottom.y, IM_COL32(255, 255, 255, 255), 0.2f);
					}
				}
				else if (features::lineesppos == 0)
				{
					if (features::allan)
					{
						lineesp(Width / 2, Height / 2, Headbox.x, Headbox.y, IM_COL32(255, 0, 228, 255), 0.2f);
					}
					else
					{
						lineesp(Width / 2, Height, Headbox.x, Headbox.y, IM_COL32(255, 255, 255, 255), 0.2f);
					}
				}
			}

		}
		auto dx = w2shead.x - (Width / 2);
		auto dy = w2shead.y - (Height / 2);
		auto dist = sqrtf(dx * dx + dy * dy);
		if (dist < features::fov && dist < closestDistance) {
			uintptr_t thisone;
			if (closestactor > distance)
			{
				closestactor = distance;
				thisone = entity.ACTOR;
			}
			closestDistance = dist;
			if (MyTeamId != ActorTeamId && entity.ACTOR == thisone)
			{
				closestPawn = entity.ACTOR;
			}
		}
	}
	for (unsigned long i = 0; i < weakspot.size(); ++i)
	{
		actors weakpointlol = weakspot[i];

		uintptr_t entitymesh = ReadPointer<uintptr_t>(weakpointlol.ACTOR + Offsets::External::MESH);
		Vector3 Headpos = bonerota(entitymesh, 0);
		Vector3 w2shead = w2s(Headpos);

		auto dx = w2shead.x - (Width / 2);
		auto dy = w2shead.y - (Height / 2);
		auto dist = sqrtf(dx * dx + dy * dy);
		if (dist < features::fov && dist < closestDistance) {
			closestDistance = dist;
			if (weakpointlol.name == XorStr("weakpoint").c_str())
			{
				closestPawn = weakpointlol.ACTOR;
			}
		}
	}

	if (features::aimbot)
	{
		if (features::aimbot && closestPawn && GetAsyncKeyState(features::aimkey) < 0) {
			targetbone(closestPawn);
		}
	}
}

void nikezbiloop()
{
	for (unsigned long i = 0; i < levelloot.size(); ++i)
	{
		actors elements = levelloot[i];

		uint64_t rootcomp = ReadPointer<uint64_t>(elements.ACTOR + Offsets::External::ROOTCOMPONENT);
		Vector3 elementpos = ReadPointer<Vector3>(rootcomp + Offsets::External::RELATIVELOC);
		Vector3 elementposW2s = w2s(elementpos);

		uintptr_t localroot = ReadPointer<uintptr_t>(offname::localactorpawn + Offsets::External::ROOTCOMPONENT);
		localactorpos = ReadPointer<Vector3>(localroot + Offsets::External::RELATIVELOC);

		float distance = localactorpos.Distance(elementpos) / 100.f;

		auto penislmao = ReadPointer<bool>(elements.ACTOR + 0xf49);

		if (features::chest && elements.name == XorStr("chest").c_str() && distance <= 60 && penislmao == 9)
		{
			char chest[64];
			sprintf(chest, XorStr(" Chest \n[%.fm]").c_str(), distance);
			outlinedtext(l_pFont, chest, ImVec2(elementposW2s.x, elementposW2s.y), 11.0f, IM_COL32(255, 255, 0, 255), true);
		}
		else if (features::ammo && elements.name == XorStr("ammo").c_str() && distance <= 60 && penislmao == 9)
		{
			char ammobox[64];
			sprintf(ammobox, XorStr(" Ammo Box \n[%.fm]").c_str(), distance);
			outlinedtext(l_pFont, ammobox, ImVec2(elementposW2s.x, elementposW2s.y), 11.0f, IM_COL32(0, 255, 0, 255), true);
		}
	}
}

void lootesploop()
{
	for (unsigned long i = 0; i < itemsloot.size(); ++i)
	{
		actors loot = itemsloot[i];

		uint64_t rootcomp = ReadPointer<uint64_t>(loot.ACTOR + 0x188);
		Vector3 elementpos = ReadPointer<Vector3>(rootcomp + Offsets::External::RELATIVELOC);
		Vector3 elementposW2s = w2s(elementpos);

		uintptr_t localroot = ReadPointer<uintptr_t>(offname::localactorpawn + Offsets::External::ROOTCOMPONENT);
		localactorpos = ReadPointer<Vector3>(localroot + Offsets::External::RELATIVELOC);

		float distance = localactorpos.Distance(elementpos) / 100.f;

		std::string weaponcustomname = loot.name;

		if (strstr(loot.name.c_str(), "Assault"))
		{
			weaponcustomname = "Assault Rifle [%.fm]";
		}
		else if (strstr(loot.name.c_str(), "Shotgun"))
		{
			weaponcustomname = "Pump Shotgun [%.fm]";
		}
		else if (strstr(loot.name.c_str(), "Shield"))
		{
			weaponcustomname = "Shields [%.fm]";
		}

		if (features::debug)
		{
			char ammobox[64];
			sprintf(ammobox, weaponcustomname.c_str(), distance);
			outlinedtext(l_pFont, ammobox, ImVec2(elementposW2s.x, elementposW2s.y), 11.0f, IM_COL32(0, 255, 0, 255), true);
		}
	}
}
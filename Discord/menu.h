#pragma once 
#include "include.h"
#include "renderutils.h"
#include "render_esp.h"

void menu()
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (features::aimkeypos == 0)
	{
		features::aimkeystr = XorStr("LMB").c_str();
		features::aimkey = 0x01; // left mouse button
	}
	else if (features::aimkeypos == 1)
	{
		features::aimkeystr = XorStr("RMB").c_str();
		features::aimkey = 0x02; // right mouse button
	}
	else if (features::aimkeypos == 2)
	{
		features::aimkeystr = XorStr("ALT").c_str();
		features::aimkey == 0x12; // alt
	}
	else if (features::aimkeypos == 3)
	{
		features::aimkeystr = XorStr("CTRL").c_str();
		features::aimkey == 0x11; // control
	}

	if (features::fovcirclee)
	{
		fovcircle(Width / 2, Height / 2, features::fov, IM_COL32(0,0,0,255), 1000, 0.5f);
	}

	if (features::crosshair)
	{
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(949, 539), ImVec2(969, 539), IM_COL32(0, 0, 0, 255), features::crossthick);
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(959, 529), ImVec2(959, 549), IM_COL32(0, 0, 0, 255), features::crossthick);
	}

	if (features::hitboxpos == 0)
	{
		features::hitboxposstr = XorStr("head").c_str();
		features::hitbox = 98;
	}
	else if (features::hitboxpos == 1)
	{
		features::hitboxposstr = XorStr("neck").c_str();
		features::hitbox = 66;
	}
	else if (features::hitboxpos == 2)
	{
		features::hitboxposstr = XorStr("chest").c_str();
		features::hitbox = 37;
	}
	else if (features::hitboxpos == 3)
	{
		features::hitboxposstr = XorStr("legs").c_str();
		features::hitbox = 2;
	}

	if (features::lineesppos == 0)
	{
		features::lineespposstr = XorStr("top").c_str();
	}
	else if (features::lineesppos == 1)
	{
		features::lineespposstr = XorStr("mid").c_str();
	}
	else if (features::lineesppos == 2)
	{
		features::lineespposstr = XorStr("bottom").c_str();
	}

	draw_esp();
	nikezbiloop();
	lootesploop();

	if (features::menu)
	{
		if (GetAsyncKeyState(VK_DOWN) & 1)
		{
			if (features::max_options != 15)
				features::max_options += 1;
		}
		if (GetAsyncKeyState(VK_UP) & 1)
		{
			if (features::max_options != 1)
				features::max_options -= 1;
		}

		ImGui::GetForegroundDrawList()->AddImage((void*)bgtext, ImVec2(133, 230), ImVec2(253, 350), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 35));

		if (features::aimkeypos == 0)
		{
			features::aimkeystr = XorStr("LMB").c_str();
			features::aimkey = 0x01; // left mouse button
		}
		else if (features::aimkeypos == 1)
		{
			features::aimkeystr = XorStr("RMB").c_str();
			features::aimkey = 0x02; // right mouse button
		}
		else if (features::aimkeypos == 2)
		{
			features::aimkeystr = XorStr("ALT").c_str();
			features::aimkey == 0x12; // alt
		}
		else if (features::aimkeypos == 3)
		{
			features::aimkeystr = XorStr("CTRL").c_str();
			features::aimkey == 0x11; // control
		}

		if (features::hitboxpos == 0)
		{
			features::hitboxposstr = XorStr("head").c_str();
			features::hitbox = 98;
		}
		else if (features::hitboxpos == 1)
		{
			features::hitboxposstr = XorStr("neck").c_str();
			features::hitbox = 66;
		}
		else if (features::hitboxpos == 2)
		{
			features::hitboxposstr = XorStr("chest").c_str();
			features::hitbox = 37;
		}
		else if (features::hitboxpos == 3)
		{
			features::hitboxposstr = XorStr("legs").c_str();
			features::hitbox = 2;
		}

		if (features::lineesppos == 0)
		{
			features::lineespposstr = XorStr("top").c_str();
		}
		else if (features::lineesppos == 1)
		{
			features::lineespposstr = XorStr("mid").c_str();
		}
		else if (features::lineesppos == 2)
		{
			features::lineespposstr = XorStr("bottom").c_str();
		}

		if (GetAsyncKeyState(VK_DOWN) & 1)
		{
			if (features::max_options != 15)
				features::max_options += 1;
		}
		if (GetAsyncKeyState(VK_UP) & 1)
		{
			if (features::max_options != 1)
				features::max_options -= 1;
		}

		Render::RectFilled(80, 200, 230, 365, ImColor(0, 0, 0));
		Render::RectFilled(80, 150, 230, 50, ImColor(0, 0, 0));
		Render::Rect(80, 200, 230, 365, ImColor(255, 255, 255), 1);
		Render::Rect(80, 150, 230, 50, ImColor(255, 255, 255), 1);
		ImGui::PushFont(font1);
		Render::Text(93, 163, ImColor(220, 220, 0, 255), XorStr("omelette - tastes good huh?").c_str());
		ImGui::PopFont();
		ImGui::PushFont(font2);

		ImColor selectedcolor = IM_COL32(220, 220, 0, 255);

		if (features::max_options > 0 && features::max_options < 6) { Render::Text(90, 225, ImColor(220, 220, 0, 255), "AIM "); }
		else { Render::Text(90, 225, ImColor(125, 125, 0, 255), XorStr("AIM ").c_str()); } //aim category

		Render::Text(95, 240, ImColor(255, 255, 255, 255), XorStr("Aimbot ").c_str()); //aimbot features
		if (features::max_options == 1)
		{
			if (GetAsyncKeyState(VK_RETURN) & 1)
			{
				features::aimbot = !features::aimbot;
			}
			Render::Text(95, 240, selectedcolor, XorStr("Aimbot ").c_str());
		}
		if (features::aimbot) { Render::Text(282, 240, ImColor(0, 255, 0, 255), XorStr("ON").c_str()); }
		else { Render::Text(278, 240, ImColor(255, 0, 0, 255), XorStr("OFF").c_str()); }

		Render::Text(95, 255, ImColor(255, 255, 255, 255), XorStr("Smoothness ").c_str()); // smoothness
		if (features::max_options == 2)
		{
			if (GetAsyncKeyState(VK_PRIOR) & 1)
			{
				if (features::smoothness < 49.99)
				{
					features::smoothness += 0.1;
				}
			}
			if (GetAsyncKeyState(VK_NEXT) & 1)
			{
				if (features::smoothness > 1.01)
				{
					features::smoothness -= 0.1;
				}
			}
			Render::Text(95, 255, selectedcolor, XorStr("Smoothness ").c_str());
		}
		char smoothbuf[256];
		sprintf(smoothbuf, XorStr("%.2f").c_str(), features::smoothness);
		if (features::smoothness < 10) { if (features::max_options == 2) Render::Text(270, 255, ImColor(220, 220, 0, 255), smoothbuf); else Render::Text(270, 255, ImColor(125, 125, 0, 255), smoothbuf); }
		else if (features::smoothness > 9.99) {
			if (features::max_options == 2) Render::Text(263, 255, ImColor(220, 220, 0, 255), smoothbuf); else Render::Text(263, 255, ImColor(125, 125, 0, 255), smoothbuf);
		}

		Render::Text(95, 270, ImColor(255, 255, 255, 255), XorStr("FOV ").c_str()); // fov
		if (features::max_options == 3)
		{
			if (GetAsyncKeyState(VK_PRIOR) & 1)
			{
				if (features::fov < 250)
				{
					features::fov += 1;
				}
			}
			if (GetAsyncKeyState(VK_NEXT) & 1)
			{
				if (features::fov > 10)
				{
					features::fov -= 1;
				}
			}
			Render::Text(95, 270, selectedcolor, XorStr("FOV ").c_str());
		}
		char fovbuf[256];
		sprintf(fovbuf, XorStr("%.2f").c_str(), features::fov);
		if (features::fov > 99) { if (features::max_options == 3) Render::Text(256, 270, ImColor(220, 220, 0, 255), fovbuf); else Render::Text(256, 270, ImColor(125, 125, 0, 255), fovbuf); }
		else if (features::fov < 100) { if (features::max_options == 3) Render::Text(263, 270, ImColor(220, 220, 0, 255), fovbuf); else Render::Text(263, 270, ImColor(125, 125, 0, 255), fovbuf); }

		Render::Text(95, 285, ImColor(255, 255, 255, 255), XorStr("Aimbot Key ").c_str()); // aim key
		if (features::max_options == 4)
		{
			if (GetAsyncKeyState(VK_PRIOR) & 1)
			{
				if (features::aimkeypos > 0)
				{
					features::aimkeypos -= 1;
				}
			}
			if (GetAsyncKeyState(VK_NEXT) & 1)
			{
				if (features::aimkeypos < 3)
				{
					features::aimkeypos += 1;
				}
			}
			Render::Text(95, 285, selectedcolor, XorStr("Aimbot Key ").c_str());
		}
		if (features::aimkeypos == 0 || features::aimkeypos == 1 || features::aimkeypos == 2) { if (features::max_options == 4) Render::Text(276, 285, ImColor(220, 220, 0, 255), features::aimkeystr.c_str()); else Render::Text(276, 285, ImColor(125, 125, 0, 255), features::aimkeystr.c_str()); }
		else if (features::aimkeypos == 3) { if (features::max_options == 4) Render::Text(269, 285, ImColor(220, 220, 0, 255), features::aimkeystr.c_str()); else Render::Text(269, 285, ImColor(125, 125, 0, 255), features::aimkeystr.c_str()); }

		Render::Text(95, 300, ImColor(255, 255, 255, 255), XorStr("Aim Bone ").c_str()); // aim bone
		if (features::max_options == 5)
		{
			if (GetAsyncKeyState(VK_PRIOR) & 1)
			{
				if (features::hitboxpos > 0)
				{
					features::hitboxpos -= 1;
				}
			}
			if (GetAsyncKeyState(VK_NEXT) & 1)
			{
				if (features::hitboxpos < 3)
				{
					features::hitboxpos += 1;
				}
			}
			Render::Text(95, 300, selectedcolor, XorStr("Aim Bone ").c_str());
		}
		if (features::hitboxpos == 0 || features::hitboxpos == 1 || features::hitboxpos == 3) { if (features::max_options == 5) Render::Text(269, 300, ImColor(220, 220, 0, 255), features::hitboxposstr.c_str()); else Render::Text(269, 300, ImColor(125, 125, 0, 255), features::hitboxposstr.c_str()); }
		else if (features::hitboxpos == 2) { if (features::max_options == 5) Render::Text(262, 300, ImColor(220, 220, 0, 255), features::hitboxposstr.c_str()); else Render::Text(262, 300, ImColor(125, 125, 0, 255), features::hitboxposstr.c_str()); }



		if (features::max_options > 5 && features::max_options < 11) { Render::Text(90, 345, ImColor(220, 220, 0, 255), XorStr("VISUALS ").c_str()); }
		else { Render::Text(90, 345, ImColor(125, 125, 0, 255), XorStr("VISUALS ").c_str()); } //visuals category

		Render::Text(95, 360, ImColor(255, 255, 255, 255), XorStr("Box ESP ").c_str()); //box feature
		if (features::max_options == 6)
		{
			if (GetAsyncKeyState(VK_RETURN) & 1)
			{
				features::boxes = !features::boxes;
			}
			Render::Text(95, 360, selectedcolor, XorStr("Box ESP ").c_str());
		}
		if (features::boxes) { Render::Text(282, 360, ImColor(0, 255, 0, 255), XorStr("ON").c_str()); }
		else { Render::Text(278, 360, ImColor(255, 0, 0, 255), XorStr("OFF").c_str()); }

		Render::Text(95, 375, ImColor(255, 255, 255, 255), XorStr("Distance ESP ").c_str()); //distance feature
		if (features::max_options == 7)
		{
			if (GetAsyncKeyState(VK_RETURN) & 1)
			{
				features::distance = !features::distance;
			}
			Render::Text(95, 375, selectedcolor, XorStr("Distance ESP ").c_str());
		}
		if (features::distance) { Render::Text(282, 375, ImColor(0, 255, 0, 255), XorStr("ON").c_str()); }
		else { Render::Text(278, 375, ImColor(255, 0, 0, 255), XorStr("OFF").c_str()); }

		Render::Text(95, 390, ImColor(255, 255, 255, 255), XorStr("Line ESP ").c_str()); //line esp feature
		if (features::max_options == 8)
		{
			if (GetAsyncKeyState(VK_RETURN) & 1)
			{
				features::lines = !features::lines;
			}
			Render::Text(95, 390, selectedcolor, XorStr("Line ESP ").c_str());
		}
		if (features::lines) { Render::Text(282, 390, ImColor(0, 255, 0, 255), XorStr("ON").c_str()); }
		else { Render::Text(278, 390, ImColor(255, 0, 0, 255), XorStr("OFF").c_str()); }

		Render::Text(95, 405, ImColor(255, 255, 255, 255), XorStr("Cercle FOV ").c_str()); //fov circle feature
		if (features::max_options == 9)
		{
			if (GetAsyncKeyState(VK_RETURN) & 1)
			{
				features::fovcirclee = !features::fovcirclee;
			}
			Render::Text(95, 405, selectedcolor, XorStr("Cercle FOV ").c_str());
		}
		if (features::fovcirclee) { Render::Text(282, 405, ImColor(0, 255, 0, 255), XorStr("ON").c_str()); }
		else { Render::Text(278, 405, ImColor(255, 0, 0, 255), XorStr("OFF").c_str()); }

		Render::Text(95, 420, ImColor(255, 255, 255, 255), XorStr("Crosshair ").c_str()); //fov circle feature
		if (features::max_options == 10)
		{
			if (GetAsyncKeyState(VK_RETURN) & 1)
			{
				features::crosshair = !features::crosshair;
			}
			Render::Text(95, 420, selectedcolor, XorStr("Crosshair ").c_str());
		}
		if (features::crosshair) { Render::Text(282, 420, ImColor(0, 255, 0, 255), XorStr("ON").c_str()); }
		else { Render::Text(278, 420, ImColor(255, 0, 0, 255), XorStr("OFF").c_str()); }



		if (features::max_options > 10 && features::max_options < 16) { Render::Text(90, 465, ImColor(220, 220, 0, 255), XorStr("MISC ").c_str()); }
		else { Render::Text(90, 465, ImColor(125, 125, 0, 255), XorStr("MISC").c_str()); } //misc category


		Render::Text(95, 480, ImColor(255, 255, 255, 255), XorStr("Line Pos (on player) ").c_str()); // aim key
		if (features::max_options == 11)
		{
			if (GetAsyncKeyState(VK_PRIOR) & 1)
			{
				if (features::lineesppos > 0)
				{
					features::lineesppos -= 1;
				}
			}
			if (GetAsyncKeyState(VK_NEXT) & 1)
			{
				if (features::lineesppos < 2)
				{
					features::lineesppos += 1;
				}
			}
			Render::Text(95, 480, selectedcolor, XorStr("Line Pos (on player) ").c_str());
		}
		if (features::lineesppos == 1 || features::lineesppos == 0) { if (features::max_options == 11) Render::Text(278, 480, ImColor(220, 220, 0, 255), features::lineespposstr.c_str()); else Render::Text(278, 480, ImColor(125, 125, 0, 255), features::lineespposstr.c_str()); }
		else if (features::lineesppos == 2) { if (features::max_options == 11) Render::Text(257, 480, ImColor(220, 220, 0, 255), features::lineespposstr.c_str()); else Render::Text(257, 480, ImColor(125, 125, 0, 255), features::lineespposstr.c_str()); }

		Render::Text(95, 495, ImColor(255, 255, 255, 255), XorStr("Instant Revive ").c_str()); // insta revive feature
		if (features::max_options == 12)
		{
			if (GetAsyncKeyState(VK_RETURN) & 1)
			{
				features::instarevive = !features::instarevive;
			}
			Render::Text(95, 495, selectedcolor, XorStr("Instant Revive ").c_str());
		}
		if (features::instarevive) { Render::Text(282, 495, ImColor(0, 255, 0, 255), XorStr("ON").c_str()); }
		else { Render::Text(278, 495, ImColor(255, 0, 0, 255), XorStr("OFF").c_str()); }

		Render::Text(95, 510, ImColor(255, 255, 255, 255), XorStr("Aim While Jumping ").c_str()); // insta revive feature
		if (features::max_options == 13)
		{
			if (GetAsyncKeyState(VK_RETURN) & 1)
			{
				features::ads = !features::ads;
			}
			Render::Text(95, 510, selectedcolor, XorStr("Aim While Jumping ").c_str());
		}
		if (features::ads) { Render::Text(282, 510, ImColor(0, 255, 0, 255), XorStr("ON").c_str()); }
		else { Render::Text(278, 510, ImColor(255, 0, 0, 255), XorStr("OFF").c_str()); }

		Render::Text(95, 525, ImColor(255, 255, 255, 255), XorStr("Player Fly ").c_str()); // insta revive feature
		if (features::max_options == 14)
		{
			if (GetAsyncKeyState(VK_RETURN) & 1)
			{
				features::pfly = !features::pfly;
			}
			Render::Text(95, 525, selectedcolor, XorStr("Player Fly ").c_str());
		}
		if (features::pfly) { Render::Text(282, 525, ImColor(0, 255, 0, 255), XorStr("ON").c_str()); }
		else { Render::Text(278, 525, ImColor(255, 0, 0, 255), XorStr("OFF").c_str()); }

		Render::Text(95, 540, ImColor(255, 255, 255, 255), XorStr("Big Players ").c_str()); // insta revive feature
		if (features::max_options == 15)
		{
			if (GetAsyncKeyState(VK_RETURN) & 1)
			{
				features::bigplayers = !features::bigplayers;
			}
			Render::Text(95, 540, selectedcolor, XorStr("Big Players ").c_str());
		}
		if (features::bigplayers) { Render::Text(282, 540, ImColor(0, 255, 0, 255), XorStr("ON").c_str()); }
		else { Render::Text(278, 540, ImColor(255, 0, 0, 255), XorStr("OFF").c_str()); }
		ImGui::PopFont();

		//Render::Text(95, 555, ImColor(255, 255, 255, 255), XorStr("FOV Changer ").c_str()); // insta revive feature
		//if (features::max_options == 16)
		//{
		//	if (GetAsyncKeyState(VK_RETURN) & 1)
		//	{
		//		features::fovchanger = !features::fovchanger;
		//	}
		//	Render::Text(95, 555, selectedcolor, XorStr("FOV Changer ").c_str());
		//}
		//if (features::fovchanger) { Render::Text(282, 555, ImColor(0, 255, 0, 255), XorStr("ON").c_str()); }
		//else { Render::Text(278, 555, ImColor(255, 0, 0, 255), XorStr("OFF").c_str()); }

		//Render::Text(95, 570, ImColor(255, 255, 255, 255), XorStr("FOVC [PGUP/PGDOWN] ").c_str()); // fov
		//if (features::max_options == 17)
		//{
		//	if (GetAsyncKeyState(VK_PRIOR) & 1)
		//	{
		//		if (features::fovvalue < 200)
		//		{
		//			features::fovvalue += 1;
		//		}
		//	}
		//	if (GetAsyncKeyState(VK_NEXT) & 1)
		//	{
		//		if (features::fovvalue > 70)
		//		{
		//			features::fovvalue -= 1;
		//		}
		//	}
		//	Render::Text(95, 570, selectedcolor, XorStr("FOVC [PGUP/PGDOWN] ").c_str());
		//}
		//char fovcbuf[256];
		//sprintf(fovcbuf, XorStr("%.2f").c_str(), features::fovvalue);
		//if (features::fovvalue > 99) { Render::Text(256, 570, ImColor(255, 142, 0, 255), fovcbuf); }
		//else if (features::fovvalue < 100) { Render::Text(263, 570, ImColor(255, 142, 0, 255), fovcbuf); }

		//if (ImGui::Begin(std::string(std::string(XorStr("renewal (things never end) | 01/15/2022"))).c_str(), 0, ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_::ImGuiWindowFlags_NoResize)) {
		//	ImGui::SetNextWindowSize(ImVec2({ 290, 650 }));

		//	ImGui::Dummy(ImVec2(2, 0));
		//	ImGui::SameLine();
		//	if (ImGui::Button(XorStr("aim").c_str(), ImVec2(80, 25)))
		//	{
		//		tabb = 1;
		//	}
		//	ImGui::SameLine();
		//	if (ImGui::Button(XorStr("visuals").c_str(), ImVec2(80, 25)))
		//	{
		//		tabb = 2;
		//	}
		//	ImGui::SameLine();
		//	if (ImGui::Button(XorStr("misc").c_str(), ImVec2(80, 25)))
		//	{
		//		tabb = 3;
		//	}
		//	if (tabb == 1) {
		//		ImGui::SetWindowSize(ImVec2(290, 225));
		//		ImGui::Text("");
		//		ImGui::Dummy(ImVec2(5, 30));
		//		ImGui::SameLine();
		//		ImGui::Checkbox(XorStr("aimbot (F8)").c_str(), &features::aimbot);
		//		if (features::aimbot)
		//		{
		//			ImGui::SetWindowSize(ImVec2(290, 250));
		//			ImGui::Dummy(ImVec2(5, 30));
		//			ImGui::SameLine();
		//			ImGui::Checkbox(XorStr("random aim (beta)").c_str(), &features::randomaim);
		//			if (features::randomaim)
		//			{
		//				ImGui::SetWindowSize(ImVec2(290, 290));
		//				ImGui::Dummy(ImVec2(5, 30));
		//				ImGui::SameLine();
		//				ImGui::SliderFloat(XorStr("speed(ms)").c_str(), &features::randomspeed, 50.0f, 3000.0f, XorStr("%.2f").c_str());
		//			}
		//		}
		//		ImGui::Dummy(ImVec2(5, 30));
		//		ImGui::SameLine();
		//		ImGui::SliderFloat(XorStr("smooth").c_str(), &features::smoothness, 1.0f, 20.0f, XorStr("%.2f").c_str());
		//		ImGui::Dummy(ImVec2(5, 30));
		//		ImGui::SameLine();
		//		ImGui::SliderFloat(XorStr("fov").c_str(), &features::fov, 1, 250, XorStr("%.2f").c_str());
		//		ImGui::Dummy(ImVec2(5, 30));
		//		ImGui::SameLine();
		//		HotkeyButton(features::aimkey, ChangeKey, keystatus); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 255, 255), XorStr("aim key").c_str());
		//	}
		//	else if (tabb == 2) {
		//		ImGui::SetWindowSize(ImVec2(290, 300));
		//		ImGui::Text("");
		//		ImGui::Dummy(ImVec2(5, 30));
		//		ImGui::SameLine();
		//		ImGui::Checkbox(XorStr("##ESP Box").c_str(), &features::boxes);  ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 255, 255), XorStr("box").c_str());
		//		if (features::boxes && features::crosshair && features::lines)
		//		{
		//			ImGui::SetWindowSize(ImVec2(290, 410));
		//			ImGui::Dummy(ImVec2(20, 30));
		//			ImGui::SameLine();
		//			ImGui::Checkbox(XorStr("##ESP Dostances").c_str(), &features::distance);  ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 255, 255), XorStr("distance").c_str());
		//			/*ImGui::Dummy(ImVec2(20, 30));
		//			ImGui::SameLine();
		//			ImGui::SliderFloat(XorStr("width").c_str(), &features::boxwidthlol, 0.300f, 0.800f, XorStr("%.2f").c_str());
		//			ImGui::Dummy(ImVec2(20, 30));
		//			ImGui::SameLine();
		//			ImGui::SliderFloat(XorStr("thick").c_str(), &features::boxthick, 1.0f, 5.0f, XorStr("%.2f").c_str());*/
		//		}
		//		else if (features::boxes && features::crosshair)
		//		{
		//			ImGui::SetWindowSize(ImVec2(290, 370));
		//			ImGui::Dummy(ImVec2(20, 30));
		//			ImGui::SameLine();
		//			ImGui::Checkbox(XorStr("##ESP Dostances").c_str(), &features::distance);  ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 255, 255), XorStr("distance").c_str());
		//			/*ImGui::Dummy(ImVec2(20, 30));
		//			ImGui::SameLine();
		//			ImGui::SliderFloat(XorStr("width").c_str(), &features::boxwidthlol, 0.300f, 0.800f, XorStr("%.2f").c_str());
		//			ImGui::Dummy(ImVec2(20, 30));
		//			ImGui::SameLine();
		//			ImGui::SliderFloat(XorStr("thick").c_str(), &features::boxthick, 1.0f, 5.0f, XorStr("%.2f").c_str());*/
		//		}
		//		else if (features::boxes && features::lines)
		//		{
		//			ImGui::SetWindowSize(ImVec2(290, 370));
		//			ImGui::Dummy(ImVec2(20, 30));
		//			ImGui::SameLine();
		//			ImGui::Checkbox(XorStr("##ESP Dostances").c_str(), &features::distance);  ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 255, 255), XorStr("distance").c_str());
		//			/*ImGui::Dummy(ImVec2(20, 30));
		//			ImGui::SameLine();
		//			ImGui::SliderFloat(XorStr("width").c_str(), &features::boxwidthlol, 0.300f, 0.800f, XorStr("%.2f").c_str());
		//			ImGui::Dummy(ImVec2(20, 30));
		//			ImGui::SameLine();
		//			ImGui::SliderFloat(XorStr("thick").c_str(), &features::boxthick, 1.0f, 5.0f, XorStr("%.2f").c_str());*/
		//		}
		//		else if (features::boxes)
		//		{
		//			ImGui::SetWindowSize(ImVec2(290, 345));
		//			ImGui::Dummy(ImVec2(20, 30));
		//			ImGui::SameLine();
		//			ImGui::Checkbox(XorStr("##ESP Dostances").c_str(), &features::distance);  ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 255, 255), XorStr("distance").c_str());
		//			/*ImGui::Dummy(ImVec2(20, 30));
		//			ImGui::SameLine();
		//			ImGui::SliderFloat(XorStr("width").c_str(), &features::boxwidthlol, 0.300f, 0.800f, XorStr("%.2f").c_str());
		//			ImGui::Dummy(ImVec2(20, 30));
		//			ImGui::SameLine();
		//			ImGui::SliderFloat(XorStr("thick").c_str(), &features::boxthick, 1.0f, 5.0f, XorStr("%.2f").c_str());*/
		//		}
		//		ImGui::Dummy(ImVec2(5, 30));
		//		ImGui::SameLine();
		//		ImGui::Checkbox(XorStr("##circle").c_str(), &features::fovcirclee);  ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 255, 255), XorStr("fov circle").c_str());
		//		ImGui::Dummy(ImVec2(5, 30));
		//		ImGui::SameLine();
		//		ImGui::Checkbox(XorStr("##crosshair").c_str(), &features::crosshair);  ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 255, 255), XorStr("crosshair").c_str());
		//		if (features::crosshair && features::boxes && features::lines)
		//		{
		//			ImGui::SetWindowSize(ImVec2(290, 410));
		//			ImGui::Dummy(ImVec2(20, 30));
		//			ImGui::SameLine();
		//			ImGui::SliderFloat(XorStr("cthick").c_str(), &features::crossthick, 1.0f, 3.0f, XorStr("%.2f").c_str());
		//		}
		//		else if (features::crosshair && features::boxes) {
		//			ImGui::SetWindowSize(ImVec2(290, 370));
		//			ImGui::Dummy(ImVec2(20, 30));
		//			ImGui::SameLine();
		//			ImGui::SliderFloat(XorStr("cthick").c_str(), &features::crossthick, 1.0f, 3.0f, XorStr("%.2f").c_str());
		//		}
		//		else if (features::crosshair && features::lines)
		//		{
		//			ImGui::SetWindowSize(ImVec2(290, 380));
		//			ImGui::Dummy(ImVec2(20, 30));
		//			ImGui::SameLine();
		//			ImGui::SliderFloat(XorStr("cthick").c_str(), &features::crossthick, 1.0f, 3.0f, XorStr("%.2f").c_str());
		//		}
		//		else if (features::crosshair)
		//		{
		//			ImGui::SetWindowSize(ImVec2(290, 320));
		//			ImGui::Dummy(ImVec2(20, 30));
		//			ImGui::SameLine();
		//			ImGui::SliderFloat(XorStr("cthick").c_str(), &features::crossthick, 1.0f, 3.0f, XorStr("%.2f").c_str());
		//		}
		//		ImGui::Dummy(ImVec2(5, 30));
		//		ImGui::SameLine();
		//		ImGui::Checkbox(XorStr("##ESP Lignes").c_str(), &features::lines); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 255, 255), XorStr("line").c_str());
		//		if (features::lines && features::boxes && features::crosshair)
		//		{
		//			ImGui::SetWindowSize(ImVec2(290, 410));
		//			ImGui::Dummy(ImVec2(20, 30));
		//			ImGui::SameLine();
		//			ImGui::Combo(XorStr("##linesespppp").c_str(), &features::lineesppos, lineespshit, sizeof(lineespshit) / sizeof(*lineespshit)); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 255, 255), XorStr("pos").c_str());
		//		}
		//		else if (features::lines && features::boxes)
		//		{
		//			ImGui::SetWindowSize(ImVec2(290, 370));
		//			ImGui::Dummy(ImVec2(20, 30));
		//			ImGui::SameLine();
		//			ImGui::Combo(XorStr("##linesespppp").c_str(), &features::lineesppos, lineespshit, sizeof(lineespshit) / sizeof(*lineespshit)); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 255, 255), XorStr("pos").c_str());
		//		}
		//		else if (features::lines && features::crosshair)
		//		{
		//			ImGui::SetWindowSize(ImVec2(290, 350));
		//			ImGui::Dummy(ImVec2(20, 30));
		//			ImGui::SameLine();
		//			ImGui::Combo(XorStr("##linesespppp").c_str(), &features::lineesppos, lineespshit, sizeof(lineespshit) / sizeof(*lineespshit)); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 255, 255), XorStr("pos").c_str());
		//		}
		//		else if (features::lines)
		//		{
		//			ImGui::SetWindowSize(ImVec2(290, 325));
		//			ImGui::Dummy(ImVec2(20, 30));
		//			ImGui::SameLine();
		//			ImGui::Combo(XorStr("##linesespppp").c_str(), &features::lineesppos, lineespshit, sizeof(lineespshit) / sizeof(*lineespshit)); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 255, 255), XorStr("pos").c_str());
		//		}
		//		ImGui::Dummy(ImVec2(5, 30));
		//		ImGui::SameLine();
		//		ImGui::Checkbox(XorStr("##chestesp").c_str(), &features::chest); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 255, 255), XorStr("chest esp").c_str());
		//		ImGui::Dummy(ImVec2(5, 30));
		//		ImGui::SameLine();
		//		ImGui::Checkbox(XorStr("##ammoboxesp").c_str(), &features::ammo); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 255, 255), XorStr("ammobox esp").c_str());
		//	}
		//	if (tabb == 3)
		//	{
		//		ImGui::SetWindowSize(ImVec2(290, 815));
		//		ImGui::Text("");
		//		ImGui::Dummy(ImVec2(0, 50));
		//		ImGui::SameLine();
		//		ImGui::PushItemWidth(120);
		//		ImGui::ColorPicker4(XorStr("boxes color").c_str(), features::boxcolor);
		//		ImGui::Dummy(ImVec2(0, 50));
		//		ImGui::SameLine();
		//		ImGui::PushItemWidth(120);
		//		ImGui::ColorPicker4(XorStr("lines color").c_str(), features::linescolor);
		//		ImGui::Dummy(ImVec2(0, 50));
		//		ImGui::SameLine();
		//		ImGui::PushItemWidth(120);
		//		ImGui::ColorPicker4(XorStr("cross color").c_str(), features::crosshaircolor);
		//		ImGui::Dummy(ImVec2(0, 50));
		//		ImGui::SameLine();
		//		ImGui::PushItemWidth(120);
		//		ImGui::ColorPicker4(XorStr("fov color").c_str(), features::fovcolor);
		//		ImGui::Text("");
		//		ImGui::Dummy(ImVec2(0, 1));
		//		ImGui::SameLine();
		//		ImGui::Checkbox(XorStr("##instarevive").c_str(), &features::instarevive);  ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 255, 255), XorStr("insta revive").c_str());
		//		ImGui::Dummy(ImVec2(0, 10));
		//		ImGui::SameLine();
		//		ImGui::Checkbox(XorStr("##ads").c_str(), &features::ads);  ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 255, 255), XorStr("ads (aim down sights)").c_str());
		//	}
		//	ImGui::End();
		//}

	}


	ImGui::EndFrame();
	D3dDevice->SetRenderState(D3DRS_ZENABLE, false);
	D3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	D3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
	D3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);


	if (D3dDevice->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		D3dDevice->EndScene();
	}
	HRESULT result = D3dDevice->Present(NULL, NULL, NULL, NULL);

	if (result == D3DERR_DEVICELOST && D3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
	{
		ImGui_ImplDX9_InvalidateDeviceObjects();
		D3dDevice->Reset(&d3dpp);
		ImGui_ImplDX9_CreateDeviceObjects();
	}
}
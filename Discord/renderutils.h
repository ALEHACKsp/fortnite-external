#pragma once

namespace Render
{
	struct color_keys
	{
		ImVec4 value;
		color_keys() { value.x = value.y = value.z = value.y = 0.0f; }
		color_keys(int r, int g, int b, int a = 255) { float sc = 1.0f / 255.0f; value.x = (float)r * sc; value.y = (float)g * sc; value.z = (float)b * sc; value.w = (float)a * sc; }
		color_keys(float r, float g, float b, float a = 1.0f) { value.x = r; value.y = g; value.z = b; value.w = a; }
		color_keys(const ImVec4& col) { value = col; }
	};
	color_keys* col_keys;

	struct vec_2
	{
		int x, y;
	};

	struct Colors
	{
		ImColor red = { 255, 0, 0, 255 };
		ImColor green = { 0, 255, 0, 255 };
		ImColor blue = { 0, 136, 255, 255 };
		ImColor aqua_blue = { 0, 255, 255, 255 };
		ImColor cyan = { 0, 210, 210, 255 };
		ImColor royal_purple = { 102, 0, 255, 255 };
		ImColor dark_pink = { 255, 0, 174, 255 };
		ImColor black = { 0, 0, 0, 255 };
		ImColor white = { 255, 255, 255, 255 };
		ImColor purple = { 255, 0, 255, 255 };
		ImColor yellow = { 255, 255, 0, 255 };
		ImColor orange = { 255, 140, 0, 255 };
		ImColor gold = { 234, 255, 0, 255 };
		ImColor royal_blue = { 0, 30, 255, 255 };
		ImColor dark_red = { 150, 5, 5, 255 };
		ImColor dark_green = { 5, 150, 5, 255 };
		ImColor dark_blue = { 100, 100, 255, 255 };
		ImColor navy_blue = { 0, 73, 168, 255 };
		ImColor light_gray = { 200, 200, 200, 255 };
		ImColor dark_gray = { 150, 150, 150, 255 };
	};
	Colors color;

	void Text(int posx, int posy, ImColor clr, const char* text)
	{
		ImGui::GetBackgroundDrawList()->AddText(ImVec2(posx, posy), ImColor(clr), text);
	}

	void OutlinedText(int posx, int posy, ImColor clr, const char* text)
	{
		ImGui::GetBackgroundDrawList()->AddText(ImVec2(posx + 1, posy + 1), ImColor(color.black), text);
		ImGui::GetBackgroundDrawList()->AddText(ImVec2(posx - 1, posy - 1), ImColor(color.black), text);
		ImGui::GetBackgroundDrawList()->AddText(ImVec2(posx + 1, posy + 1), ImColor(color.black), text);
		ImGui::GetBackgroundDrawList()->AddText(ImVec2(posx - 1, posy - 1), ImColor(color.black), text);
		ImGui::GetBackgroundDrawList()->AddText(ImVec2(posx, posy), ImColor(clr), text);
	}

	void ShadowText(int posx, int posy, ImColor clr, const char* text)
	{
		ImGui::GetBackgroundDrawList()->AddText(ImVec2(posx + 1, posy + 2), ImColor(0, 0, 0, 200), text);
		ImGui::GetBackgroundDrawList()->AddText(ImVec2(posx + 1, posy + 2), ImColor(0, 0, 0, 200), text);
		ImGui::GetBackgroundDrawList()->AddText(ImVec2(posx, posy), ImColor(clr), text);
	}

	void Rect(int x, int y, int w, int h, ImColor color, int thickness)
	{
		ImGui::GetBackgroundDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(ImVec4(color)), 0, 0, thickness);
	}

	void RectFilledGradient(int x, int y, int w, int h, ImColor color)
	{
		ImGui::GetBackgroundDrawList()->AddRectFilledMultiColor(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(ImVec4(color)), ImGui::ColorConvertFloat4ToU32(ImVec4(color)), 0, 0);
	}

	void RectFilled(int x, int y, int w, int h, ImColor color)
	{
		ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(ImVec4(color)), 0, 0);
	}

	void Tab(const char* v, float size_x, float size_y, static int tab_name, int tab_next)
	{
		if (ImGui::Button(v, ImVec2{ size_x, size_y })) tab_name = tab_next;
	}
}

void fullbox(int x, int y, int w, int h, ImU32& color, int thickness)
{
	ImGui::GetBackgroundDrawList()->AddRect(ImVec2(x, y), ImVec2(w, h), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 0, 0, 3);
	ImGui::GetBackgroundDrawList()->AddRect(ImVec2(x, y), ImVec2(w, h), ImGui::GetColorU32(color), 0, 0, thickness);
}
float outlinedtext(ImFont* pFont, const std::string& text, const ImVec2& pos, float size, ImU32 color, bool center)
{
	std::stringstream stream(text);
	std::string line;

	float y = 0.0f;
	int i = 0;

	while (std::getline(stream, line))
	{
		ImVec2 textSize = pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, line.c_str());

		if (center)
		{
			ImGui::GetBackgroundDrawList()->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) + 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());

			ImGui::GetBackgroundDrawList()->AddText(pFont, size, ImVec2(pos.x - textSize.x / 2.0f, pos.y + textSize.y * i), ImGui::GetColorU32(color), line.c_str());
		}
		else
		{
			ImGui::GetBackgroundDrawList()->AddText(pFont, size, ImVec2((pos.x) + 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());

			ImGui::GetBackgroundDrawList()->AddText(pFont, size, ImVec2(pos.x, pos.y + textSize.y * i), ImGui::GetColorU32(color), line.c_str());
		}

		y = pos.y + textSize.y * (i + 1);
		i++;
	}
	return y;
}

void lineesp(int x1, int y1, int x2, int y2, ImU32 color, float thickness)
{
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), color, thickness);
}

void fovcircle(int x, int y, int radius, ImU32 color, int segments, float thickness)
{
	ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(x, y), radius, color, segments, thickness);
}
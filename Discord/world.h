#pragma once

FTransform boneid(DWORD_PTR mesh, int index)
{
	DWORD_PTR bonearray;
	bonearray = ReadPointer<DWORD_PTR>(mesh + 0x590);
	if (bonearray == NULL)
	{
		bonearray = ReadPointer<DWORD_PTR>(mesh + 0x590 + 0x10);
	}

	return ReadPointer<FTransform>(bonearray + (index * 0x60));

}
Vector3 bonerota(DWORD_PTR mesh, int id)
{
	FTransform bone = boneid(mesh, id);
	FTransform ComponentToWorld = ReadPointer<FTransform>(mesh + 0x240);
	D3DMATRIX Matrix;

	Matrix = matrixx(bone.ToMatrixWithScale(), ComponentToWorld.ToMatrixWithScale());

	return Vector3(Matrix._41, Matrix._42, Matrix._43);
}


static D3DMATRIX matrix(Vector3 rot, Vector3 origin = Vector3(0, 0, 0)) {
	double radPitch = (rot.x * double(M_PI) / 180.f);
	double radYaw = (rot.y * double(M_PI) / 180.f);
	double radRoll = (rot.z * double(M_PI) / 180.f);

	double SP = sinf(radPitch);
	double CP = cosf(radPitch);
	double SY = sinf(radYaw);
	double CY = cosf(radYaw);
	double SR = sinf(radRoll);
	double CR = cosf(radRoll);

	D3DMATRIX matrix;
	matrix.m[0][0] = CP * CY;
	matrix.m[0][1] = CP * SY;
	matrix.m[0][2] = SP;
	matrix.m[0][3] = 0.f;

	matrix.m[1][0] = SR * SP * CY - CR * SY;
	matrix.m[1][1] = SR * SP * SY + CR * CY;
	matrix.m[1][2] = -SR * CP;
	matrix.m[1][3] = 0.f;

	matrix.m[2][0] = -(CR * SP * CY + SR * SY);
	matrix.m[2][1] = CY * SR - CR * SP * SY;
	matrix.m[2][2] = CR * CP;
	matrix.m[2][3] = 0.f;

	matrix.m[3][0] = origin.x;
	matrix.m[3][1] = origin.y;
	matrix.m[3][2] = origin.z;
	matrix.m[3][3] = 1.f;

	return matrix;
}
Vector3 w2s(Vector3 WorldLocation)
{
	Vector3 Screenlocation = Vector3(0, 0, 0);
	Vector3 Camera;

	auto chain69 = ReadPointer<uintptr_t>(offname::localactor + 0xC8);
	uint64_t chain699 = ReadPointer<uintptr_t>(chain69 + 8);

	Camera.x = ReadPointer<double>(chain699 + 0xAE0);
	Camera.y = ReadPointer<double>(offname::rootcomp + 0x148);

	double test = asin(Camera.x);
	double degrees = test * (180.0 / M_PI);

	Camera.x = degrees;
	if (Camera.y < 0)
		Camera.y = 360 + Camera.y;

	D3DMATRIX tempMatrix = matrix(Camera);
	Vector3 vAxisX, vAxisY, vAxisZ;
	vAxisX = Vector3(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
	vAxisY = Vector3(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
	vAxisZ = Vector3(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

	uint64_t chain = ReadPointer<uint64_t>(offname::localactor + 0x70);
	uint64_t chain1 = ReadPointer<uint64_t>(chain + 0x98);
	uint64_t chain2 = ReadPointer<uint64_t>(chain1 + 0x180);

	Vector3 vDelta = WorldLocation - ReadPointer<Vector3>(chain2 + 0x20);
	Vector3 vTransformed = Vector3(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

	if (vTransformed.z < 1.f)
		vTransformed.z = 1.f;

	double zoom = ReadPointer<double>(chain699 + 0x690);
	double FovAngle = 80.0f / (zoom / 1.19f);

	float ScreenCenterX = Width / 2;
	float ScreenCenterY = Height / 2;
	float ScreenCenterZ = Height / 2;

	Screenlocation.x = ScreenCenterX + vTransformed.x * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;
	Screenlocation.y = ScreenCenterY - vTransformed.y * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;
	Screenlocation.z = ScreenCenterZ - vTransformed.z * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;

	return Screenlocation;
}

void aimbot(float x, float y)
{
	float ScreenCenterX = (Width / 2);
	float ScreenCenterY = (Height / 2);
	int AimSpeed = features::smoothness;
	float TargetX = 0;
	float TargetY = 0;

	if (x != 0)
	{
		if (x > ScreenCenterX)
		{
			TargetX = -(ScreenCenterX - x);
			TargetX /= AimSpeed;
			if (TargetX + ScreenCenterX > ScreenCenterX * 2) TargetX = 0;
		}

		if (x < ScreenCenterX)
		{
			TargetX = x - ScreenCenterX;
			TargetX /= AimSpeed;
			if (TargetX + ScreenCenterX < 0) TargetX = 0;
		}
	}

	if (y != 0)
	{
		if (y > ScreenCenterY)
		{
			TargetY = -(ScreenCenterY - y);
			TargetY /= AimSpeed;
			if (TargetY + ScreenCenterY > ScreenCenterY * 2) TargetY = 0;
		}

		if (y < ScreenCenterY)
		{
			TargetY = y - ScreenCenterY;
			TargetY /= AimSpeed;
			if (TargetY + ScreenCenterY < 0) TargetY = 0;
		}
	}
	mouse_event(MOUSEEVENTF_MOVE, static_cast<DWORD>(TargetX), static_cast<DWORD>(TargetY), NULL, NULL);

	return;
}

Vector3 AimbotCorrection(float bulletVelocity, float bulletGravity, float targetDistance, Vector3 targetPosition, Vector3 targetVelocity) {
	Vector3 recalculated = targetPosition;
	float gravity = fabs(bulletGravity);
	float time = targetDistance / fabs(bulletVelocity);
	float bulletDrop = (gravity / 250) * time * time;
	recalculated.z += bulletDrop * 120;
	recalculated.x += time * (targetVelocity.x);
	recalculated.y += time * (targetVelocity.y);
	recalculated.z += time * (targetVelocity.z);
	return recalculated;
}

void targetbone(DWORD_PTR entity)
{
	uint64_t currentactormesh = ReadPointer<uint64_t>(entity + Offsets::External::MESH);
	auto rootHead = bonerota(currentactormesh, features::hitbox);

	Vector3 rootHeadOut = w2s(rootHead);
	if (rootHeadOut.y != 0 || rootHeadOut.y != 0)
	{
		aimbot(rootHeadOut.x, rootHeadOut.y);
	}
}

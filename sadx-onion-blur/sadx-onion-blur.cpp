#include "stdafx.h"

FunctionPointer(void, njAction_Queue, (NJS_ACTION *a1, float a2, QueuedModelFlagsB a3), 0x00405470);

void draw_onion(int anim_index, CharObj2* data2)
{
	njAction(data2->AnimationThing.AnimData[anim_index].Animation, data2->AnimationThing.Frame);

	float frame = data2->AnimationThing.Frame;
	const auto frame_count = static_cast<float>(data2->AnimationThing.AnimData[anim_index].Animation->motion->nbFrame);

	auto control_3d_orig = _nj_control_3d_flag_;

	_nj_control_3d_flag_ |= NJD_CONTROL_3D_CONSTANT_ATTR;

	BackupConstantAttr();
	AddConstantAttr(0, NJD_FLAG_USE_ALPHA);
	njColorBlendingMode(NJD_SOURCE_COLOR, NJD_COLOR_BLENDING_SRCALPHA);
	njColorBlendingMode(NJD_DESTINATION_COLOR, NJD_COLOR_BLENDING_ONE);
	Direct3D_SetZFunc(1);

	// TODO: configurable pass count

	float alpha = 0.75f;

	for (int i = 0; i < 2; ++i)
	{
		SetMaterialAndSpriteColor_Float(alpha, 1.0, 1.0, 1.0);
		alpha = alpha - 0.25f;

		frame -= 2.0f;
		if (frame < 0.0f)
		{
			frame = frame_count + frame;
		}

		njAction_Queue(data2->AnimationThing.AnimData[anim_index].Animation, frame,
		               QueuedModelFlagsB_EnableZWrite);
	}

	RestoreConstantAttr();
	_nj_control_3d_flag_ = control_3d_orig;
}


static const void* loc_494400 = reinterpret_cast<const void*>(0x494400);

void __cdecl sub_494400_o(int a1, CharObj2* a2)
{
	__asm
	{
		mov eax, a1
		mov esi, a2
		call loc_494400
	}
}


void __cdecl sub_494400_c(int anim_index, CharObj2* data2)
{
	if (MetalSonicFlag || anim_index != 13)
	{
		sub_494400_o(anim_index, data2);
		return;
	}

	draw_onion(anim_index, data2);
}

static void __declspec(naked) sub_494400_asm()
{
	__asm
	{
		push esi // a2
		push eax // a1

		call sub_494400_c

		pop eax // a1
		pop esi // a2
		retn
	}
}

extern "C"
{
	__declspec(dllexport) ModInfo SADXModInfo = { ModLoaderVer };

	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{
		// Sonic's motion blur
		WriteCall(reinterpret_cast<void*>(0x004947B7), &sub_494400_asm);
		WriteCall(reinterpret_cast<void*>(0x00494B00), &sub_494400_asm);
	}
}

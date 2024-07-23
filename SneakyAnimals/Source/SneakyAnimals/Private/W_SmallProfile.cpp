// Fill out your copyright notice in the Description page of Project Settings.


#include "W_SmallProfile.h"
#include "FL_General.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Image.h>

void UW_SmallProfile::Refresh_Widget()
{
	FUserProfileResult result = UFL_General::Get_UserProfile();
	if (result.success)
	{
		Username_Text->SetText(result.S_UserProfile.Username);
		ProfilePicture_Image->SetBrushFromTexture(result.S_UserProfile.User_Avatar);
	}
}

void UW_SmallProfile::NativeConstruct()
{
	Super::NativeConstruct();

	Refresh_Widget();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "W_Tooltip.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/SizeBox.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Image.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>

void UW_Tooltip::NativeConstruct()
{
	Setup();
}

void UW_Tooltip::Setup()
{
	Set_Image();
	Title_Text->SetText(Tooltip_Title);
	Tooltip_Description_Text->SetText(Tooltip_Description);
}

void UW_Tooltip::Set_Image()
{
	if (IsValid(Image))
	{
		Image_SizeBox->SetVisibility(ESlateVisibility::Visible);
		Tooltip_Image->SetBrushFromTexture(Image);
		
		// since we use 4k images we divide the Size X and Size Y by 2
		// 4k 이미지 쓰니까 2로 나눠준다.
		Image_SizeBox->SetWidthOverride(((float)(Image->GetSizeX() / 2)));
		Image_SizeBox->SetWidthOverride(((float)(Image->GetSizeY() / 2)));
	}
	
}

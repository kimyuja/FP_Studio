// Fill out your copyright notice in the Description page of Project Settings.


#include "W_LoadingScreen.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Image.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>

void UW_LoadingScreen::NativeConstruct()
{
	Super::NativeConstruct();

	Background_Image->SetBrushFromTexture(LoadingScreen_Image);
	Feedback_Text->SetText(LoadingScreen_FeedbackText);
}

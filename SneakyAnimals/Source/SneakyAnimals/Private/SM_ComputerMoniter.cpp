// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_ComputerMoniter.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Image.h>

void USM_ComputerMoniter::NativeConstruct()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void USM_ComputerMoniter::SetWarning()
{
	SetVisibility(ESlateVisibility::Visible);
	moniter1->SetBrushFromTexture(warningScreen);
	moniter2->SetBrushFromTexture(warningScreen);
	FTimerHandle blackScreenT;
	GetWorld()->GetTimerManager().SetTimer(blackScreenT, [&]()
	{
		SetVisibility(ESlateVisibility::Hidden);
	}, 1.0, false, 5.0);
}

void USM_ComputerMoniter::SetHelp()
{
	SetVisibility(ESlateVisibility::Visible);
	moniter1->SetBrushFromTexture(sosScreen);
	moniter2->SetBrushFromTexture(sosScreen);
	FTimerHandle whiteScreenT;
	GetWorld()->GetTimerManager().SetTimer(whiteScreenT, [&]()
		{
			SetVisibility(ESlateVisibility::Hidden);
		}, 1.0, false, 5.0);
}

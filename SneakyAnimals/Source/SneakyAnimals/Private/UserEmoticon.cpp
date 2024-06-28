// Fill out your copyright notice in the Description page of Project Settings.


#include "UserEmoticon.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Image.h>

void UUserEmoticon::NativeConstruct()
{
	Super::NativeConstruct();
	
	curEmoticon->SetVisibility(ESlateVisibility::Hidden);
	curEmoticon->SetRenderScale(FVector2D(0,0));
}

void UUserEmoticon::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

void UUserEmoticon::ShowEmoticon(int32 emoNum)
{
	switch (playerType)
	{
	case EPlayerType::COW:
		curEmoticon->SetBrushFromTexture(cowEmoticons[emoNum]);
		break;
	case EPlayerType::FROG:
		curEmoticon->SetBrushFromTexture(frogEmoticons[emoNum]);
		break;
	case EPlayerType::RABBIT:
		curEmoticon->SetBrushFromTexture(rabbitEmoticons[emoNum]);
		break;
	case EPlayerType::TURTLE:
		curEmoticon->SetBrushFromTexture(turtleEmoticons[emoNum]);
		break;
	default:
		break;
	}
	curEmoticon->SetVisibility(ESlateVisibility::Visible);
	PlayAnimation(EmoticonAnim);
	FTimerHandle closeT;
	GetWorld()->GetTimerManager().SetTimer(closeT, [&]()
	{
		curEmoticon->SetVisibility(ESlateVisibility::Hidden);
	}, 3.0, false, 0);
}

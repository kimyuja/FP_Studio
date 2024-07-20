// Fill out your copyright notice in the Description page of Project Settings.


#include "W_InGameUI.h"
#include "SAGameStateBase.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/ProgressBar.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Overlay.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetTextLibrary.h>

void UW_InGameUI::NativeConstruct()
{
	Super::NativeConstruct();

	gameState = Cast<ASAGameStateBase>(GetWorld()->GetGameState());

	Player1_DeathCount->SetText(FText::FromString(FString::FromInt(0)));
	Player2_DeathCount->SetText(FText::FromString(FString::FromInt(0)));
	Player3_DeathCount->SetText(FText::FromString(FString::FromInt(0)));
	Player4_DeathCount->SetText(FText::FromString(FString::FromInt(0)));
	Player1_TriggeredCount->SetText(FText::FromString(FString::FromInt(0)));
	Player2_TriggeredCount->SetText(FText::FromString(FString::FromInt(0)));
	Player3_TriggeredCount->SetText(FText::FromString(FString::FromInt(0)));
	Player4_TriggeredCount->SetText(FText::FromString(FString::FromInt(0)));

	DeathCount->SetVisibility(ESlateVisibility::Hidden);
}

void UW_InGameUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (gameState == nullptr)
	{
		return;
	}
	if (!bISClear)
	{
		SetStageTimer();
		gametime += InDeltaTime;
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Timer Working"));
	}
}

void UW_InGameUI::SetPlayerTD()
{
	int32 stage = gameState->stageNum;
	switch (stage)
	{
	case 1:
		Player1_DeathCount->SetText(FText::FromString(FString::FromInt(gameState->deathCount1[0])));
		Player2_DeathCount->SetText(FText::FromString(FString::FromInt(gameState->deathCount1[1])));
		Player3_DeathCount->SetText(FText::FromString(FString::FromInt(gameState->deathCount1[2])));
		Player4_DeathCount->SetText(FText::FromString(FString::FromInt(gameState->deathCount1[3])));
		break;
	case 2:
		Player1_DeathCount->SetText(FText::FromString(FString::FromInt(gameState->deathCount2[0])));
		Player2_DeathCount->SetText(FText::FromString(FString::FromInt(gameState->deathCount2[1])));
		Player3_DeathCount->SetText(FText::FromString(FString::FromInt(gameState->deathCount2[2])));
		Player4_DeathCount->SetText(FText::FromString(FString::FromInt(gameState->deathCount2[3])));
		break;
	case 3:
		Player1_DeathCount->SetText(FText::FromString(FString::FromInt(gameState->deathCount3[0])));
		Player2_DeathCount->SetText(FText::FromString(FString::FromInt(gameState->deathCount3[1])));
		Player3_DeathCount->SetText(FText::FromString(FString::FromInt(gameState->deathCount3[2])));
		Player4_DeathCount->SetText(FText::FromString(FString::FromInt(gameState->deathCount3[3])));
		break;
	case 4:
		Player1_DeathCount->SetText(FText::FromString(FString::FromInt(gameState->deathCount4[0])));
		Player2_DeathCount->SetText(FText::FromString(FString::FromInt(gameState->deathCount4[1])));
		Player3_DeathCount->SetText(FText::FromString(FString::FromInt(gameState->deathCount4[2])));
		Player4_DeathCount->SetText(FText::FromString(FString::FromInt(gameState->deathCount4[3])));
		break;
	default:
		break;
	}
	Player1_TriggeredCount->SetText(FText::FromString(FString::FromInt(gameState->activeCount[0])));
	Player2_TriggeredCount->SetText(FText::FromString(FString::FromInt(gameState->activeCount[1])));
	Player3_TriggeredCount->SetText(FText::FromString(FString::FromInt(gameState->activeCount[2])));
	Player4_TriggeredCount->SetText(FText::FromString(FString::FromInt(gameState->activeCount[3])));
}

void UW_InGameUI::SetStageTimer()
{
	float time = maxtime - gametime;
	if (time < 30.0)
	{
		Minuite->SetColorAndOpacity(FLinearColor::Red);
		Second->SetColorAndOpacity(FLinearColor::Red);
		MiliSeconds->SetColorAndOpacity(FLinearColor::Red);
	}
	TimeProgressBar->SetPercent(1 - (gametime / maxtime));
	int32 minuite = time / 60;
	int32 second = (int32)time % 60;
	int32 miliSeconds = (int32)(time * 100) % 100;
	Minuite->SetText(UKismetTextLibrary::Conv_DoubleToText(minuite, ERoundingMode::HalfToEven, false, true, 2));
	Second->SetText(UKismetTextLibrary::Conv_DoubleToText(second, ERoundingMode::HalfToEven, false, true, 2));
	MiliSeconds->SetText(UKismetTextLibrary::Conv_DoubleToText(miliSeconds, ERoundingMode::HalfToEven, false, true, 2));
}

void UW_InGameUI::SetTimerShow(bool _bshow)
{
	//UE_LOG(LogTemp, Warning, TEXT("Clear!!!!!!!"));
	if (_bshow)
	{
		gametime = 0;
		bISClear = false;
		TimeProgress_Box->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		bISClear = true;
		TimeProgress_Box->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UW_InGameUI::SetOverlayShow(bool bShow)
{
	if (bShow)
	{
		SetPlayerTD();
		DeathCount->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		DeathCount->SetVisibility(ESlateVisibility::Hidden);
	}
}

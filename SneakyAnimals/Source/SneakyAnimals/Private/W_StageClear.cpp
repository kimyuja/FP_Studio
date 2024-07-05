// Fill out your copyright notice in the Description page of Project Settings.


#include "W_StageClear.h"
#include "SAModeBase.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include "SAGameStateBase.h"

void UW_StageClear::NativeConstruct()
{
	Super::NativeConstruct();
	gameState = Cast<ASAGameStateBase>(GetWorld()->GetGameState());
	if (gameState)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameMode"));
	}
}

void UW_StageClear::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry,InDeltaTime);
}

void UW_StageClear::SetWidgetState()
{
	int32 stage = gameState->stageNum;
	ClearTime->SetText(gameState->MakeClearTime());
	switch (stage)
	{
	case 1:
		DeathCount_P1->SetText(FText::FromString(FString::FromInt(gameState->deathCount1[0])));
		DeathCount_P2->SetText(FText::FromString(FString::FromInt(gameState->deathCount1[1])));
		DeathCount_P3->SetText(FText::FromString(FString::FromInt(gameState->deathCount1[2])));
		DeathCount_P4->SetText(FText::FromString(FString::FromInt(gameState->deathCount1[3])));
		break;
	case 2:
		DeathCount_P1->SetText(FText::FromString(FString::FromInt(gameState->deathCount2[0])));
		DeathCount_P2->SetText(FText::FromString(FString::FromInt(gameState->deathCount2[1])));
		DeathCount_P3->SetText(FText::FromString(FString::FromInt(gameState->deathCount2[2])));
		DeathCount_P4->SetText(FText::FromString(FString::FromInt(gameState->deathCount2[3])));
		break;
	case 3:
		DeathCount_P1->SetText(FText::FromString(FString::FromInt(gameState->deathCount3[0])));
		DeathCount_P2->SetText(FText::FromString(FString::FromInt(gameState->deathCount3[1])));
		DeathCount_P3->SetText(FText::FromString(FString::FromInt(gameState->deathCount3[2])));
		DeathCount_P4->SetText(FText::FromString(FString::FromInt(gameState->deathCount3[3])));
		break;
	case 4:
		DeathCount_P1->SetText(FText::FromString(FString::FromInt(gameState->deathCount4[0])));
		DeathCount_P2->SetText(FText::FromString(FString::FromInt(gameState->deathCount4[1])));
		DeathCount_P3->SetText(FText::FromString(FString::FromInt(gameState->deathCount4[2])));
		DeathCount_P4->SetText(FText::FromString(FString::FromInt(gameState->deathCount4[3])));
		break;
	default:
		break;
	}
}

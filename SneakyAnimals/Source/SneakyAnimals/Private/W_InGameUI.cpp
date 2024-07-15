// Fill out your copyright notice in the Description page of Project Settings.


#include "W_InGameUI.h"
#include "SAGameStateBase.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>

void UW_InGameUI::NativeConstruct()
{
	gameState = Cast<ASAGameStateBase>(GetWorld()->GetGameState());
}

void UW_InGameUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (gameState == nullptr)
	{
		return;
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
}

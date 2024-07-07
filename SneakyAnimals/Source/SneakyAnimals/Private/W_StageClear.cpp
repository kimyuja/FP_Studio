// Fill out your copyright notice in the Description page of Project Settings.


#include "W_StageClear.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include "SAGameStateBase.h"

void UW_StageClear::NativeConstruct()
{
	Super::NativeConstruct();
	gameState = Cast<ASAGameStateBase>(GetWorld()->GetGameState());
}

void UW_StageClear::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry,InDeltaTime);

	if (bIsClear)
	{
		curtime += InDeltaTime;
	}

	if (curtime > 5)
	{
		NextRoundTime->SetText(FText::FromString(TEXT("0")));
		UE_LOG(LogTemp, Warning, TEXT("0"));
		bIsClear = false;
		curtime = 0;
	}
	else if (curtime > 4)
	{
		NextRoundTime->SetText(FText::FromString(TEXT("1")));
		UE_LOG(LogTemp, Warning, TEXT("1"));
	}
	else if (curtime > 3)
	{
		NextRoundTime->SetText(FText::FromString(TEXT("2")));
		UE_LOG(LogTemp, Warning, TEXT("2"));
	}
	else if (curtime > 2)
	{
		NextRoundTime->SetText(FText::FromString(TEXT("3")));
		UE_LOG(LogTemp, Warning, TEXT("3"));
	}
	else if (curtime > 1)
	{
		NextRoundTime->SetText(FText::FromString(TEXT("4")));
		UE_LOG(LogTemp, Warning, TEXT("4"));
	}
	else if (curtime > 0)
	{
		NextRoundTime->SetText(FText::FromString(TEXT("5")));
		UE_LOG(LogTemp, Warning, TEXT("5"));
	}
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

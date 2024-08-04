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
		//UE_LOG(LogTemp, Warning, TEXT("0"));
		bIsClear = false;
		//curtime = 0;
	}
	else if (curtime > 4)
	{
		NextRoundTime->SetText(FText::FromString(TEXT("1")));
		//UE_LOG(LogTemp, Warning, TEXT("1"));
	}
	else if (curtime > 3)
	{
		NextRoundTime->SetText(FText::FromString(TEXT("2")));
		//UE_LOG(LogTemp, Warning, TEXT("2"));
	}
	else if (curtime > 2)
	{
		NextRoundTime->SetText(FText::FromString(TEXT("3")));
		//UE_LOG(LogTemp, Warning, TEXT("3"));
	}
	else if (curtime > 1)
	{
		NextRoundTime->SetText(FText::FromString(TEXT("4")));
		//UE_LOG(LogTemp, Warning, TEXT("4"));
	}
	else if (curtime > 0)
	{
		NextRoundTime->SetText(FText::FromString(TEXT("5")));
		//UE_LOG(LogTemp, Warning, TEXT("5"));
	}
}

void UW_StageClear::SetWidgetState()
{
	int32 stage = gameState->stageNum;
	// LevelNum_Text->SetText(FText::FromString(FString::FromInt(gameState->stageNum - 1)));
	ClearTime->SetText(gameState->MakeClearTime());

	//// player name
	//Player1_Text->SetText(gameState->Get_UserName(0));
	//Player2_Text->SetText(gameState->Get_UserName(1));
	//Player3_Text->SetText(gameState->Get_UserName(2));
	//Player4_Text->SetText(gameState->Get_UserName(3));

	switch (stage)
	{
	// 마녀의집 클리어
	case 1:
		LevelNum_Text->SetText(FText::FromString("1"));
		LevelName_Text->SetText(FText::FromString("witch's house"));

		DeathCount_P1->SetText(FText::FromString(FString::FromInt(gameState->deathCount1[0])));
		DeathCount_P2->SetText(FText::FromString(FString::FromInt(gameState->deathCount1[1])));
		DeathCount_P3->SetText(FText::FromString(FString::FromInt(gameState->deathCount1[2])));
		DeathCount_P4->SetText(FText::FromString(FString::FromInt(gameState->deathCount1[3])));
		UE_LOG(LogTemp, Warning, TEXT("%d"), gameState->deathCount1[0]);
		UE_LOG(LogTemp, Warning, TEXT("%d"), gameState->deathCount1[1]);
		UE_LOG(LogTemp, Warning, TEXT("%d"), gameState->deathCount1[2]);
		UE_LOG(LogTemp, Warning, TEXT("%d"), gameState->deathCount1[3]);
		break;
	// 잠수함 클리어
	case 2:
		LevelNum_Text->SetText(FText::FromString("2"));
		LevelName_Text->SetText(FText::FromString("submarine"));

		DeathCount_P1->SetText(FText::FromString(FString::FromInt(gameState->deathCount2[0])));
		DeathCount_P2->SetText(FText::FromString(FString::FromInt(gameState->deathCount2[1])));
		DeathCount_P3->SetText(FText::FromString(FString::FromInt(gameState->deathCount2[2])));
		DeathCount_P4->SetText(FText::FromString(FString::FromInt(gameState->deathCount2[3])));
		UE_LOG(LogTemp, Warning, TEXT("%d"), gameState->deathCount2[0]);
		UE_LOG(LogTemp, Warning, TEXT("%d"), gameState->deathCount2[1]);
		UE_LOG(LogTemp, Warning, TEXT("%d"), gameState->deathCount2[2]);
		UE_LOG(LogTemp, Warning, TEXT("%d"), gameState->deathCount2[3]);
		break;
	// 슈퍼마켓 클리어
	case 3:
		LevelNum_Text->SetText(FText::FromString("3"));
		LevelName_Text->SetText(FText::FromString("supermarket"));

		DeathCount_P1->SetText(FText::FromString(FString::FromInt(gameState->deathCount3[0])));
		DeathCount_P2->SetText(FText::FromString(FString::FromInt(gameState->deathCount3[1])));
		DeathCount_P3->SetText(FText::FromString(FString::FromInt(gameState->deathCount3[2])));
		DeathCount_P4->SetText(FText::FromString(FString::FromInt(gameState->deathCount3[3])));
		UE_LOG(LogTemp, Warning, TEXT("%d"), gameState->deathCount3[0]);
		UE_LOG(LogTemp, Warning, TEXT("%d"), gameState->deathCount3[1]);
		UE_LOG(LogTemp, Warning, TEXT("%d"), gameState->deathCount3[2]);
		UE_LOG(LogTemp, Warning, TEXT("%d"), gameState->deathCount3[3]);
		break;
	// 금고 클리어
	case 4:
		LevelNum_Text->SetText(FText::FromString("4"));
		LevelName_Text->SetText(FText::FromString("bank vault"));

		DeathCount_P1->SetText(FText::FromString(FString::FromInt(gameState->deathCount4[0])));
		DeathCount_P2->SetText(FText::FromString(FString::FromInt(gameState->deathCount4[1])));
		DeathCount_P3->SetText(FText::FromString(FString::FromInt(gameState->deathCount4[2])));
		DeathCount_P4->SetText(FText::FromString(FString::FromInt(gameState->deathCount4[3])));
		UE_LOG(LogTemp, Warning, TEXT("%d"), gameState->deathCount4[0]);
		UE_LOG(LogTemp, Warning, TEXT("%d"), gameState->deathCount4[1]);
		UE_LOG(LogTemp, Warning, TEXT("%d"), gameState->deathCount4[2]);
		UE_LOG(LogTemp, Warning, TEXT("%d"), gameState->deathCount4[3]);
		break;
	default:
		break;
	}
}

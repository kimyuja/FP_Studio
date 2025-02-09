// Fill out your copyright notice in the Description page of Project Settings.


#include "W_InGameUI.h"
#include "SAGameStateBase.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/ProgressBar.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Overlay.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetTextLibrary.h>
#include "TestPlayer.h"

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

	//// player 1,2,3,4에 user name 넣어주기
	//Player1_NameText->SetText(gameState->Get_UserName(0));
	//Player2_NameText->SetText(gameState->Get_UserName(1));
	//Player3_NameText->SetText(gameState->Get_UserName(2));
	//Player4_NameText->SetText(gameState->Get_UserName(3));
	if (gameState == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("gameState is nullptr"));
		return;
	}

	int32 NumProfiles = gameState->UserProfiles.Num();
	UE_LOG(LogTemp, Log, TEXT("Number of UserProfiles: %d"), NumProfiles);

	// player 1,2,3,4에 user name 넣어주기
	if (NumProfiles > 0) {
		Player1_NameText->SetText(gameState->Get_UserName(0));
	}
	else {
		Player1_NameText->SetText(FText::FromString("Player 1"));
	}

	if (NumProfiles > 1) {
		Player2_NameText->SetText(gameState->Get_UserName(1));
	}
	else {
		Player2_NameText->SetText(FText::FromString("Player 2"));
	}

	if (NumProfiles > 2) {
		Player3_NameText->SetText(gameState->Get_UserName(2));
	}
	else {
		Player3_NameText->SetText(FText::FromString("Player 3"));
	}

	if (NumProfiles > 3) {
		Player4_NameText->SetText(gameState->Get_UserName(3));
	}
	else {
		Player4_NameText->SetText(FText::FromString("Player 4"));
	}

	switch (stage)
	{
	case 1:
		Player1_DeathCount->SetText(FText::FromString(FString::FromInt(gameState->deathCount1[0])));
		Player2_DeathCount->SetText(FText::FromString(FString::FromInt(gameState->deathCount1[1])));
		Player3_DeathCount->SetText(FText::FromString(FString::FromInt(gameState->deathCount1[2])));
		Player4_DeathCount->SetText(FText::FromString(FString::FromInt(gameState->deathCount1[3])));
		UE_LOG(LogTemp, Warning, TEXT("Player 1 Death Count %d"), gameState->deathCount1[0]);
		UE_LOG(LogTemp, Warning, TEXT("Player 2 Death Count %d"), gameState->deathCount1[1]);
		UE_LOG(LogTemp, Warning, TEXT("Player 3 Death Count %d"), gameState->deathCount1[2]);
		UE_LOG(LogTemp, Warning, TEXT("Player 4 Death Count %d"), gameState->deathCount1[3]);
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
	UE_LOG(LogTemp, Warning, TEXT("Player 1 Trigger Count %d"), gameState->deathCount1[0]);
	UE_LOG(LogTemp, Warning, TEXT("Player 2 Trigger Count %d"), gameState->deathCount1[1]);
	UE_LOG(LogTemp, Warning, TEXT("Player 3 Trigger Count %d"), gameState->deathCount1[2]);
	UE_LOG(LogTemp, Warning, TEXT("Player 4 Trigger Count %d"), gameState->deathCount1[3]);
}

void UW_InGameUI::SetStageTimer()
{
	float time = maxtime - gametime;
	time = FMath::Clamp(time, 0, 180);
	if (time <= 0)
	{
		auto* p = Cast<ATestPlayer>(GetOwningPlayer());
		if(p)
		{
			p->ServerRPC_ClearStage();
		}
		UE_LOG(LogTemp, Warning, TEXT("End"));
	}
	if (time < 30.0)
	{
		Minuite->SetColorAndOpacity(FLinearColor::Red);
		Second->SetColorAndOpacity(FLinearColor::Red);
		MiliSeconds->SetColorAndOpacity(FLinearColor::Red);
	}
	else
	{
		Minuite->SetColorAndOpacity(FLinearColor::Black);
		Second->SetColorAndOpacity(FLinearColor::Black);
		MiliSeconds->SetColorAndOpacity(FLinearColor::Black);
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

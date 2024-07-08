// Fill out your copyright notice in the Description page of Project Settings.


#include "SAGameStateBase.h"
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include "TestPlayer.h"
#include <../../../../../../../Source/Runtime/Engine/Public/Net/UnrealNetwork.h>



ASAGameStateBase::ASAGameStateBase()
{
    PrimaryActorTick.bStartWithTickEnabled = true; //플레이하자마자 Tick처리
    PrimaryActorTick.bCanEverTick = true; // Tick처리를 가능하게함
    bReplicates = true;
}

void ASAGameStateBase::BeginPlay()
{
    Super::BeginPlay();
}

void ASAGameStateBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (bOnGame)
    {
        clearTime[stageNum - 1] += DeltaTime;
    }
}

void ASAGameStateBase::SetStageStart()
{
    bOnGame = true;
    //clearTime[stageNum] = 0;
}

void ASAGameStateBase::SetClearInstance()
{
	stageNum = 1;

	clearTime.Empty();

	deathCount1.Empty();
	deathCount2.Empty();
	deathCount3.Empty();
	deathCount4.Empty();

	for (int i = 0; i < 4; i++)
	{
		clearTime.Add(0);
		deathCount1.Add(0);
		deathCount2.Add(0);
		deathCount3.Add(0);
		deathCount4.Add(0);
        //stageLoc.Add(FVector(0,0,0));
	}
    stageLoc.Add(FVector(0, 0, 0));
    stageLoc.Add(FVector(50000, -50000, 0));
    stageLoc.Add(FVector(0, 1000, 0));
    stageLoc.Add(FVector(-1000, 0, 0));
    stageLoc.Add(FVector(0, -1000, 0));

	voteCount.Empty();

}

void ASAGameStateBase::SetPlayerNum()
{
    TArray<int32> playerNums = { 0,1,2,3 };
    int32 LastIndex = playerNums.Num() - 1;
    int count = 0;
    for (int32 i = 0; i <= LastIndex; ++i)
    {
        int32 Index = FMath::RandRange(0, LastIndex);
        if (i != Index)
        {
            playerNums.Swap(i, Index);
        }
    }
    for (TActorIterator<ATestPlayer> it(GetWorld()); it; ++it)
    {
        it->playerNum = playerNums[count];
        count++;
    }
}

void ASAGameStateBase::SetDeathCountUp(int32 playerNum)
{
    UE_LOG(LogTemp, Warning, TEXT("stage %d : Player %d"), stageNum, playerNum);

	switch (stageNum)
	{
	case 1:
		deathCount1[playerNum]++;
		//UE_LOG(LogTemp, Warning, TEXT("deathCount %d"), deathCount1[playerNum]);
		break;
	case 2:
		deathCount2[playerNum]++;
		//UE_LOG(LogTemp, Warning, TEXT("deathCount %d"), deathCount2[playerNum]);
		break;
	case 3:
		deathCount3[playerNum]++;
		//UE_LOG(LogTemp, Warning, TEXT("deathCount %d"), deathCount3[playerNum]);
		break;
	case 4:
		deathCount4[playerNum]++;
		//UE_LOG(LogTemp, Warning, TEXT("deathCount %d"), deathCount4[playerNum]);
		break;
	default:
		break;
	}
}

FText ASAGameStateBase::MakeClearTime()
{
    if (clearTime[stageNum - 1] < 10.0f)
    {
        return FText::FromString("00 : 0" + FString::FromInt(clearTime[stageNum - 1]));
    }
    else if (clearTime[stageNum - 1] < 60.0f)
    {
        return FText::FromString("00 : " + FString::FromInt(clearTime[stageNum - 1]));
    }
    else if (clearTime[stageNum - 1] < 70.0f)
    {
        return FText::FromString("01 : 0" + FString::FromInt((int)clearTime[stageNum - 1] % 60));
    }
    else if (clearTime[stageNum - 1] < 120.0f)
    {
        return FText::FromString("01 : " + FString::FromInt((int)clearTime[stageNum - 1] % 60));
    }
    else if (clearTime[stageNum - 1] < 130.0f)
    {
        return FText::FromString("02 : 0" + FString::FromInt((int)clearTime[stageNum - 1] % 60));
    }
    else if (clearTime[stageNum - 1] < 180.0f)
    {
        return FText::FromString("02 : " + FString::FromInt((int)clearTime[stageNum - 1] % 60));
    }
    else
    {
        return FText::FromString("03 : 00");
    }

    /*FString clearS = FString::FromInt(clearTime[stageNum - 1]/60) + ":" + FString::FromInt((int)clearTime[stageNum - 1] % 60);
    return FText::FromString(clearS);*/
}


void ASAGameStateBase::MoveNextStage(FVector moveLoc)
{
    if (players.Num() == 0)
    {
        for (TActorIterator<ATestPlayer> it(GetWorld()); it; ++it)
        {
            players.Add(*it);
        }
    }
    if (stageLoc.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No %d stage Location"), stageNum);
        return;
    }
    for (int32 i = 0; i < players.Num(); i++)
    {
        players[i]->SetActorLocation(moveLoc);
        UE_LOG(LogTemp, Warning, TEXT("player %d Move"), i);
    }
    stageNum++;
}

void ASAGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ASAGameStateBase, stageNum);
    DOREPLIFETIME(ASAGameStateBase, deathCount1);
    DOREPLIFETIME(ASAGameStateBase, deathCount2);
    DOREPLIFETIME(ASAGameStateBase, deathCount3);
    DOREPLIFETIME(ASAGameStateBase, deathCount4);
    DOREPLIFETIME(ASAGameStateBase, clearTime);
    DOREPLIFETIME(ASAGameStateBase, voteCount);
    DOREPLIFETIME(ASAGameStateBase, bOnGame);
    DOREPLIFETIME(ASAGameStateBase, stageLoc);
}

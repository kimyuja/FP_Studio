// Fill out your copyright notice in the Description page of Project Settings.


#include "SAGameStateBase.h"
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include "TestPlayer.h"
#include <../../../../../../../Source/Runtime/Engine/Public/Net/UnrealNetwork.h>
#include "PS_Gameplay.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>



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
	clearTime[stageNum - 1] += DeltaTime;
	//UE_LOG(LogTemp, Warning, TEXT("STAGE %d, %f"), (stageNum - 1), clearTime[stageNum - 1]);
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
    voteCount.Empty();
    activeCount.Empty();

    for (int i = 0; i < 4; i++)
    {
        clearTime.Add(0);
        deathCount1.Add(0);
        deathCount2.Add(0);
        deathCount3.Add(0);
        deathCount4.Add(0);
        voteCount.Add(0);
        activeCount.Add(0);
        //stageLoc.Add(FVector(0,0,0));
    }
    stageLoc.Add(FVector(0, 0, 0));
    stageLoc.Add(FVector(50000, -50000, 0));
    stageLoc.Add(FVector(50000, 50000, 0));
    stageLoc.Add(FVector(-50000, 50000, 0));
    stageLoc.Add(FVector(-50000, -50000, 0));
}

void ASAGameStateBase::SetPlayerNum()
{
    int32 idx = 0; // 인덱스 초기화
    int32 RandSeed = FMath::Rand(); // 인덱스에 더할 랜덤 숫자 저장 (각 플레이어에게 더하는 랜덤 숫자는 동일)

    // TActorIterator<ATestPlayer> 루프를 돌면서 각 ATestPlayer의 playerNum과 playerShowNum 설정
    idx = 0;
    for (TActorIterator<ATestPlayer> it(GetWorld()); it; ++it)
    {
        if (PlayerArray.Num() < 1)
        {
            return;
        }
        it->playerNum = (idx + RandSeed) % PlayerArray.Num();
        it->playerShowNum = idx;
        // player 전부 들어오기 전에 맵 커스텀 UI 띄워버리면 중복되는 playerNum이 들어감. 그래서 레벨 BP 에서 딜레이를 준다.
        if (it->GetLocalRole() == ROLE_Authority) // 서버에서만 호출
        {
            it->ClientRPC_Update_PlayerNum_PlayerShowNum(it->playerNum, it->playerShowNum);
        }
        idx++;
    }
}

void ASAGameStateBase::SetEndNum()
{
    for (TActorIterator<ATestPlayer> it(GetWorld()); it; ++it)
    {
        if (PlayerArray.Num() < 1)
        {
            return;
        }
        it->endNum = PlayerArray.Num();
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

    /*FString clearS = FString::FromInt(clearTime[stageNum - 1] / 60) + ":" + FString::FromInt((int)clearTime[stageNum - 1] % 60);
    return FText::FromString(clearS);*/
}

void ASAGameStateBase::MoveNextStage(FVector moveLoc)
{
    /*if (players.Num() == 0)
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
        UE_LOG(LogTemp, Warning, TEXT("player %d Move to Stage %d Location"), i, stageNum);
    }*/

    for (TActorIterator<ATestPlayer> it(GetWorld()); it; ++it)
    {
        it->SetActorLocation(moveLoc);
        UE_LOG(LogTemp, Warning, TEXT("player %d Move to Stage %d Location"), it->playerNum, stageNum);
    }

    //stageNum++;
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
	DOREPLIFETIME(ASAGameStateBase, bIsMixed);
}

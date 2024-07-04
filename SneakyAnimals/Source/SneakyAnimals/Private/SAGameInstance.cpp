// Fill out your copyright notice in the Description page of Project Settings.


#include "SAGameInstance.h"
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include "TestPlayer.h"

void USAGameInstance::SetClearInstance()
{
	stageNum = 0;

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
    }

	voteCount.Empty();
}

void USAGameInstance::SetPlayerNum()
{
    TArray<int32> playerNums = {0,1,2,3};
    int32 LastIndex = playerNums.Num() - 1;
    int count = 0;
    for (int32 i = 0; i <= 20; ++i)
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

void USAGameInstance::SetDeathCountUp(int32 playerNum)
{
    switch (stageNum)
    {
    case 1:
        deathCount1[playerNum]++;
        break;
    case 2:
        deathCount2[playerNum]++;
        break;
    case 3:
        deathCount3[playerNum]++;
        break;
    case 4:
        deathCount4[playerNum]++;
        break;
    default:
        break;
    }
}

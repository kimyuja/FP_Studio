// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SAModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API ASAModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 stageNum = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> clearTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> deathCount1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> deathCount2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> deathCount3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> deathCount4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> voteCount;

	UFUNCTION()
	void SetClearInstance();

	UFUNCTION()
	void SetPlayerNum();

	UFUNCTION()
	void SetDeathCountUp(int32 playerNum);

};

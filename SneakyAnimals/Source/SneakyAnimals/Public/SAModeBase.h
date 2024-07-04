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

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)override;

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

	UFUNCTION(BlueprintCallable)
	void SetClearInstance();

	UFUNCTION(BlueprintCallable)
	void SetPlayerNum();

	UFUNCTION(BlueprintCallable)
	void SetDeathCountUp(int32 playerNum);

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "SAGameState.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API ASAGameState : public AGameState
{
	GENERATED_BODY()

public:

	ASAGameState();

	virtual void Tick(float DeltaTime)override;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	int32 stageNum = 0;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	TArray<float> clearTime;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	TArray<int32> deathCount1;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	TArray<int32> deathCount2;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	TArray<int32> deathCount3;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	TArray<int32> deathCount4;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	TArray<int32> voteCount;

	UPROPERTY(Replicated, EditAnywhere)
	bool bOnGame = false;
	
	UFUNCTION(BlueprintCallable)
	void SetStageStart();

	UFUNCTION(BlueprintCallable)
	void SetClearInstance();

	UFUNCTION(BlueprintCallable)
	void SetPlayerNum();

	UFUNCTION(BlueprintCallable)
	void SetDeathCountUp(int32 playerNum);

	UFUNCTION(BlueprintCallable)
	FText MakeClearTime();

	UFUNCTION(Server, Reliable)
	void ServerRPC_SetClearInstance();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_SetClearInstance();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
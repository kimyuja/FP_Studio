// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "DataStructure.h"
#include "PS_Base.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API APS_Base : public APlayerState
{
	GENERATED_BODY()
protected:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category="MyPlayerData")
	FStructure_Player_Appearance Player_Appearance;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

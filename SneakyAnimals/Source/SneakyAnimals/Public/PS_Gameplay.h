// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PS_Base.h"
#include "PS_Gameplay.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API APS_Gameplay : public APS_Base
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void ClientRPC_Init_Implementation() override;

public:
	virtual void ClientRPC_Init() override;

	virtual void OnRep_Player_UserProfile_OR() override;
};

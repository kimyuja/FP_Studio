// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PC_Base.h"
#include "PC_Gameplay.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API APC_Gameplay : public APC_Base
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable)
	void ServerRPC_Spawn_Character();

private:
	bool ValidatePlayerState();
	void Setup_PC();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PS_Base.h"
#include "PS_Lobby.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API APS_Lobby : public APS_Base
{
	GENERATED_BODY()
	
public:

	// Gets set by Lobby Game Mode
	UPROPERTY(Replicated, EditAnywhere, Category =  "Lobby Settings")
	bool bCanHostForceLaunchGame = false;

	virtual void OnRep_Player_Appearance_OR() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

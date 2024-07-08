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

public:
	UPROPERTY(ReplicatedUsing = OnRep_Player_Appearance)
	FStructure_Player_Appearance Player_Appearance;

	UFUNCTION()
	void OnRep_Player_Appearance();

	UFUNCTION()
	virtual void OnRep_Player_Appearance_OR();

	// RPC (Remote Procedure Call)
	UFUNCTION(Server, Reliable)
	void ServerRPC_Update_Player_Appearance(const FStructure_Player_Appearance _Player_Appearance);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


};
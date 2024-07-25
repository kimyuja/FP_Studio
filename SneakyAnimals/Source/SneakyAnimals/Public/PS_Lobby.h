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

private:
	void Toggle_ReadyStatus();
	

protected:
	virtual void BeginPlay() override;
	//virtual void ClientRPC_Init_Implementation();
	virtual void Load_Player_ConnectionInfo(bool _ClientReadyStatus) override;

public:
	UFUNCTION(Client, Reliable)
	void ClientRPC_Init_Test();
	virtual void ClientRPC_Init() override;

	// Gets set by Lobby Game Mode
	UPROPERTY(Replicated, EditAnywhere, Category =  "Lobby Settings")
	bool bCanHostForceLaunchGame = false;

	virtual void OnRep_Player_Appearance_OR() override;
	virtual void OnRep_Player_UserProfile_OR() override;
	virtual void OnRep_Player_ConnectionInfo_OR() override;

	UFUNCTION(BlueprintCallable, Category="Player Ready Status")
	bool Get_ReadyStatus();
	
	UFUNCTION(BlueprintCallable, Category="Player Ready Status")
	void Set_ReadyStatus(bool _bReadyStatus);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable)
	void ServerRPC_Toggle_ReadyStatus();
};

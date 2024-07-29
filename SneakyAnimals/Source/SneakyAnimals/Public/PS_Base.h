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

private:
	UTexture2D* T_ProfilePicture;

protected:
    UFUNCTION()
    virtual void Load_Player_Appearance();
    UFUNCTION()
	virtual void Load_Player_UserProfile();
	//UFUNCTION()
	//virtual void Load_Player_UserProfile_Test();
    UFUNCTION()
    virtual void Load_Player_ConnectionInfo(bool ClientReadyStatus);

public:
	APS_Base();
	virtual void BeginPlay() override;

	UPROPERTY(ReplicatedUsing = OnRep_Player_Appearance)
	FStructure_Player_Appearance Player_Appearance;
	
	UPROPERTY(ReplicatedUsing = OnRep_Player_UserProfile, BlueprintReadOnly)
	FStructure_UserProfile Player_UserProfile;
	
	UPROPERTY(ReplicatedUsing = OnRep_Player_ConnectionInfo)
	FStructure_PlayerConnectionInfo Player_ConnectionInfo;

	UFUNCTION()
	void OnRep_Player_Appearance();
	
	UFUNCTION()
	void OnRep_Player_UserProfile();
	
	UFUNCTION()
	void OnRep_Player_ConnectionInfo();

	UFUNCTION()
	virtual void OnRep_Player_Appearance_OR();
	
	UFUNCTION()
	virtual void OnRep_Player_UserProfile_OR();
	
	UFUNCTION()
	virtual void OnRep_Player_ConnectionInfo_OR();

	// RPC (Remote Procedure Call)
	UFUNCTION(Server, Reliable)
	void ServerRPC_Update_Player_Appearance(const FStructure_Player_Appearance& _Player_Appearance);
	
	UFUNCTION(Server, Reliable)
	void ServerRPC_Update_Player_UserProfile(const FStructure_UserProfile& _Player_UserProfile);
	
	UFUNCTION(Server, Reliable)
	void ServerRPC_Update_Player_ConnectionInfo(const FStructure_PlayerConnectionInfo& _Player_ConnectionInfo);

	// KYJ Test
	UFUNCTION(Server, Reliable)
	void ServerRPC_Update_SaveGame_Player_UserProfile(int32 uniqueIdx, const FStructure_UserProfile _Player_UserProfile);

	UFUNCTION(Client, Reliable)
    virtual void ClientRPC_Init();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
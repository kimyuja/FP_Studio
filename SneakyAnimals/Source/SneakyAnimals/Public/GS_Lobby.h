// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GS_Base.h"
#include "DataStructure.h"
#include "GS_Lobby.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API AGS_Lobby : public AGS_Base
{
	GENERATED_BODY()

public:
	// Gets set by Lobby Game Mode
	UPROPERTY(ReplicatedUsing=OnRep_bReadyForLaunch, EditAnywhere, Category =  "Lobby Status")
	bool bReadyForLaunch = false;
	
	UPROPERTY(ReplicatedUsing=OnRep_bLaunching, EditAnywhere, Category =  "Lobby Status")
	bool bLaunching = false;
	
	UPROPERTY(ReplicatedUsing= OnRep_ServerName, EditAnywhere, Category =  "Session Info")
	FText ServerName;
	
	UPROPERTY(ReplicatedUsing= OnRep_ConnectedPlayers_PlayerConnectionInfo, EditAnywhere, Category =  "Players")
	TArray<FStructure_ConnectedPlayer> ConnectedPlayers_PlayerConnectionInfo;

	UFUNCTION()
	void OnRep_bReadyForLaunch();
	
	UFUNCTION()
	void OnRep_bLaunching();
	
	UFUNCTION()
	void OnRep_ServerName();
	
	UFUNCTION()
	void OnRep_ConnectedPlayers_PlayerConnectionInfo();

	UFUNCTION(Category = "User Interface Updates")
	void Update_Lobby_PlayerList();
	
	UFUNCTION(Category = "User Interface Updates")
	void Update_Host_ReadyUpButton(const FText& Text, bool bWarningState);
	
	UFUNCTION(Category="User Interface Updates")
	void Update_ServerName();
	
	UFUNCTION(Category="Lobby Updates")
	void Update_ConnectedPlayers_Array();
	
	UFUNCTION(Category="Lobby Updates")
	void Update_Lobby();
	
	UFUNCTION(Category="Lobby Status")
	void Check_EveryoneReady();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(Category = "Lobby Status")
	void Update_KickCount();
	
	UFUNCTION(Category = "Lobby Status")
	int32 Get_KickCount();

private:
	int32 KickCount = 0;
	void Update_LobbyStatus_Indicator(ESlateVisibility s, FText LobbyStatus_Text);
	void Update_Lobby_Launchevent(bool bLaunchingGame);
	void Reset_Player_ReadyStatus();
};

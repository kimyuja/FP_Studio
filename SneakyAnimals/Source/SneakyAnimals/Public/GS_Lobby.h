// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GS_Base.h"
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

	UFUNCTION()
	void OnRep_bReadyForLaunch();

	UFUNCTION()
	void Update_Host_ReadyUpButton(const FText& Text, bool bWarningState);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
private:
	void Update_LobbyStatus_Indicator(ESlateVisibility, FText LobbyStatus_Text);
	
};

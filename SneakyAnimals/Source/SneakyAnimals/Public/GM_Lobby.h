// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GM_Base.h"
#include "GM_Lobby.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API AGM_Lobby : public AGM_Base
{
	GENERATED_BODY()

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void SwapPlayerControllers(APlayerController* OldPC, APlayerController* NewPC) override;
	virtual void Logout(AController* Exiting) override;

	void ServerTravel();
	void Create_MapTravel_LoadingScreens();
private:
	bool bCanHostForceLaunchGame = false;
	void Handle_PlayerDisconnection();
};

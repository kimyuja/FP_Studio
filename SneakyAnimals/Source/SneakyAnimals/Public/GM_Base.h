// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GM_Base.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API AGM_Base : public AGameMode
{
	GENERATED_BODY()
	
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void SwapPlayerControllers(APlayerController* OldPC, APlayerController* NewPC) override;
	virtual void Logout(AController* Exiting) override;

    UPROPERTY(BlueprintReadOnly, Category = "Players")
    TArray<APlayerController*> All_PCs;

	UFUNCTION(BlueprintCallable, Category = "Manage")
	void KickPlayer(int32 PlayerID);
	
	UFUNCTION(BlueprintCallable, Category = "Connection")
	void Handle_PlayerConnection(APlayerController* NewPlayer);
	
	UFUNCTION(BlueprintCallable, Category = "Connection")
	void Handle_PlayerDisconnection(UObject* Player);

protected:
	void ServerTravel_ToGameplayMap();

};

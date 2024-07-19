// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PC_Base.generated.h"


/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API APC_Base : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(Client, Reliable)
	void ClientRPC_Create_MapTravel_LoadingScreen();

	UPROPERTY(BlueprintReadWrite, Category="MySettings")
	UTexture2D* T_Color4_Normal;

protected:
	virtual void BeginPlay() override;
	void CleanUp_UI();

private:


};

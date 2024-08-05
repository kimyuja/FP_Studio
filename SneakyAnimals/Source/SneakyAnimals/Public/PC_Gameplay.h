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
	virtual void SetupInputComponent() override;

	UFUNCTION(Server, Reliable)
	void ServerRPC_Spawn_Character();

	//----------------Key Inform---------
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* Toggle_KeyInform;
	
	UPROPERTY(EditAnywhere, Category="References")
	TSubclassOf<class UUserWidget> KeyInfom_bp;

	UPROPERTY(EditAnywhere, Category = "References")
	class UUserWidget* KeyInfom_inst;

	UFUNCTION()
	void Toggle_Key_Inform();
	//-----------------------------------

private:
	bool ValidatePlayerState();
	void Setup_PC();
};

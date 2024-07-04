// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PC_Base.h"
#include "PC_Lobby.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API APC_Lobby : public APC_Base
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetupInputComponent() override;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* Lobby_InputMappingContext;

	/** Toggle_CharacterCustomization Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* Toggle_CharacterCustomizationAction;

	UFUNCTION()
	void Toggle_CharacterCustomization();
};

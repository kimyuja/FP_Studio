// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h>
#include "TestPlayer.generated.h"

UCLASS()
class SNEAKYANIMALS_API ATestPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATestPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, Category = Test)
	class UCameraComponent* camera;

	UPROPERTY(EditDefaultsOnly, Category = Test)
	class USpringArmComponent* cameraBoom;

	UPROPERTY(EditDefaultsOnly, Category = Test)
	class UInputMappingContext* imc_testmove;

	UPROPERTY(EditDefaultsOnly, Category = Test)
	class UInputAction* ia_move;

	UPROPERTY(EditDefaultsOnly, Category = Test)
	class UInputAction* ia_jump;

	UPROPERTY(EditDefaultsOnly, Category = Test)
	class UInputAction* ia_look;

	bool bCanActive;

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void PlayerJump(const FInputActionValue& Value);

	void PlayerJumpEnd(const FInputActionValue& Value);

	void GimmickSearch();


};

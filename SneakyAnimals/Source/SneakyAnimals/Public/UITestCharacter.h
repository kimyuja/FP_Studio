// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h>
#include "WitchsHouseStruct.h"
#include "UITestCharacter.generated.h"

UCLASS()
class SNEAKYANIMALS_API AUITestCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUITestCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	FWitchStruct WitchsStruct;

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

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void PlayerJump(const FInputActionValue& Value);

	void PlayerJumpEnd(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UMapCustomWidget> customMapWidgetClass;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
    class UMapCustomWidget* customMapWidget;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="UI")
    class UItemComponent* itemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UMapCustomWidget> MapCustomWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
    class UMapCustomWidget* MapCustomWidget;

	// UFUNCTION()
    // void CreateMapCustomWidget();

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    // class UItemComponent* itemComponent;

};

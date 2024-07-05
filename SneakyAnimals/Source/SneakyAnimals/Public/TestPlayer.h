// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h>
#include "I_LobbyToggleInterface.h"
#include "TestPlayer.generated.h"

class AClearDoor;

UCLASS()
class SNEAKYANIMALS_API ATestPlayer : public ACharacter, public II_LobbyToggleInterface
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

	UPROPERTY(EditDefaultsOnly, Category = Test)
	class UInputAction* ia_activeG;

	UPROPERTY(EditDefaultsOnly, Category = Test)
	class UInputAction* ia_emo1;

	UPROPERTY(EditDefaultsOnly, Category = Test)
	class UInputAction* ia_emo2;

	UPROPERTY(EditDefaultsOnly, Category = Test)
	class UInputAction* ia_emo3;

	UPROPERTY(EditDefaultsOnly, Category = Test)
	FVector respawnLoc = FVector(0, 0, 80);
	
	UPROPERTY(EditDefaultsOnly, Category = Test)
	TSubclassOf<class UStaticMesh> broom;

	UPROPERTY(EditDefaultsOnly, Category = Test)
	TSubclassOf<UUserWidget> emoUI;

	UPROPERTY(EditDefaultsOnly, Category = Test)
	class UWidgetComponent* emoticonUI;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> stageClearUI;

	UPROPERTY(EditDefaultsOnly)
	class UW_StageClear* clearUI;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class ASAGameStateBase* gameState;

	UPROPERTY(Replicated,EditAnywhere,BlueprintReadWrite)
	int32 playerNum;

	bool bCanActive;

	bool bCanOpenDoor = false;

	bool bIsDie = false;

	bool bIsGoodDriver = false;

	bool bIsBlack = false;

	float lerpTime;

	class AGimmick* g;

	FTimerHandle falloverT;

	FTimerHandle poorDriveT;

	FTimerHandle endManT;

	FTimerHandle ThunderT;

	UPROPERTY()
	TObjectPtr<AClearDoor> CheckDoor;

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void PlayerJump(const FInputActionValue& Value);

	void PlayerJumpEnd(const FInputActionValue& Value);

	void ActiveGimmick(const FInputActionValue& Value);

	void ShowEmo(const FInputActionValue& Value);

	void DeathCounting();

	void FadeInOut(bool bInOut);

	void BlackScreen();

	void Respawn(float delaytime = 3.0f);

	void GimmickSearch();

	void Death_Fallover();

	void Death_Homerun(FVector impactLoc, float power = (1000.0f));

	void Death_PoorDrive(bool bIsBestDriver);

	void Death_EndMan();

	void Death_Thunderclap();

	UFUNCTION()
	void Toggle_CharacterCustomization_Implementation();

    // Timer function
    UFUNCTION()
    void SetThirdPersonView();

	UFUNCTION(NetMulticast, Reliable)
    void Multicast_SetThirdPersonView();

	UFUNCTION(Server, Reliable)
	void ServerRPC_ActiveGimmick(ATestPlayer* aP);

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_ActiveGimmick(ATestPlayer* _aP);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Test)
	class UCameraComponent* camera;

	UPROPERTY(EditDefaultsOnly, Category = Test)
	class UCameraComponent* customCamera;

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
	class UInputAction* ia_currentScore;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite ,Category = Test)
	FVector respawnLoc = FVector(0, 0, 80);
	
	UPROPERTY(EditDefaultsOnly, Category = Test)
	TSubclassOf<class UStaticMesh> broom;

	UPROPERTY(EditDefaultsOnly, Category = Test)
	TSubclassOf<UUserWidget> emoUI;

	UPROPERTY(EditDefaultsOnly, Category = Test)
	class UWidgetComponent* emoticonUI;

	UPROPERTY(EditDefaultsOnly, Category = Test)
	class UUserEmoticon* myEmoUI;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> stageClearUI;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> voteUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UW_StageClear* clearUI;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> mainUIF;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UW_InGameUI* mainUI;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class ASAGameStateBase* gameState;

	UPROPERTY(Replicated,EditAnywhere,BlueprintReadWrite)
	int32 playerNum;

	UPROPERTY(Replicated,EditAnywhere,BlueprintReadWrite)
	int32 playerShowNum;

	UPROPERTY(ReplicatedUsing = OnRep_Current_SkeletalMesh)
	USkeletalMesh* Current_SkeletalMesh;
	
	UPROPERTY(ReplicatedUsing = OnRep_Current_Accessories)
	USkeletalMesh* Current_Accessories;
	
	UPROPERTY(ReplicatedUsing = OnRep_Current_Skins)
	UMaterial* Current_Skins;
	
	UPROPERTY(ReplicatedUsing = OnRep_Current_Eyes)
	UMaterial* Current_Eyes;

	UPROPERTY(ReplicatedUsing = OnRep_Current_Top)
	USkeletalMesh* Current_Top;

	UPROPERTY(ReplicatedUsing = OnRep_Current_Bottom)
	USkeletalMesh* Current_Bottom;

	UPROPERTY(ReplicatedUsing = OnRep_Current_Outer)
	USkeletalMesh* Current_Outer;

	UPROPERTY(ReplicatedUsing = OnRep_Current_Dress)
	USkeletalMesh* Current_Dress;

    // �Ǽ��縮 ���̷�Ż �޽� ������Ʈ
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USkeletalMeshComponent* SM_Accessories;

	// ���� ���̷�Ż �޽� ������Ʈ
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USkeletalMeshComponent* SM_Top;

	// ���� ���̷�Ż �޽� ������Ʈ
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USkeletalMeshComponent* SM_Bottom;

	// ���� ���̷�Ż �޽� ������Ʈ
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USkeletalMeshComponent* SM_Outer;

	// �巹�� ���̷�Ż �޽� ������Ʈ
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USkeletalMeshComponent* SM_Dress;

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	class UMapCustomWidget* MapCustomWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
    TSubclassOf<UMapCustomWidget> C_WitchHouseMap;

	 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
    TSubclassOf<UMapCustomWidget> C_SubmarineMap;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
    TSubclassOf<UMapCustomWidget> C_BankMap;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
    TSubclassOf<UMapCustomWidget> C_SupermarketMap;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
    class UItemComponent* itemComponent;

	UFUNCTION(BlueprintCallable, Category="UI")
	UItemComponent* GetItemComponent() {return itemComponent;}
	
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	class UItemObject* itemObject;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	class UMapCustomWidget* CustomMapWidget;

    UFUNCTION(BlueprintCallable, Category = "CreateWidget")
	void CreateSelectedWidget();
	//
	UFUNCTION()
	void OnRep_Current_SkeletalMesh();
	
	UFUNCTION()
	void OnRep_Current_Accessories();
	
	UFUNCTION()
	void OnRep_Current_Skins();
	
	UFUNCTION()
	void OnRep_Current_Eyes();

	UFUNCTION()
	void OnRep_Current_Top();

	UFUNCTION()
	void OnRep_Current_Bottom();

	UFUNCTION()
	void OnRep_Current_Outer();

	UFUNCTION()
	void OnRep_Current_Dress();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	bool bGameIsStart = false;

	UPROPERTY(Replicated,EditAnywhere,BlueprintReadWrite)
	bool bCanActive;

	bool bCanOpenDoor = false;

	bool bIsDie = false;

	bool bIsGoodDriver = false;

	bool bIsBlack = false;

	float lerpTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	int32 endNum = 3;

	class AGimmick* g;

	FTimerHandle falloverT;

	FTimerHandle poorDriveT;

	FTimerHandle endManT;

	FTimerHandle ThunderT;

	APlayerController* con;

	UPROPERTY()
	TObjectPtr<AClearDoor> CheckDoor;

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void PlayerJump(const FInputActionValue& Value);

	void PlayerJumpEnd(const FInputActionValue& Value);

	void ActiveGimmick(const FInputActionValue& Value);

	void ShowEmo(const FInputActionValue& Value);

	void ShowScore(const FInputActionValue& Value);

	void CloseScore(const FInputActionValue& Value);

	void DeathCounting();

	void FadeInOut(bool bInOut);

	void BlackScreen();

	void SetPlayerPhysics();

	void Respawn(float delaytime = 3.0f);

	void GimmickSearch();

	void Death_Fallover();

	void Death_Homerun(FVector impactLoc, float power = (1000.0f));

	void Death_PoorDrive(bool bIsBestDriver);

	void Death_EndMan();

	void Death_Thunderclap();

	UFUNCTION(Server, Reliable)
	void ServerRPC_StartGetFinalScore();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_StartGetFinalScore();

	UFUNCTION()
	void Toggle_CharacterCustomization_Implementation();

    // Timer function
    UFUNCTION()
    void SetThirdPersonView();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_SetThirdPersonView();

	UFUNCTION(Server, Reliable)
	void ServerRPC_ActiveGimmick(ATestPlayer* aP);

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_ActiveGimmick(ATestPlayer* _aP);

	UFUNCTION(Server, Reliable)
	void ServerRPC_ClearStage();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_ClearStage();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerRPC_MoveStage();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_MoveStage(FVector moveLoc);

	UFUNCTION(Server, Reliable)
	void ServerRPC_FadeOut(bool _bInOut);

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_FadeOut(bool _bOut);

	UFUNCTION(Server, Reliable)
	void ServerRPC_ShowEmo(int32 emoNum);

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_ShowEmo(int32 _emoNum);

	UFUNCTION(Server, Reliable)
	void ServerRPC_SetPlayerPhysics(AActor* target, FVector pushDir = FVector(0,100,10));

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_SetPlayerPhysics(AActor* _target, FVector _pushDir);

	UFUNCTION(Server, Reliable)
	void ServerRPC_SetGActorLoc(AActor* MoveObj, FVector GetLoc, int32 ActiveNum);

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_SetGActorLoc(AActor* _MoveObj, FVector _GetLoc, int32 _ActiveNum);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
    // �ִϸ��̼� ��������Ʈ Ŭ����
    UPROPERTY(EditAnywhere, Category = "Animation")
    TSubclassOf<UAnimInstance> AnimationBlueprintClass;
};

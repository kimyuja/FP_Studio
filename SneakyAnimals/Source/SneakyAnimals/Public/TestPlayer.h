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

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite ,Category = Test)
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

    // ¾Ç¼¼»ç¸® ½ºÄÌ·¹Å» ¸Þ½¬ ÄÄÆ÷³ÍÆ®
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USkeletalMeshComponent* SM_Accessories;

	// »óÀÇ ½ºÄÌ·¹Å» ¸Þ½¬ ÄÄÆ÷³ÍÆ®
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USkeletalMeshComponent* SM_Top;

	// ÇÏÀÇ ½ºÄÌ·¹Å» ¸Þ½¬ ÄÄÆ÷³ÍÆ®
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USkeletalMeshComponent* SM_Bottom;

	// ¿ÜÅõ ½ºÄÌ·¹Å» ¸Þ½¬ ÄÄÆ÷³ÍÆ®
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USkeletalMeshComponent* SM_Outer;

	// µå·¹½º ½ºÄÌ·¹Å» ¸Þ½¬ ÄÄÆ÷³ÍÆ®
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USkeletalMeshComponent* SM_Dress;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	class UMapCustomWidget* CustomMapWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
    TSubclassOf<UMapCustomWidget> customMapWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	class UNewGridWidget* newGridWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
    TSubclassOf<UNewGridWidget> newGridWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
    class UItemComponent* itemComponent;

	UFUNCTION(BlueprintCallable, Category="UI")
	UItemComponent* GetItemComponent() {return itemComponent;}
	
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	class UItemObject* itemObject;


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
	int32 endNum = 1;

	int32 curStageNum = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	bool bIsCustomEnd = false;

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

	UFUNCTION()
	void ClearStage();

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
	void ServerRPC_SetGActorLocAndActiveNum(AActor* MoveObj, int32 ActiveNum);

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_SetGActorLocAndActiveNum(AActor* _MoveObj, int32 _ActiveNum);
	
	UFUNCTION(Server, Reliable)
	void ServerRPC_SetGActorRot(AActor* MoveObj, FRotator GetRot);

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_SetGActorRot(AActor* _MoveObj, FRotator _GetRot);

	UFUNCTION(Server, Reliable)
	void ServerRPC_SetGActorLoc(AActor* MoveObj, FVector GetLoc);

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_SetGActorLoc(AActor* _MoveObj, FVector _GetLoc);

	UFUNCTION(Server, Reliable)
	void ServerRPC_RespawnPlayer(float dTime);

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_RespawnPlayer(float _dTime);

	// --------------KYJ----------------
	UFUNCTION(Client, Reliable)
	void ClientRPC_Init_Appearance();
	
	UFUNCTION(Server, Reliable)
	void ServerRPC_Update_Character(USkeletalMesh* _mesh);

	UFUNCTION(Server, Reliable)
	void ServerRPC_Update_Skins(UMaterial* mat);

	UFUNCTION(Server, Reliable)
	void ServerRPC_Update_Eyes(UMaterial* mat);
	
	UFUNCTION(Server, Reliable)
	void ServerRPC_Update_Accessory(USkeletalMesh* _mesh);

	UFUNCTION(Server, Reliable)
	void ServerRPC_Update_Top(USkeletalMesh* _mesh);

	UFUNCTION(Server, Reliable)
	void ServerRPC_Update_Bottom(USkeletalMesh* _mesh);

	UFUNCTION(Server, Reliable)
	void ServerRPC_Update_Outer(USkeletalMesh* _mesh);

	UFUNCTION(Server, Reliable)
	void ServerRPC_Update_Dress(USkeletalMesh* _mesh);
	
	UFUNCTION(Client, Reliable)
	void ClientRPC_Update_PlayerNum_PlayerShowNum(int32 _PlayerNum, int32 _PlayerShowNum);
	UFUNCTION(Server, Reliable)
	void ServerRPC_Update_PlayerNum_PlayerShowNum(int32 _PlayerNum, int32 _PlayerShowNum);

	void Get_Player_Appearance();
	void Get_Character();
	void Get_Skins();
	void Get_Eyes();
	void Get_Accessory();
	void Get_Top();
	void Get_Bottom();
	void Get_Outer();
	void Get_Dress();

	// --------------------------------

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
    // ¾Ö´Ï¸ÞÀÌ¼Ç ºí·çÇÁ¸°Æ® Å¬·¡½º
    UPROPERTY(EditAnywhere, Category = "Animation")
    TSubclassOf<UAnimInstance> AnimationBlueprintClass;
};

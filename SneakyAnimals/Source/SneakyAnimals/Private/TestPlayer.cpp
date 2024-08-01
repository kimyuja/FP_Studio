
// Fill out your copyright notice in the Description page of Project Settings.


#include "TestPlayer.h"
#include "Engine.h"
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h>
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/SpringArmComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Camera/CameraComponent.h>
#include "Gimmick.h"
#include "WH_BookshelfGimmick.h"
#include "WH_BroomstickGimmick.h"
#include "WH_WitchCauldronGimmick.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Camera/PlayerCameraManager.h>
#include "ClearDoor.h"
#include "WH_PotionGimmick.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>
#include "SM_PeriscopeGimmick.h"
#include "UserEmoticon.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/WidgetComponent.h>
#include "SM_PressButtonGimmick.h"
#include "SM_ComputerGimmick.h"
#include "GM_Lobby.h"
#include "W_StageClear.h"
#include "SAModeBase.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Blueprint/UserWidget.h>
#include <../../../../../../../Source/Runtime/Engine/Public/Net/UnrealNetwork.h>
#include "SAGameStateBase.h"
#include "ItemComponent.h"
#include "MapCustomWidget.h"
#include "NewGridWidget.h"
#include "W_CustomMap.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Camera/CameraActor.h>
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include "W_InGameUI.h"
#include "UnderTheSea.h"
#include "Gimmick.h"
#include "SM_WhistleGimmick.h"
#include "SP_CartGimmick.h"
#include "SP_CleanerGimmick.h"
#include "SP_ShowcaseGimmick.h"
#include "SP_BottleGimmick.h"
#include "BS_HandleGimmick.h"
#include "BS_GoldBarGimmick.h"
#include "BS_SwitchGimmick.h"
#include "BS_LaserGimmick.h"
#include "FL_General.h"

// Sets default values
ATestPlayer::ATestPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90.0f));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	GetMesh()->SetOwnerNoSee(true);

	// 악세사리 스켈레탈 메쉬 컴포넌트를 초기화하고 자식으로 설정
	SM_Accessories = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SM_Accessories"));
	SM_Top = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SM_Top"));
	SM_Bottom = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SM_Bottom"));
	SM_Outer = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SM_Outer"));
	SM_Dress = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SM_Dress"));
	SM_Accessories->SetupAttachment(GetMesh());
	SM_Top->SetupAttachment(GetMesh());
	SM_Bottom->SetupAttachment(GetMesh());
	SM_Outer->SetupAttachment(GetMesh());
	SM_Dress->SetupAttachment(GetMesh());

	// 애니메이션 블루프린트를 로드하고 설정
	/*static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimBlueprint(TEXT("/Game/Characters/Mannequins/Animations/ABP_Quinn.ABP_Quinn_C"));
	if (AnimBlueprint.Succeeded())
	{
		AnimationBlueprintClass = AnimBlueprint.Object->GeneratedClass;
		SM_Accessories->SetAnimInstanceClass(AnimationBlueprintClass);
		SM_Top->SetAnimInstanceClass(AnimationBlueprintClass);
		SM_Bottom->SetAnimInstanceClass(AnimationBlueprintClass);
		SM_Outer->SetAnimInstanceClass(AnimationBlueprintClass);
		SM_Dress->SetAnimInstanceClass(AnimationBlueprintClass);
	}*/
	

	cameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	cameraBoom->SetupAttachment(GetMesh());
	cameraBoom->SetRelativeLocation(FVector(0, 0, 170));
	cameraBoom->TargetArmLength = 0;
	cameraBoom->bUsePawnControlRotation = true;

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(cameraBoom, USpringArmComponent::SocketName);
	camera->bUsePawnControlRotation = false;

	customCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Custom Camera"));
	customCamera->SetupAttachment(GetMesh());
	customCamera->SetRelativeLocation(FVector(0, 500, 170));
	customCamera->SetRelativeRotation(FRotator(0,-90,0));

	emoticonUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("Emoticon UI"));
	emoticonUI->SetupAttachment(camera);
	emoticonUI->SetRelativeLocation(FVector(100, 0, 0));
	emoticonUI->SetDrawSize(FVector2D(100, 100));
	emoticonUI->SetWidgetClass(emoUI);

	/*bReplicates = true;
	bNetLoadOnClient = true;
	SetReplicateMovement(true);*/

	itemComponent = CreateDefaultSubobject<UItemComponent>(TEXT("itemComponent"));

}

// Called when the game starts or when spawned
void ATestPlayer::BeginPlay()
{
	Super::BeginPlay();

	bReplicates = true;
	bNetLoadOnClient = true;
	SetReplicateMovement(true);

	if (!itemComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("itemComponent is null in testplayer"));
		return;
	}
	

	
	clearUI = Cast<UW_StageClear>(CreateWidget(GetWorld(), stageClearUI));
	
	if (clearUI)
	{
		clearUI->SetOwningPlayer(GetWorld()->GetFirstPlayerController());
		clearUI->AddToViewport(1);
		clearUI->SetVisibility(ESlateVisibility::Hidden);
	}
	gameState = Cast<ASAGameStateBase>(GetWorld()->GetGameState());

	if (gameState)
	{
		UE_LOG(LogTemp, Warning, TEXT("Game Mode Set"));
		if (IsLocallyControlled())
		{
			gameState->SetClearInstance();
		}
		gameState->SetStageStart();
		gameState->SetPlayerNum();
		//gameState->SetEndNum();
	}

	mainUI = Cast<UW_InGameUI>(CreateWidget(GetWorld(), mainUIF));
	if (IsLocallyControlled())
	{
		mainUI = Cast<UW_InGameUI>(CreateWidget(GetWorld(), mainUIF));
	}
	if (mainUI)
	{
		//if (IsLocallyControlled())
		//{
			mainUI->AddToViewport(2);
			if (HasAuthority())
			{
				UE_LOG(LogTemp, Warning, TEXT("server!!!!!!!!"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("client!!!!!!!!"));
			}
			mainUI->SetTimerShow(false);
		//}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Main Fail!!!!!!!!"));
	}

	if (emoUI)
	{
		myEmoUI = Cast<UUserEmoticon>(CreateWidget(GetWorld(), emoUI));
		if (myEmoUI)
		{
			myEmoUI->AddToViewport(3);
		}
	}

	// 게임 모드에 따라서 카메라 위치를 1인칭, 3인칭으로 바꾸기
	// Lobby_Level : 3인칭, 이유 : 자신의 캐릭터 커스터마이징이 바뀌는 게 보여야 해서
	// Game_Level : 1인칭, 이유 : 기믹이랑 상호작용하려면 1인칭이 편해서
	AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
	if (GameMode)
	{
		AGM_Lobby* gm_lobby = Cast<AGM_Lobby>(GameMode);
		if (gm_lobby)
		{
			FTimerHandle cameraT;
			GetWorldTimerManager().SetTimer(cameraT,this, &ATestPlayer::SetThirdPersonView, 1.0, false);
		}
		else
		{
			// 게임 맵(1인칭)의 경우 본인의 메쉬 다 본인이 안 보이게 하기 -> 시야 확보
			SM_Accessories->SetOwnerNoSee(true);
			SM_Top->SetOwnerNoSee(true);
			SM_Bottom->SetOwnerNoSee(true);
			SM_Outer->SetOwnerNoSee(true);
			SM_Dress->SetOwnerNoSee(true);
		}
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(imc_testmove, 0);
		}
	}
}

void ATestPlayer::Toggle_CharacterCustomization_Implementation()
{
	//UE_LOG(LogTemp, Warning, TEXT("toggle test"));
}

void ATestPlayer::SetThirdPersonView()
{
	MultiRPC_SetThirdPersonView();
}

// Called every frame
void ATestPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	lerpTime += DeltaTime;
	if (lerpTime > 1)
	{
		GetWorldTimerManager().PauseTimer(falloverT);
		GetWorldTimerManager().PauseTimer(poorDriveT);
		GetWorldTimerManager().PauseTimer(endManT);
		//UE_LOG(LogTemp, Warning, TEXT("???"));
	}
	if (lerpTime > 4.0)
	{
		GetWorldTimerManager().PauseTimer(ThunderT);
		//UE_LOG(LogTemp, Warning, TEXT("??????"));
	}

	if (bCanActive)
	{
		GimmickSearch();
	}

	if (clearUI && clearUI->curtime > 5 && HasAuthority())
	{
		//curStageNum++;
		////gameState->stageNum = curStageNum;
		//respawnLoc = gameState->stageLoc[gameState->stageNum];
		UE_LOG(LogTemp, Warning, TEXT("!@!@!@!@!@!@!@!@!@!@!@"));
		ServerRPC_MoveStage();
		clearUI->curtime = 0;
	}
}

// Called to bind functionality to input
void ATestPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhancedinput = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (enhancedinput) {
		// 이동
		enhancedinput->BindAction(ia_move, ETriggerEvent::Triggered, this, &ATestPlayer::Move);
		// 점프
		enhancedinput->BindAction(ia_jump, ETriggerEvent::Started, this, &ATestPlayer::PlayerJump);
		enhancedinput->BindAction(ia_jump, ETriggerEvent::Completed, this, &ATestPlayer::PlayerJumpEnd);
		// 시야회전
		enhancedinput->BindAction(ia_look, ETriggerEvent::Triggered, this, &ATestPlayer::Look);
		enhancedinput->BindAction(ia_activeG, ETriggerEvent::Started, this, &ATestPlayer::ActiveGimmick);

		enhancedinput->BindAction(ia_emo1, ETriggerEvent::Started, this, &ATestPlayer::ShowEmo);

		enhancedinput->BindAction(ia_currentScore, ETriggerEvent::Started, this, &ATestPlayer::ShowScore);
		enhancedinput->BindAction(ia_currentScore, ETriggerEvent::Completed, this, &ATestPlayer::CloseScore);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Fail"));
	}

}



void ATestPlayer::CreateSelectedWidget()
{
	if (GetWorld()->GetGameState()->PlayerArray.Num() < endNum) {
		// 현재 플레이어가 아직 다 안 들어왔으면
		FTimerHandle t;
		GetWorld()->GetTimerManager().SetTimer(t, [&](){
			ATestPlayer::CreateSelectedWidget();
			return;
		}, 0.5f, false);
	}
	CustomMapWidget = nullptr;

	CustomMapWidget = Cast<UMapCustomWidget>(CreateWidget(GetWorld(), customMapWidgetClass));

	// playerNum = 1;
	
	CustomMapWidget->playerRandNum = playerNum;
	
	if (CustomMapWidget)
	{
		CustomMapWidget->AddToViewport();
		// CustomMapWidget->CustomMapGridWidget->InitializeWidget(160.f);
		
	}
}

void ATestPlayer::OnRep_Current_SkeletalMesh()
{
	if (Current_SkeletalMesh->IsValidLowLevelFast())
	{
		GetMesh()->SetSkinnedAssetAndUpdate(Current_SkeletalMesh);

		GetMesh()->SetVisibility(true);
		SM_Outer->SetVisibility(false);
	}
}

void ATestPlayer::OnRep_Current_Accessories()
{
	if (Current_Accessories->IsValidLowLevelFast())
	{
		SM_Accessories->SetSkinnedAssetAndUpdate(Current_Accessories);
	}
	else
	{
		SM_Accessories->SetSkinnedAssetAndUpdate(nullptr);
	}
}

void ATestPlayer::OnRep_Current_Skins()
{
	// Material Slot 이름을 가져옴
	TArray<FName> MaterialSlotNames = GetMesh()->GetMaterialSlotNames();

	// 확인할 Material Slot 이름들
	FName SlotName1(TEXT("M_Bull_Dandelion_BODY"));
	FName SlotName2(TEXT("M_BUNNY_JESSICA_BODY"));
	FName SlotName3(TEXT("standardSurface2"));
	FName SlotName4(TEXT("M_TURTLE_BODY"));

	// Material Slot이 존재하는지 확인하고 설정
	for (int32 i = 0; i < MaterialSlotNames.Num(); ++i)
	{
		if (MaterialSlotNames[i] == SlotName1 || MaterialSlotNames[i] == SlotName2 || MaterialSlotNames[i] == SlotName3 || MaterialSlotNames[i] == SlotName4)
		{
			GetMesh()->SetMaterialByName(MaterialSlotNames[i], Current_Skins);
		}
	}
}

void ATestPlayer::OnRep_Current_Eyes()
{
	// Material Slot 이름을 가져옴
	TArray<FName> MaterialSlotNames = GetMesh()->GetMaterialSlotNames();

	// 확인할 Material Slot 이름들
	FName SlotName1(TEXT("pasted__M_Bull_Dandelion_EYES"));
	FName SlotName2(TEXT("M_BUNNY_JESSICA_EYES"));
	FName SlotName3(TEXT("pasted__standardSurface3"));
	FName SlotName4(TEXT("M_TURTLE_EYES"));

	// Material Slot이 존재하는지 확인하고 설정
	for (int32 i = 0; i < MaterialSlotNames.Num(); ++i)
	{
		if (MaterialSlotNames[i] == SlotName1 || MaterialSlotNames[i] == SlotName2 || MaterialSlotNames[i] == SlotName3 || MaterialSlotNames[i] == SlotName4)
		{
			GetMesh()->SetMaterialByName(MaterialSlotNames[i], Current_Eyes);
		}
	}
}

void ATestPlayer::OnRep_Current_Top()
{
	if (Current_Top->IsValidLowLevelFast())
	{
		SM_Top->SetSkinnedAssetAndUpdate(Current_Top);

		GetMesh()->SetVisibility(true);
		SM_Outer->SetVisibility(false);
	}
	else
	{
		SM_Top->SetSkinnedAssetAndUpdate(nullptr);
	}
}

void ATestPlayer::OnRep_Current_Bottom()
{
	if (Current_Bottom->IsValidLowLevelFast())
	{
		SM_Bottom->SetSkinnedAssetAndUpdate(Current_Bottom);

		GetMesh()->SetVisibility(true);
		SM_Outer->SetVisibility(false);
	}
	else
	{
		SM_Bottom->SetSkinnedAssetAndUpdate(nullptr);
	}
}

void ATestPlayer::OnRep_Current_Outer()
{
	if (Current_Outer->IsValidLowLevelFast())
	{
		SM_Outer->SetSkinnedAssetAndUpdate(Current_Outer);

		GetMesh()->SetVisibility(false);
		SM_Outer->SetVisibility(true);
	}
}

void ATestPlayer::OnRep_Current_Dress()
{
	if (Current_Dress->IsValidLowLevelFast())
	{
		SM_Dress->SetSkinnedAssetAndUpdate(Current_Dress);

		GetMesh()->SetVisibility(true);
		SM_Outer->SetVisibility(false);
	}
	else
	{
		SM_Dress->SetSkinnedAssetAndUpdate(nullptr);
	}
}

void ATestPlayer::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ATestPlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X / 2.0f);
		AddControllerPitchInput(-LookAxisVector.Y / 2.0f);
	}
}

void ATestPlayer::PlayerJump(const FInputActionValue& Value)
{
	Jump();
}

void ATestPlayer::PlayerJumpEnd(const FInputActionValue& Value)
{
	StopJumping();
}

void ATestPlayer::ActiveGimmick(const FInputActionValue& Value)
{
	if (!bCanActive)
	{
		return;
	}
	/*if (bCanActive)
	{
		bCanActive = false;
		UE_LOG(LogTemp, Warning, TEXT("Fail %d"), bCanActive);
		int32 key = g->OnMyActive(this);
		if (key == 2)
		{
			bCanOpenDoor = true;
			gameMode->bOnGame = false;
			clearUI->SetWidgetState();
			clearUI->SetVisibility(ESlateVisibility::Visible);
		}
	}*/
	ATestPlayer* target = this;

	//UE_LOG(LogTemp, Warning, TEXT("%f, %f, %f"), target->GetActorLocation().X, target->GetActorLocation().Y, target->GetActorLocation().Z);
	if (IsLocallyControlled())
	{
		ServerRPC_ActiveGimmick(target);
	}
}

void ATestPlayer::ShowEmo(const FInputActionValue& Value)
{
	float inputNum = Value.Get<float>();
	ServerRPC_ShowEmo(inputNum);
	myEmoUI->ShowEmoticon(inputNum - 1);
}

void ATestPlayer::ShowScore(const FInputActionValue& Value)
{
	if (!bGameIsStart)
	{
		return;
	}
	mainUI->SetOverlayShow(true);
}

void ATestPlayer::CloseScore(const FInputActionValue& Value)
{
	if (!bGameIsStart)
	{
		return;
	}
	mainUI->SetOverlayShow(false);
}

void ATestPlayer::DeathCounting()
{
	gameState->SetDeathCountUp(playerNum);
}

void ATestPlayer::FadeInOut(bool bInOut)
{
	ServerRPC_FadeOut(bInOut);
	/*if (!bIsDie || !IsLocallyControlled())
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Player %d FadeInOut"), playerNum);
	APlayerCameraManager* cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(),0);
	if (bInOut)
	{
		cameraManager->StartCameraFade(0,1.0f, 1.0f, FColor::Black, false, true);
		bIsBlack = true;
	}
	else
	{
		cameraManager->StartCameraFade(1.0f, 0, 1.5f, FColor::Black);
		bIsBlack = false;
	}*/
}

void ATestPlayer::BlackScreen()
{
	APlayerCameraManager* cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	cameraManager->StartCameraFade(0, 1.0f, 0.1f, FColor::Black, false, true);
	//bIsBlack = true;
}

void ATestPlayer::SetPlayerPhysics()
{
	GetCapsuleComponent()->SetSimulatePhysics(true);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ATestPlayer::Respawn(float delaytime)
{
	if (!bIsBlack)
	{
		ServerRPC_FadeOut(true);
	}
	UE_LOG(LogTemp, Warning, TEXT("Player %d Respawn"), playerNum);
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	/*FTimerHandle pT;
	GetWorldTimerManager().SetTimer(pT, [&]() {
		GetMesh()->SetSimulatePhysics(false);
		}, 1.0, false, 2.0);*/
	FTimerHandle respawnT;
	GetWorldTimerManager().SetTimer(respawnT, [&](){
		GetMesh()->SetSimulatePhysics(false);
		ServerRPC_FadeOut(false);
		GetCapsuleComponent()->SetSimulatePhysics(false);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		GetCapsuleComponent()->SetRelativeRotation(FRotator(0, 0, 0));
		GetMesh()->SetRelativeScale3D(FVector(1.0, 1.0, 1.0));
		GetMesh()->SetRelativeRotation(FRotator(0,-90,0));
		SetActorLocation(respawnLoc);
		//for (TActorIterator<ATestPlayer> it(GetWorld()); it; ++it)
		//{
		//	//it->GetMesh()->SetSimulatePhysics(false);
		//	cameraBoom->SetRelativeLocation(FVector(0, 0, 170));
		//}
		cameraBoom->SetRelativeLocation(FVector(0,0, 170));
		//FadeInOut(false);
		bIsDie = false;
		bIsBlack = false;
		bCanActive = true;
		GetWorldTimerManager().ClearAllTimersForObject(this);
	}, 1.0, false, delaytime);
	
}

void ATestPlayer::GimmickSearch()
{
	if (bIsDie)
	{
		return;
	}
	FHitResult hitInfo;
	FVector start = camera->GetComponentLocation();
	FVector end = camera->GetComponentLocation() + camera->GetForwardVector() * 300.0;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);

	bool result = GetWorld()->LineTraceSingleByObjectType(hitInfo, start, end, ECC_GameTraceChannel1 , params);
	if (result)
	{
		g = Cast<AGimmick>(hitInfo.GetActor());
		if (g)
		{
			g->bCanActive = true;
		}
		//DrawDebugLine(GetWorld(), start, hitInfo.ImpactPoint, FColor::Red, false, -1, 0, 1);
		//DrawDebugLine(GetWorld(), hitInfo.ImpactPoint, end, FColor::Green, false, -1, 0, 1);
	}
	else
	{
		//DrawDebugLine(GetWorld(), start, end, FColor::Red, false, -1, 0, 1);
	}
}

void ATestPlayer::Death_Fallover()
{
	bIsDie = true;
	lerpTime = 0;
	bCanActive = false;
	GetWorldTimerManager().SetTimer(falloverT, [&]()
		{
			float eye = FMath::Lerp(250.0, 0, lerpTime);
			float tall = FMath::Lerp(1.0, 0.3, lerpTime);
			cameraBoom->SetRelativeLocation(FVector(0,0,eye));
			GetMesh()->SetRelativeScale3D(GetMesh()->GetRelativeScale3D() * FVector(1,1,tall));
		}, 0.03f, true, 0);
	Respawn();
}

void ATestPlayer::Death_Homerun(FVector impactLoc, float power)
{
	bCanActive = false;
	bIsDie = true;
	//GetMesh()->SetSimulatePhysics(true);
	//GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetSimulatePhysics(true);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->AddImpulse(impactLoc * power, TEXT(""), true);
	//GetMesh()->AddImpulse(impactLoc * power, TEXT(""), true);
	Respawn(5.0);
}

void ATestPlayer::Death_PoorDrive(bool bIsBestDriver)
{
	//UE_LOG(LogTemp, Warning, TEXT("%d"), bIsBestDriver);
	bIsGoodDriver = bIsBestDriver;
	bCanActive = false;
	lerpTime = 0;
	for (TActorIterator<AClearDoor> it(GetWorld()); it; ++it)
	{

		if (it->roomType == 0)
		{
			AClearDoor* tempDoor = *it;
			CheckDoor = tempDoor;
			FString DoorName = CheckDoor->GetName();
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, DoorName);
		}
	}
	//UE_LOG(LogTemp, Warning, TEXT("%f, %f, %f"), burstRot.Pitch, burstRot.Roll, burstRot.Yaw);
	GetWorldTimerManager().SetTimer(poorDriveT, [&]()
		{
			float speed = FMath::Lerp(0, FVector::Dist(GetActorLocation(), CheckDoor->GetActorLocation()), lerpTime);
			if (bIsGoodDriver)
			{
				if (FVector::Dist(GetActorLocation(), CheckDoor->GetActorLocation()) < 100.0)
				{
					CheckDoor->SetActorLocation(FVector(0, 0, -50000.0f));
					CheckDoor->object->SetVisibility(false);
					return;
				}
				else
				{
					FVector burstLoc = (CheckDoor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
					FRotator burstRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CheckDoor->GetActorLocation());
					//UE_LOG(LogTemp, Warning, TEXT("%f, %f, %f"), burstLoc.X, burstLoc.Y, burstLoc.Z);
					//Controller->SetControlRotation(FRotator(0, burstRot.Yaw, burstRot.Roll));
					//UE_LOG(LogTemp, Warning, TEXT("%f, %f, %f"), burstRot.Pitch, burstRot.Yaw, burstRot.Roll);
					//SetActorRotation(burstRot);
					SetActorLocation(GetActorLocation() + speed * burstLoc, true);
				}
			}
			else
			{
				speed = FMath::Lerp(0, 500, lerpTime);
				UE_LOG(LogTemp, Warning, TEXT("Drive Fail"));
				SetActorLocation(GetActorLocation() + speed * GetActorForwardVector(), true);
			}
		}, 0.03f, true, 0);
	if (!bIsGoodDriver)
	{
		bIsDie = true;
		Respawn(5.0);
		//ServerRPC_FadeOut(true);
	}
}

void ATestPlayer::Death_EndMan()
{
	bIsDie = true;
	bCanActive = false;
	lerpTime = 0;
	GetWorldTimerManager().SetTimer(endManT, [&]()
		{
			float eye = FMath::Lerp(90.0, 0, lerpTime);
			float size = FMath::Lerp(1.0, 0.3, lerpTime);
			cameraBoom->SetRelativeLocation(FVector(0, 0, eye));
			GetMesh()->SetRelativeScale3D(GetMesh()->GetRelativeScale3D() * FVector(size, size, size));
		}, 0.03f, true, 0);
	Respawn();
}

void ATestPlayer::Death_Thunderclap()
{
	//bIsDie = true;
	bCanActive = false;
	if (!IsLocallyControlled())
	{
		return;
	}
	lerpTime = 0;
	GetWorldTimerManager().SetTimer(ThunderT, [&]()
	{
		APlayerCameraManager* cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
		cameraManager->StartCameraFade(0, 1.0f, 0.25f, FColor::White);
	}, 0.25, true , 0);
	bIsBlack = true;
	Respawn(5.0);
}

void ATestPlayer::ServerRPC_StartGetFinalScore_Implementation()
{
	MultiRPC_StartGetFinalScore();
}

void ATestPlayer::MultiRPC_StartGetFinalScore_Implementation()
{
	con = GetWorld()->GetFirstPlayerController();
	DisableInput(con);
	SetActorRotation(FRotator(0, 90, 0));
	SetActorLocation(FVector(-1000, 0, 0));
	for (TActorIterator<ACameraActor> it(GetWorld()); it; ++it)
	{
		UE_LOG(LogTemp, Warning, TEXT("Find"));
		if (it->ActorHasTag(TEXT("Final")))
		{
			UE_LOG(LogTemp, Warning, TEXT("Find"));
			con->SetViewTargetWithBlend(*it, 0.2f);
			//camera->SetRelativeLocation(camera->GetRelativeLocation() + it->GetActorLocation());
		}
	}

	if (!IsLocallyControlled())
	{
		CreateWidget(GetWorld(),voteUI)->AddToViewport(0);
	}
}

void ATestPlayer::MultiRPC_SetThirdPersonView_Implementation()
{
	// 3인칭 설정
	cameraBoom->TargetArmLength = 300.0f;
	cameraBoom->SetRelativeLocation(FVector(0, 0, 85));
	cameraBoom->bUsePawnControlRotation = false;
	camera->bUsePawnControlRotation = true;
	cameraBoom->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// 본인 보이게 설정
	GetMesh()->SetOwnerNoSee(false);
}

void ATestPlayer::ServerRPC_ActiveGimmick_Implementation(ATestPlayer* aP)
{
	//UE_LOG(LogTemp, Warning, TEXT("%f, %f, %f"), aP->GetActorLocation().X, aP->GetActorLocation().Y, aP->GetActorLocation().Z);
	if (clearUI->bIsClear)
	{
		mainUI->SetTimerShow(false);
	}
	MultiRPC_ActiveGimmick(aP);
}

void ATestPlayer::MultiRPC_ActiveGimmick_Implementation(ATestPlayer* _aP)
{
	if (!clearUI)
	{
		return;
	}
	if (clearUI->bIsClear)
	{
		mainUI->SetTimerShow(false);
	}
	if (bCanActive && g->bCanActive)
	{
		//UE_LOG(LogTemp, Warning, TEXT("aP = %f, %f, %f"), _aP->GetActorLocation().X, _aP->GetActorLocation().Y, _aP->GetActorLocation().Z);
		//UE_LOG(LogTemp, Warning, TEXT("P = %f, %f, %f"), GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z);
		bCanActive = false;
		int32 key = g->OnMyActive(_aP);
		/*if (key == 2)
		{
			ClearStage();
			UE_LOG(LogTemp, Warning, TEXT("1111111111111111111!!!!!!!!"));
		}*/
		g = nullptr;
	}
}

void ATestPlayer::ClearStage()
{
	ServerRPC_ClearStage();
}

void ATestPlayer::ServerRPC_ClearStage_Implementation()
{
	MultiRPC_ClearStage();
	//curStageNum++;
	//UE_LOG(LogTemp, Warning, TEXT("%d : %d"), playerNum, curStageNum);
	/*for (TActorIterator<ATestPlayer> p(GetWorld()); p; ++p)
	{
		if (IsLocallyControlled())
		{
			p->curStageNum++;
			UE_LOG(LogTemp, Warning, TEXT("%d : %d"), p->playerNum, p->curStageNum);
		}
	}*/
}

void ATestPlayer::MultiRPC_ClearStage_Implementation()
{
	//UE_LOG(LogTemp, Warning, TEXT("22222222222222222222!!!!!!!!"));
	if (gameState->stageNum == 3)
	{
		for (TActorIterator<AUnderTheSea> sea(GetWorld()); sea; ++sea)
		{
			sea->bisclear = true;
		}
	}
	
	for (TActorIterator<ATestPlayer> p(GetWorld()); p; ++p)
	{
		if (p->mainUI)
		{
			p->mainUI->SetTimerShow(false);
		}
	}
	bCanOpenDoor = true;
	gameState->bOnGame = false;
	if (HasAuthority())
	{
		//UE_LOG(LogTemp, Warning, TEXT("server End!!!!!!!!"));
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("client End!!!!!!!!"));
	}
	FTimerHandle clearT;
	GetWorldTimerManager().SetTimer(clearT, [&]() {
		clearUI->PlayAnimation(clearUI->GameEnd);
		clearUI->SetWidgetState();
		clearUI->bIsClear = true;
		clearUI->SetVisibility(ESlateVisibility::Visible);
		}, 1.0, false, 2.0);
}

void ATestPlayer::ServerRPC_MoveStage_Implementation()
{
	//for (TActorIterator<ATestPlayer> p(GetWorld()); p; ++p)
	//{
	//	//p->respawnLoc = moveLoc;
	//	p->bGameIsStart = true;
	//	clearUI->SetVisibility(ESlateVisibility::Hidden);
	//	UE_LOG(LogTemp, Warning, TEXT("Clean"));
	//}
	UE_LOG(LogTemp, Warning, TEXT(" cur %d : %d"), playerNum, curStageNum);
	for (TActorIterator<ATestPlayer> p(GetWorld()); p; ++p)
	{
		p->curStageNum++;
		UE_LOG(LogTemp, Warning, TEXT("%d : %d"), p->playerNum, p->curStageNum);
		/*if (IsLocallyControlled())
		{
			p->curStageNum++;
			UE_LOG(LogTemp, Warning, TEXT("%d : %d"), p->playerNum, p->curStageNum);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("FAil %d : %d"), p->playerNum, p->curStageNum);
		}*/
	}

	/*if (curStageNum == 0)
	{
		curStageNum++;
	}*/

	MultiRPC_MoveStage(gameState->stageLoc[curStageNum-1], curStageNum);
	//UE_LOG(LogTemp, Warning, TEXT("Stage %d (ServerRPC_MoveStage)"), gameState->stageNum);
	//if (HasAuthority() && IsLocallyControlled())
	//{
	//	/*for (TActorIterator<ATestPlayer> p(GetWorld()); p; ++p)
	//	{
	//		p->curStageNum++;
	//		p->gameState->stageNum = curStageNum;
	//		p->respawnLoc = gameState->stageLoc[gameState->stageNum -1];
	//		p->bGameIsStart = true;
	//	}*/
	//	curStageNum++;
	//	gameState->stageNum = curStageNum;
	//	//respawnLoc = gameState->stageLoc[gameState->stageNum];
	//	UE_LOG(LogTemp, Warning, TEXT("Check : %d"), check);
	//}
}

void ATestPlayer::MultiRPC_MoveStage_Implementation(FVector moveLoc, int32 _stageNum)
{
	/*if (HasAuthority())
	{
		curStageNum++;
		gameState->stageNum = curStageNum;
		respawnLoc = gameState->stageLoc[gameState->stageNum];
		UE_LOG(LogTemp, Warning, TEXT("Check : %d"), check);
	}*/
	respawnLoc = moveLoc;
	gameState->stageNum = _stageNum;
	bGameIsStart = true;
	if (HasAuthority())
	{
		//curStageNum++;
		//gameState->stageNum = curStageNum;
		UE_LOG(LogTemp, Warning, TEXT("Server Stage Num : %d"), gameState->stageNum);
	}
	else
	{
		/*gameState->stageNum = curStageNum;*/
		UE_LOG(LogTemp, Warning, TEXT("Client Stage Num : %d"), gameState->stageNum);
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Show???????????????"));
	gameState->MoveNextStage(moveLoc);
	for (TActorIterator<ATestPlayer> p(GetWorld()); p; ++p)
	{
		//p->respawnLoc = moveLoc;
		p->bGameIsStart = true;
		clearUI->SetVisibility(ESlateVisibility::Hidden);
	}
	UE_LOG(LogTemp, Warning, TEXT("MOOOOOOOOOOOOOOOOOOOOOOOOOOve        %d"), gameState->stageNum);
	if (gameState->stageNum > 4)
	{
		CreateWidget(GetWorld(), voteUI)->AddToViewport(0);

		if (!HasAuthority())
		{
			// On the Client
			ServerRPC_StartGetFinalScore();
		}
		else
		{
			// On the Server
			MultiRPC_StartGetFinalScore();
		}
	}
	else if (gameState->stageNum >= 1)
	{
		//bGameIsStart = true;
		for (TActorIterator<ATestPlayer> p(GetWorld()); p; ++p)
		{
			if (p->mainUI)
			{
				p->mainUI->maxtime = 180.0;
				p->mainUI->SetTimerShow(true);
				UE_LOG(LogTemp, Warning, TEXT("Show!!!!!!!!!!!!!!!!!!!"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("MangGam!!!!!!!!!!!!!!!!!!!"));
			}
		}
	}
}

void ATestPlayer::ServerRPC_FadeOut_Implementation(bool _bInOut)
{
	MultiRPC_FadeOut(_bInOut);
}

void ATestPlayer::MultiRPC_FadeOut_Implementation(bool _bOut)
{
	if (!bIsDie || !IsLocallyControlled())
	{
		return;
	}
	//UE_LOG(LogTemp, Warning, TEXT("Player %d FadeInOut"), playerNum);
	APlayerCameraManager* cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	if (_bOut)
	{
		cameraManager->StartCameraFade(0, 1.0f, 1.0f, FColor::Black, false, true);
		UE_LOG(LogTemp, Warning, TEXT("Player %d FadeOut"), playerNum);
		bIsBlack = true;
	}
	else
	{
		cameraManager->StartCameraFade(1.0f, 0, 1.5f, FColor::Black);
		UE_LOG(LogTemp, Warning, TEXT("Player %d FadeIn"), playerNum);
		bIsBlack = false;
	}
}

void ATestPlayer::ServerRPC_ShowEmo_Implementation(int32 emoNum)
{
	MultiRPC_ShowEmo(emoNum);
}

void ATestPlayer::MultiRPC_ShowEmo_Implementation(int32 _emoNum)
{
	Cast<UUserEmoticon>(emoticonUI->GetWidget())->ShowEmoticon(_emoNum - 1);
}

void ATestPlayer::ServerRPC_SetPlayerPhysics_Implementation(AActor* target, FVector pushDir)
{
	UE_LOG(LogTemp, Warning, TEXT("Rock????"), playerNum);
	MultiRPC_SetPlayerPhysics(target, pushDir);
}

void ATestPlayer::MultiRPC_SetPlayerPhysics_Implementation(AActor* _target, FVector _pushDir)
{
	UE_LOG(LogTemp, Warning, TEXT("Rock"), playerNum);
	GetCapsuleComponent()->SetSimulatePhysics(true);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->AddImpulse(_pushDir, TEXT(""), true);
}

void ATestPlayer::ServerRPC_SetGActorLoc_Implementation(AActor* MoveObj, FVector GetLoc)
{
	MultiRPC_SetGActorLoc(MoveObj, GetLoc);
}

void ATestPlayer::MultiRPC_SetGActorLoc_Implementation(AActor* _MoveObj, FVector _GetLoc)
{
	_MoveObj->SetActorLocation(_GetLoc);
}

void ATestPlayer::ServerRPC_RespawnPlayer_Implementation(float dTime)
{
	MultiRPC_RespawnPlayer(dTime);
}

void ATestPlayer::MultiRPC_RespawnPlayer_Implementation(float _dTime)
{
	if (!bIsBlack)
	{
		ServerRPC_FadeOut(true);
	}
	UE_LOG(LogTemp, Warning, TEXT("Player %d Respawn"), playerNum);
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	/*FTimerHandle pT;
	GetWorldTimerManager().SetTimer(pT, [&]() {
		GetMesh()->SetSimulatePhysics(false);
		}, 1.0, false, 2.0);*/
	FTimerHandle respawnT;
	GetWorldTimerManager().SetTimer(respawnT, [&]() {
		GetMesh()->SetSimulatePhysics(false);
		ServerRPC_FadeOut(false);
		GetCapsuleComponent()->SetSimulatePhysics(false);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		GetCapsuleComponent()->SetRelativeRotation(FRotator(0, 0, 0));
		GetMesh()->SetRelativeScale3D(FVector(1.0, 1.0, 1.0));
		GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
		SetActorLocation(respawnLoc);
		//for (TActorIterator<ATestPlayer> it(GetWorld()); it; ++it)
		//{
		//	//it->GetMesh()->SetSimulatePhysics(false);
		//	cameraBoom->SetRelativeLocation(FVector(0, 0, 170));
		//}
		cameraBoom->SetRelativeLocation(FVector(0, 0, 170));
		//FadeInOut(false);
		bIsDie = false;
		bIsBlack = false;
		bCanActive = true;
		GetWorldTimerManager().ClearAllTimersForObject(this);
		}, 1.0, false, _dTime);
}

void ATestPlayer::ServerRPC_SetGActorRot_Implementation(AActor* MoveObj, FRotator GetRot)
{
	MultiRPC_SetGActorRot(MoveObj, GetRot);
}

void ATestPlayer::MultiRPC_SetGActorRot_Implementation(AActor* _MoveObj, FRotator _GetRot)
{
	_MoveObj->SetActorRotation(_GetRot);
}

void ATestPlayer::ServerRPC_SetGActorLocAndActiveNum_Implementation(AActor* MoveObj, int32 ActiveNum)
{
	MultiRPC_SetGActorLocAndActiveNum(MoveObj, ActiveNum);
}

void ATestPlayer::MultiRPC_SetGActorLocAndActiveNum_Implementation(AActor* _MoveObj, int32 _ActiveNum)
{
	newGridWidget = nullptr;

	newGridWidget = CreateWidget<UNewGridWidget>(GetWorld(), newGridWidgetClass);

	if (newGridWidget != nullptr)
	{
		// 마녀의집
		if (_MoveObj->IsA(AWH_BookshelfGimmick::StaticClass()))
		{
			for (TActorIterator<AWH_BookshelfGimmick> It(GetWorld()); It; ++It)
			{
				(*It)->Myactivetype = _ActiveNum;
			}
		}
		else if (_MoveObj->IsA(AWH_BroomstickGimmick::StaticClass()))
		{
			for (TActorIterator<AWH_BroomstickGimmick> It(GetWorld()); It; ++It)
			{
				(*It)->Myactivetype = _ActiveNum;
			}
		}
		else if (_MoveObj->IsA(AWH_PotionGimmick::StaticClass()))
		{
			for (TActorIterator<AWH_PotionGimmick> It(GetWorld()); It; ++It)
			{
				(*It)->Myactivetype = _ActiveNum;
			}
		}
		else if (_MoveObj->IsA(AWH_WitchCauldronGimmick::StaticClass()))
		{
			for (TActorIterator<AWH_WitchCauldronGimmick> It(GetWorld()); It; ++It)
			{
				(*It)->Myactivetype = _ActiveNum;
			}
		}
		// 잠수함
		else if (_MoveObj->IsA(ASM_ComputerGimmick::StaticClass()))
		{
			for (TActorIterator<ASM_ComputerGimmick> It(GetWorld()); It; ++It)
			{
				(*It)->Myactivetype = _ActiveNum;
			}
		}
		else if (_MoveObj->IsA(ASM_PeriscopeGimmick::StaticClass()))
		{
			for (TActorIterator<ASM_PeriscopeGimmick> It(GetWorld()); It; ++It)
			{
				(*It)->Myactivetype = _ActiveNum;
			}
		}
		else if (_MoveObj->IsA(ASM_PressButtonGimmick::StaticClass()))
		{
			for (TActorIterator<ASM_PressButtonGimmick> It(GetWorld()); It; ++It)
			{
				(*It)->Myactivetype = _ActiveNum;
			}
		}
		else if (_MoveObj->IsA(ASM_WhistleGimmick::StaticClass()))
		{
			for (TActorIterator<ASM_WhistleGimmick> It(GetWorld()); It; ++It)
			{
				(*It)->Myactivetype = _ActiveNum;
			}
		}
		// 슈퍼마켓
		else if (_MoveObj->IsA(ASP_BottleGimmick::StaticClass()))
		{
			for (TActorIterator<ASP_BottleGimmick> It(GetWorld()); It; ++It)
			{
				(*It)->Myactivetype = _ActiveNum;
			}
		}
		else if (_MoveObj->IsA(ASP_CartGimmick::StaticClass()))
		{
			for (TActorIterator<ASP_CartGimmick> It(GetWorld()); It; ++It)
			{
				(*It)->Myactivetype = _ActiveNum;
			}
		}
		else if (_MoveObj->IsA(ASP_CleanerGimmick::StaticClass()))
		{
			for (TActorIterator<ASP_CleanerGimmick> It(GetWorld()); It; ++It)
			{
				(*It)->Myactivetype = _ActiveNum;
			}
		}
		else if (_MoveObj->IsA(ASP_ShowcaseGimmick::StaticClass()))
		{
			for (TActorIterator<ASP_ShowcaseGimmick> It(GetWorld()); It; ++It)
			{
				(*It)->Myactivetype = _ActiveNum;
			}
		}
		// 금고
		else if (_MoveObj->IsA(ABS_GoldBarGimmick::StaticClass()))
		{
			for (TActorIterator<ABS_GoldBarGimmick> It(GetWorld()); It; ++It)
			{
				(*It)->Myactivetype = _ActiveNum;
			}
		}
		else if (_MoveObj->IsA(ABS_HandleGimmick::StaticClass()))
		{
			for (TActorIterator<ABS_HandleGimmick> It(GetWorld()); It; ++It)
			{
				(*It)->Myactivetype = _ActiveNum;
			}
		}
		else if (_MoveObj->IsA(ABS_LaserGimmick::StaticClass()))
		{
			for (TActorIterator<ABS_LaserGimmick> It(GetWorld()); It; ++It)
			{
				(*It)->Myactivetype = _ActiveNum;
			}
		}
		else if (_MoveObj->IsA(ABS_SwitchGimmick::StaticClass()))
		{
			for (TActorIterator<ABS_SwitchGimmick> It(GetWorld()); It; ++It)
			{
				(*It)->Myactivetype = _ActiveNum;
			}
		}
	}
}

void ATestPlayer::ClientRPC_Init_Appearance_Implementation()
{
	ATestPlayer::Get_Player_Appearance();
}

void ATestPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATestPlayer, playerNum);
	DOREPLIFETIME(ATestPlayer, respawnLoc);
	DOREPLIFETIME(ATestPlayer, Current_SkeletalMesh);
	DOREPLIFETIME(ATestPlayer, Current_Accessories);
	DOREPLIFETIME(ATestPlayer, Current_Skins);
	DOREPLIFETIME(ATestPlayer, Current_Eyes);
	DOREPLIFETIME(ATestPlayer, Current_Top);
	DOREPLIFETIME(ATestPlayer, Current_Bottom);
	DOREPLIFETIME(ATestPlayer, Current_Outer);
	DOREPLIFETIME(ATestPlayer, Current_Dress);
	DOREPLIFETIME(ATestPlayer, bCanActive);
}

void ATestPlayer::ClientRPC_Update_PlayerNum_PlayerShowNum_Implementation(int32 _PlayerNum, int32 _PlayerShowNum)
{
	UE_LOG(LogTemp, Warning, TEXT("ClientRPC playernum : %d, player show num : %d"), _PlayerNum, _PlayerShowNum);
	playerNum = _PlayerNum;
	playerShowNum = _PlayerShowNum;
	//ServerRPC_Update_PlayerNum_PlayerShowNum(_PlayerNum, _PlayerShowNum);
	//OnRep_PlayerNum(); // player num 바뀌면 방 커스텀 UI 나오도록 하기
}

void ATestPlayer::ServerRPC_Update_PlayerNum_PlayerShowNum_Implementation(int32 _PlayerNum, int32 _PlayerShowNum)
{
	UE_LOG(LogTemp, Warning, TEXT("ServerRPC playernum : %d, player show num : %d"), _PlayerNum, _PlayerShowNum);
	playerNum = _PlayerNum;
	playerShowNum = _PlayerShowNum;
}

void ATestPlayer::Get_Player_Appearance()
{
	Get_Character();
	Get_Skins();
	Get_Eyes();
	Get_Accessory();
	Get_Top();
	Get_Bottom();
	Get_Outer();
	Get_Dress();
}

void ATestPlayer::Get_Character()
{
	FStructure_Player_Appearance_Result result = UFL_General::Get_Player_Appearance();
	if (result.bSuccess)
	{
		ServerRPC_Update_Character(result.Player_Appearance.Character.Mesh);
	} 
	else
	{
		return;
	}
}

void ATestPlayer::Get_Skins()
{
	FStructure_Player_Appearance_Result result = UFL_General::Get_Player_Appearance();
	if (result.bSuccess)
	{
		ServerRPC_Update_Skins(result.Player_Appearance.Skins_Slot.Material);
	}
	else
	{
		return;
	}
}

void ATestPlayer::Get_Eyes()
{
	FStructure_Player_Appearance_Result result = UFL_General::Get_Player_Appearance();
	if (result.bSuccess)
	{
		ServerRPC_Update_Eyes(result.Player_Appearance.Eyes_Slot.Material);
	}
	else
	{
		return;
	}
}

void ATestPlayer::Get_Accessory()
{
	FStructure_Player_Appearance_Result result = UFL_General::Get_Player_Appearance();
	if (result.bSuccess)
	{
		ServerRPC_Update_Accessory(result.Player_Appearance.Accessories_Slot.Mesh);
	}
	else
	{
		return;
	}
}

void ATestPlayer::Get_Top()
{
	FStructure_Player_Appearance_Result result = UFL_General::Get_Player_Appearance();
	if (result.bSuccess)
	{
		ServerRPC_Update_Top(result.Player_Appearance.Top_Slot.Mesh);
	}
	else
	{
		return;
	}
}

void ATestPlayer::Get_Bottom()
{
	FStructure_Player_Appearance_Result result = UFL_General::Get_Player_Appearance();
	if (result.bSuccess)
	{
		ServerRPC_Update_Bottom(result.Player_Appearance.Bottom_Slot.Mesh);
	}
	else
	{
		return;
	}
}

void ATestPlayer::Get_Outer()
{
	FStructure_Player_Appearance_Result result = UFL_General::Get_Player_Appearance();
	if (result.bSuccess)
	{
		ServerRPC_Update_Outer(result.Player_Appearance.Outer_Slot.Mesh);
	}
	else
	{
		return;
	}
}

void ATestPlayer::Get_Dress()
{
	FStructure_Player_Appearance_Result result = UFL_General::Get_Player_Appearance();
	if (result.bSuccess)
	{
		ServerRPC_Update_Dress(result.Player_Appearance.Dress_Slot.Mesh);
	}
	else
	{
		return;
	}
}

void ATestPlayer::ServerRPC_Update_Character_Implementation(USkeletalMesh* _mesh)
{
	Current_SkeletalMesh = _mesh;
	OnRep_Current_SkeletalMesh();
}

void ATestPlayer::ServerRPC_Update_Skins_Implementation(UMaterial* mat)
{
	Current_Skins = mat;
	OnRep_Current_Skins();
}

void ATestPlayer::ServerRPC_Update_Eyes_Implementation(UMaterial* mat)
{
	Current_Eyes = mat;
	OnRep_Current_Eyes();
}

void ATestPlayer::ServerRPC_Update_Accessory_Implementation(USkeletalMesh* _mesh)
{
	Current_Accessories = _mesh;
	OnRep_Current_Accessories();
}

void ATestPlayer::ServerRPC_Update_Top_Implementation(USkeletalMesh* _mesh)
{
	Current_Top = _mesh;
	OnRep_Current_Top();
}

void ATestPlayer::ServerRPC_Update_Bottom_Implementation(USkeletalMesh* _mesh)
{
	Current_Bottom = _mesh;
	OnRep_Current_Bottom();
}

void ATestPlayer::ServerRPC_Update_Outer_Implementation(USkeletalMesh* _mesh)
{
	Current_Outer = _mesh;
	OnRep_Current_Outer();
}

void ATestPlayer::ServerRPC_Update_Dress_Implementation(USkeletalMesh* _mesh)
{
	Current_Dress = _mesh;
	OnRep_Current_Dress();
}



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
#include "W_CustomMap.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Camera/CameraActor.h>
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>

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
	SM_Accessories->SetupAttachment(GetMesh());

	// 애니메이션 블루프린트를 로드하고 설정
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimBlueprint(TEXT("/Script/Engine.AnimBlueprint'/Game/Characters/Mannequins/Animations/ABP_Quinn.ABP_Quinn_C'"));
	if (AnimBlueprint.Succeeded())
	{
		AnimationBlueprintClass = AnimBlueprint.Object->GeneratedClass;
		SM_Accessories->SetAnimInstanceClass(AnimationBlueprintClass);
	}
	

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

	bReplicates = true;
	SetReplicateMovement(true);

	itemComponent = CreateDefaultSubobject<UItemComponent>(TEXT("itemComponent"));

}

// Called when the game starts or when spawned
void ATestPlayer::BeginPlay()
{
	Super::BeginPlay();

	// itemComponent = FindComponentByClass<UItemComponent>();
	
	clearUI = Cast<UW_StageClear>(CreateWidget(GetWorld(),stageClearUI));
	if (clearUI)
	{
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
	}
	if (lerpTime > 4.0)
	{
		GetWorldTimerManager().PauseTimer(ThunderT);
	}

	if (bCanActive)
	{
		GimmickSearch();
	}

	if (clearUI->curtime > 5 && HasAuthority())
	{
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
		enhancedinput->BindAction(ia_emo2, ETriggerEvent::Started, this, &ATestPlayer::ActiveGimmick);
		enhancedinput->BindAction(ia_emo3, ETriggerEvent::Started, this, &ATestPlayer::ActiveGimmick);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Fail"));
	}

}



void ATestPlayer::CreateSelectedWidget()
{
	UMapCustomWidget* CustomMapWidget = nullptr;

	switch(playerNum)
	{
	case 0:
		CustomMapWidget = Cast<UMapCustomWidget>(CreateWidget(GetWorld(), C_WitchHouseMap));
		break;
	case 1:
		CustomMapWidget = Cast<UMapCustomWidget>(CreateWidget(GetWorld(), C_SubmarineMap));
		break;
	case 2:
		CustomMapWidget = Cast<UMapCustomWidget>(CreateWidget(GetWorld(), C_BankMap));
		break;
	case 3:
		CustomMapWidget = Cast<UMapCustomWidget>(CreateWidget(GetWorld(), C_SupermarketMap));
		break;
	default:
		break;
	}

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
	if (Current_Skins->IsValidLowLevelFast())
	{
		GetMesh()->SetMaterialByName(FName(TEXT("M_Bull_Dandelion_BODY")), Current_Skins);
	}
}

void ATestPlayer::OnRep_Current_Eyes()
{
	if (Current_Eyes->IsValidLowLevelFast())
	{
		GetMesh()->SetMaterialByName(FName(TEXT("pasted__M_Bull_Dandelion_EYES")), Current_Eyes);
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
	Cast<UUserEmoticon>(emoticonUI->GetWidget())->ShowEmoticon(inputNum - 1);
}

void ATestPlayer::DeathCounting()
{
	gameState->SetDeathCountUp(playerNum);
}

void ATestPlayer::FadeInOut(bool bInOut)
{
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
	}
}

void ATestPlayer::BlackScreen()
{
	APlayerCameraManager* cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	cameraManager->StartCameraFade(0, 1.0f, 0.1f, FColor::Black, false, true);
	bIsBlack = true;
}

void ATestPlayer::Respawn(float delaytime)
{
	if (!bIsBlack)
	{
		FadeInOut(true);
	}
	UE_LOG(LogTemp, Warning, TEXT("Player %d Respawn"), playerNum);
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	FTimerHandle respawnT;
	GetWorldTimerManager().SetTimer(respawnT, [&](){
		GetCapsuleComponent()->SetSimulatePhysics(false);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		GetCapsuleComponent()->SetRelativeRotation(FRotator(0, 0, 0));
		GetMesh()->SetRelativeScale3D(FVector(1.0, 1.0, 1.0));
		GetMesh()->SetRelativeRotation(FRotator(0,-90,0));
		SetActorLocation(respawnLoc);
		cameraBoom->SetRelativeLocation(FVector(0,0, 170));
		FadeInOut(false);
		bIsDie = false;
		bIsBlack = false;
		GetWorldTimerManager().ClearAllTimersForObject(this);
	}, 1.0, false, 3.0);
	
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
		DrawDebugLine(GetWorld(), start, hitInfo.ImpactPoint, FColor::Red, false, -1, 0, 1);
		DrawDebugLine(GetWorld(), hitInfo.ImpactPoint, end, FColor::Green, false, -1, 0, 1);
	}
	else
	{
		DrawDebugLine(GetWorld(), start, end, FColor::Red, false, -1, 0, 1);
	}
}

void ATestPlayer::Death_Fallover()
{
	bIsDie = true;
	lerpTime = 0;
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
		Respawn(10.0);
	}
}

void ATestPlayer::Death_EndMan()
{
	bIsDie = true;
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
	bIsDie = true;
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
	SetActorLocation(FVector(0, 0, 0));
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
	CreateWidget(GetWorld(),voteUI)->AddToViewport(0);
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
		return;
	}
	MultiRPC_ActiveGimmick(aP);
}

void ATestPlayer::MultiRPC_ActiveGimmick_Implementation(ATestPlayer* _aP)
{
	if (bCanActive)
	{
		//UE_LOG(LogTemp, Warning, TEXT("aP = %f, %f, %f"), _aP->GetActorLocation().X, _aP->GetActorLocation().Y, _aP->GetActorLocation().Z);
		//UE_LOG(LogTemp, Warning, TEXT("P = %f, %f, %f"), GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z);
		bCanActive = false;
		int32 key = g->OnMyActive(_aP);
		//UE_LOG(LogTemp, Warning, TEXT("Fail %d"), key);
		if (key == 2)
		{
			bCanOpenDoor = true;
			gameState->bOnGame = false;
			clearUI->SetWidgetState();
			clearUI->bIsClear = true;
			
			clearUI->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void ATestPlayer::ServerRPC_MoveStage_Implementation()
{
	MultiRPC_MoveStage(gameState->stageLoc[gameState->stageNum]);
	UE_LOG(LogTemp, Warning, TEXT("Stage %d"), gameState->stageNum);
}

void ATestPlayer::MultiRPC_MoveStage_Implementation(FVector moveLoc)
{
	gameState->MoveNextStage(moveLoc);
	respawnLoc = moveLoc;
	clearUI->SetVisibility(ESlateVisibility::Hidden);
	if (gameState->stageNum > 1)
	{
		ServerRPC_StartGetFinalScore();
	}
}

void ATestPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATestPlayer, playerNum);

	DOREPLIFETIME(ATestPlayer, Current_SkeletalMesh);
	DOREPLIFETIME(ATestPlayer, Current_Accessories);
	DOREPLIFETIME(ATestPlayer, Current_Skins);
}
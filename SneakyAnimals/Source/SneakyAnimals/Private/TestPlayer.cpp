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

// Sets default values
ATestPlayer::ATestPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90.0f));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	GetMesh()->SetOwnerNoSee(true);

	cameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	cameraBoom->SetupAttachment(GetMesh());
	cameraBoom->SetRelativeLocation(FVector(0, 0, 90));
	cameraBoom->TargetArmLength = 0;
	cameraBoom->bUsePawnControlRotation = true;

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(cameraBoom, USpringArmComponent::SocketName);
	camera->bUsePawnControlRotation = false;
	

}

// Called when the game starts or when spawned
void ATestPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(imc_testmove, 0);
		}
	}

}

// Called every frame
void ATestPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	lerpTime += DeltaTime;
	if (lerpTime > 1)
	{
		lerpTime = 0;
		GetWorldTimerManager().PauseTimer(falloverT);
		GetWorldTimerManager().PauseTimer(poorDriveT);
		GetWorldTimerManager().PauseTimer(endManT);
	}

	if (bCanActive)
	{
		GimmickSearch();
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
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Fail"));
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
	if (bCanActive)
	{
		UE_LOG(LogTemp, Warning, TEXT("Fail %d"), bCanActive);
		if (Cast<AWH_BookshelfGimmick>(g))
		{
			int32 key = Cast<AWH_BookshelfGimmick>(g)->OnMyActive(this);
			if (key == 2)
			{
				bCanOpenDoor = true;
			}
		}
		else if (Cast<AWH_WitchCauldronGimmick>(g))
		{
			int32 key = Cast<AWH_WitchCauldronGimmick>(g)->OnMyActive(this);
			if (key == 2)
			{
				bCanOpenDoor = true;
			}
		}
		else if (Cast<AWH_BroomstickGimmick>(g))
		{
			int32 key = Cast<AWH_BroomstickGimmick>(g)->OnMyActive(this);
			if (key == 2)
			{
				bCanOpenDoor = true;
			}
		}
		else if (Cast<AWH_PotionGimmick>(g))
		{
			int32 key = Cast<AWH_PotionGimmick>(g)->OnMyActive(this);
			if (key == 2)
			{
				bCanOpenDoor = true;
			}
		}
	}
}

void ATestPlayer::FadeInOut(bool bInOut)
{
	APlayerCameraManager* cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(),0);
	if (bInOut)
	{
		cameraManager->StartCameraFade(0,1.0f, 1.0f, FColor::Black, false, true);
	}
	else
	{
		cameraManager->StartCameraFade(1.0f, 0, 1.5f, FColor::Black);
	}
}

void ATestPlayer::Respawn(float delaytime)
{
	FadeInOut(true);
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	FTimerHandle respawnT;
	GetWorldTimerManager().SetTimer(respawnT, [&](){
		GetCapsuleComponent()->SetSimulatePhysics(false);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		GetCapsuleComponent()->SetRelativeRotation(FRotator(0, 0, 0));
		GetMesh()->SetRelativeScale3D(FVector(1.0, 1.0, 1.0));
		GetMesh()->SetRelativeRotation(FRotator(0,-90,0));
		SetActorLocation(respawnLoc);
		cameraBoom->SetRelativeLocation(FVector(0,0,90.0));
		FadeInOut(false);
		bIsDie = false;
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
			float eye = FMath::Lerp(90.0, 0, lerpTime);
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
	UE_LOG(LogTemp, Warning, TEXT("%d"), bIsBestDriver);
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
					UE_LOG(LogTemp, Warning, TEXT("%f, %f, %f"), burstLoc.X, burstLoc.Y, burstLoc.Z);
					Controller->SetControlRotation(FRotator(0, burstRot.Yaw, burstRot.Roll));
					UE_LOG(LogTemp, Warning, TEXT("%f, %f, %f"), burstRot.Pitch, burstRot.Yaw, burstRot.Roll);
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


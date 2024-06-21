// Fill out your copyright notice in the Description page of Project Settings.


#include "TestPlayer.h"
#include "Engine.h"
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h>
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/SpringArmComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Camera/CameraComponent.h>
#include "Gimmick.h"
#include "WH_BookshelfGimmick.h"

// Sets default values
ATestPlayer::ATestPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90.0f));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	cameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	cameraBoom->SetupAttachment(RootComponent);
	cameraBoom->SetRelativeLocation(FVector(0, 40, 60));
	cameraBoom->TargetArmLength = 300.0f;
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
	}
}

void ATestPlayer::Respawn()
{
	GetCharacterMovement()->StopActiveMovement();
	FTimerHandle respawnT;
	GetWorldTimerManager().SetTimer(respawnT, [&](){
		GetMesh()->SetRelativeScale3D(FVector(1.0, 1.0, 1.0));
		SetActorLocation(respawnLoc);
	}, 1.0, false, 3.0);
	
}

void ATestPlayer::GimmickSearch()
{
	FHitResult hitInfo;
	FVector start = GetActorLocation();
	FVector end = GetActorLocation() + GetActorForwardVector() * 200.0;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);

	bool result = GetWorld()->LineTraceSingleByObjectType(hitInfo, start, end, ECC_GameTraceChannel1 , params);
	DrawDebugLine(GetWorld(),start, end, FColor::Red, false, -1, 0, 1);
	if (result)
	{
		g = Cast<AGimmick>(hitInfo.GetActor());
		if (g)
		{
			g->bCanActive = true;
		}
	}
}

void ATestPlayer::Death_Fallover()
{
	GetCharacterMovement()->StopActiveMovement();
	lerpTime = 0;
	GetWorldTimerManager().SetTimer(falloverT, [&]()
		{
			float tall = FMath::Lerp(1.0, 0.3, lerpTime);
			GetMesh()->SetRelativeScale3D(GetMesh()->GetRelativeScale3D() * FVector(1,1,tall));
		}, 0.03f, true, 0);
	Respawn();
}

